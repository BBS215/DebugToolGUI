#pragma once

#ifdef __cplusplus
extern "C" {
#endif
int LoadHIDLib(void);
void UnloadHIDLib(void);
int Check_ConnectDevice(uint16_t Device_VID, uint16_t Device_PID, uint16_t Device_UsagePage, uint16_t Device_Usage);
int Read_Register(uint16_t reg_addr, uint8_t width, uint16_t *p_reg_data,
					uint16_t Device_VID, uint16_t Device_PID, uint16_t Device_UsagePage, uint16_t Device_Usage);
int Write_Register(uint16_t reg_addr, uint8_t width, uint16_t reg_data,
					uint16_t Device_VID, uint16_t Device_PID, uint16_t Device_UsagePage, uint16_t Device_Usage);
#ifdef __cplusplus
}
#endif

