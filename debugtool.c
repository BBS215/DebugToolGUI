#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "debugtool.h"
#include "hid.h"

#define DEBUG_DEV_REPORT_ID			(BYTE)0x5
#define DEBUG_DEV_CMD_READ_8		(BYTE)0x1
#define DEBUG_DEV_CMD_READ_16		(BYTE)0x2
#define DEBUG_DEV_CMD_WRITE_8		(BYTE)0x3
#define DEBUG_DEV_CMD_WRITE_16		(BYTE)0x4

void usleep(__int64 usec)
{
	HANDLE timer;
	LARGE_INTEGER ft;

	ft.QuadPart = -(10 * usec); // Convert to 100 nanosecond interval, negative value indicates relative time

	timer = CreateWaitableTimer(NULL, TRUE, NULL);
	SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
}

PHID_DEVICE Find_My_Keyboard(PHID_DEVICE deviceList, ULONG numDevices,
						uint16_t Device_VID, uint16_t Device_PID, uint16_t Device_UsagePage, uint16_t Device_Usage)
{
	ULONG i;
	PHID_DEVICE my_device = NULL;
	if (!deviceList) return NULL;
	if (!numDevices) return NULL;
	for (i = 0; i < numDevices; i++)
	{
		if (INVALID_HANDLE_VALUE != deviceList[i].HidDevice)
		{
			if ((deviceList[i].Attributes.VendorID == Device_VID) &&
				(deviceList[i].Attributes.ProductID == Device_PID) &&
				(deviceList[i].Caps.UsagePage == Device_UsagePage) &&
				(deviceList[i].Caps.Usage == Device_Usage))
			{
				my_device = &deviceList[i];
				break;
			}
		}
	}
	return my_device;
}

int Write_to_device(uint8_t *p_buffer, uint8_t buf_size, uint16_t Device_VID, uint16_t Device_PID, uint16_t Device_UsagePage, uint16_t Device_Usage)
{
	HID_DEVICE			writeDevice;
	DWORD				bytesWritten = 0;
	BOOL				status = FALSE;
	ULONG				SetUsageStatus = 0;
	uint8_t				write_buffer[8];
	int					ret = 0;
	PHID_DEVICE			deviceList = NULL;
	ULONG				numDevices = 0;
	PHID_DEVICE			pDevice = NULL;

	if (!p_buffer) return -1;
	if (!buf_size) return -2;
	if (buf_size > 8) return -3;
	ZeroMemory(&writeDevice, sizeof(writeDevice));
	ZeroMemory(&write_buffer, 8);
	memcpy(write_buffer, p_buffer, buf_size);

	FindKnownHidDevices(&deviceList, &numDevices);
	pDevice = Find_My_Keyboard(deviceList, numDevices, Device_VID, Device_PID, Device_UsagePage, Device_Usage);
	if (!pDevice) {
		if (deviceList) {
			free(deviceList);
			deviceList = NULL;
		}
		return -4;
	}

	status = OpenHidDevice(pDevice->DevicePath,
		FALSE,
		TRUE,
		FALSE,
		FALSE,
		&writeDevice);
	if (deviceList) {
		free(deviceList);
		deviceList = NULL;
		pDevice = NULL;
	}
	if (!status) return -5;

	SetUsageStatus = dll_HidP_SetUsageValue(HidP_Output,
		writeDevice.OutputData->UsagePage,
		0, // All Collections
		writeDevice.OutputData->ValueData.Usage,
		writeDevice.OutputData->ValueData.Value,
		writeDevice.Ppd,
		writeDevice.OutputReportBuffer,
		writeDevice.Caps.OutputReportByteLength);

	if (SetUsageStatus != HIDP_STATUS_SUCCESS)
	{
		ret = -6;
	} else {
		status = WriteFile(writeDevice.HidDevice,
			write_buffer,
			writeDevice.Caps.OutputReportByteLength,
			&bytesWritten,
			NULL);
		if (!status) ret = -7;
		if ((status) && (bytesWritten != (uint32_t)writeDevice.Caps.OutputReportByteLength)) ret = -8;
	}
	CloseHidDevice(&writeDevice);
	return ret;
}

int Read_from_device(_In_ BYTE reportID, _Out_ uint8_t *p_buffer, _In_ uint8_t buf_size,
	uint16_t Device_VID, uint16_t Device_PID, uint16_t Device_UsagePage, uint16_t Device_Usage)
{
	HID_DEVICE  syncDevice;
	BOOL status;
	PHID_DEVICE deviceList = NULL;
	ULONG       numDevices = 0;
	PHID_DEVICE	pDevice = NULL;
	int ret = 0;

	if (!p_buffer) return -1;
	if (!buf_size) return -2;

	RtlZeroMemory(&syncDevice, sizeof(syncDevice));

	FindKnownHidDevices(&deviceList, &numDevices);
	pDevice = Find_My_Keyboard(deviceList, numDevices, Device_VID, Device_PID, Device_UsagePage, Device_Usage);
	if (!pDevice) {
		if (deviceList) {
			free(deviceList);
			deviceList = NULL;
		}
		return -3;
	}

	status = OpenHidDevice(pDevice->DevicePath,
		TRUE,
		FALSE,
		FALSE,
		FALSE,
		&syncDevice);
	if (deviceList) {
		free(deviceList);
		deviceList = NULL;
		pDevice = NULL;
	}
	if (!status) return -4;

	syncDevice.InputReportBuffer[0] = reportID;
	status = dll_HidD_GetInputReport(syncDevice.HidDevice,
		syncDevice.InputReportBuffer,
		syncDevice.Caps.InputReportByteLength);
	if (!status)
	{
		//printf("HidD_GetInputReport() failed. Error: 0x%X\n", GetLastError());
		ret = -5;
	} else {
		ZeroMemory(p_buffer, buf_size);
		ret = (syncDevice.Caps.InputReportByteLength<buf_size)?syncDevice.Caps.InputReportByteLength:buf_size;
		memcpy(p_buffer, syncDevice.InputReportBuffer, ret);
	}
	CloseHidDevice(&syncDevice);
	return ret;
}

int Read_Register(uint16_t reg_addr, uint8_t width, uint16_t *p_reg_data,
					uint16_t Device_VID, uint16_t Device_PID, uint16_t Device_UsagePage, uint16_t Device_Usage)
{
	uint8_t buf[6];
	int retry_count = 15; // 15 sec
	int ret;
	uint8_t cmd = 0;

	if (width == 16) cmd = DEBUG_DEV_CMD_READ_16;
	else			cmd = DEBUG_DEV_CMD_READ_8;
	buf[0] = DEBUG_DEV_REPORT_ID;
	buf[1] = cmd;
	buf[2] = (uint8_t)((reg_addr >> 8) & 0xFF);
	buf[3] = (uint8_t)(reg_addr & 0xFF);
	buf[4] = 0;
	buf[5] = 0;
	ret = Write_to_device(buf, 6, Device_VID, Device_PID, Device_UsagePage, Device_Usage);
	while ((ret) && (retry_count)) {
		usleep(1 * 1000 * 1000); // 1 sec
		ret = Write_to_device(buf, 6, Device_VID, Device_PID, Device_UsagePage, Device_Usage);
		retry_count--;
	};
	if (ret < 0) return (ret-10);
	ZeroMemory(buf, 6);
	ret = Read_from_device(DEBUG_DEV_REPORT_ID, buf, 6, Device_VID, Device_PID, Device_UsagePage, Device_Usage);
	if (ret < 6) return -3;
	if (buf[0] != DEBUG_DEV_REPORT_ID) return -4;
	if (buf[1] != cmd) return -5;
	if (buf[2] != (uint8_t)((reg_addr >> 8) & 0xFF)) return -6;
	if (buf[3] != (uint8_t)(reg_addr & 0xFF)) return -6;
	if (p_reg_data) *p_reg_data = (uint16_t)((uint16_t)buf[5] | ((uint16_t)(buf[4]) << 8));
	return 0;
}

int Write_Register(uint16_t reg_addr, uint8_t width, uint16_t reg_data,
					uint16_t Device_VID, uint16_t Device_PID, uint16_t Device_UsagePage, uint16_t Device_Usage)
{
	uint8_t buf[6];
	int retry_count = 15; // 15 sec
	int ret;
	uint8_t cmd = 0;

	if (width == 16) cmd = DEBUG_DEV_CMD_WRITE_16;
	else			cmd = DEBUG_DEV_CMD_WRITE_8;
	buf[0] = DEBUG_DEV_REPORT_ID;
	buf[1] = cmd;
	buf[2] = (uint8_t)((reg_addr >> 8) & 0xFF);
	buf[3] = (uint8_t)(reg_addr & 0xFF);
	buf[4] = (uint8_t)((reg_data >> 8) & 0xFF);
	buf[5] = (uint8_t)(reg_data & 0xFF);
	ret = Write_to_device(buf, 6, Device_VID, Device_PID, Device_UsagePage, Device_Usage);
	while ((ret) && (retry_count)) {
		usleep(1 * 1000 * 1000); // 1 sec
		ret = Write_to_device(buf, 6, Device_VID, Device_PID, Device_UsagePage, Device_Usage);
		retry_count--;
	};
	if (ret < 0) return (ret-10);
	return 0;
}

int Check_ConnectDevice(uint16_t Device_VID, uint16_t Device_PID, uint16_t Device_UsagePage, uint16_t Device_Usage)
{
	PHID_DEVICE 	deviceList = NULL;
	ULONG       	numDevices = 0;
	PHID_DEVICE		pDevice = NULL;
	int 			ret = 0;

	FindKnownHidDevices(&deviceList, &numDevices);
	pDevice = Find_My_Keyboard(deviceList, numDevices, Device_VID, Device_PID, Device_UsagePage, Device_Usage);
	if (!pDevice) ret = -1;
	else ret = 0;
	if (deviceList) {
		free(deviceList);
		deviceList = NULL;
	}
	return ret;
}

