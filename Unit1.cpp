//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "debugtool.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
int is_connected = 0;
uint16_t Device_VID;
uint16_t Device_PID;
uint16_t Device_UsagePage;
uint16_t Device_Usage;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void Edit_RegDataHEX_changed(void)
{
	uint16_t RegData = 0;
	UnicodeString temp_str;
	int i;

	RegData = (uint16_t)wcstoul(Form1->Edit_RegDataHEX->Text.c_str(), NULL, 16);
	temp_str = L"";
	for(i=0;i<16;i++) {
		if (RegData & (1 << i)) {
			temp_str.Insert(L"1", 0);
		} else {
			temp_str.Insert(L"0", 0);
		}
	}
	Form1->Edit_RegDataBIN->Text = temp_str;
	temp_str.printf(L"%d", RegData);
	Form1->Edit_RegDataDEC->Text = temp_str;
}

void Edit_RegDataBIN_changed(void)
{
	uint16_t RegData = 0;
	UnicodeString temp_str;
	RegData = (uint16_t)wcstoul(Form1->Edit_RegDataBIN->Text.c_str(), NULL, 2);
	if (Form1->CheckBox_RegWidth->Checked) temp_str.printf(L"%04x", RegData);
	else temp_str.printf(L"%02x", RegData);
	Form1->Edit_RegDataHEX->Text = temp_str;
	temp_str.printf(L"%d", RegData);
	Form1->Edit_RegDataDEC->Text = temp_str;
}

void Edit_RegDataDEC_changed(void)
{
	uint16_t RegData = 0;
	UnicodeString temp_str;
	int i;

	RegData = (uint16_t)wcstoul(Form1->Edit_RegDataDEC->Text.c_str(), NULL, 10);
	temp_str = L"";
	for(i=0;i<16;i++) {
		if (RegData & (1 << i)) {
			temp_str.Insert(L"1", 0);
		} else {
			temp_str.Insert(L"0", 0);
		}
	}
	Form1->Edit_RegDataBIN->Text = temp_str;
	if (Form1->CheckBox_RegWidth->Checked) temp_str.printf(L"%04x", RegData);
	else temp_str.printf(L"%02x", RegData);
	Form1->Edit_RegDataHEX->Text = temp_str;
}

void DeviceConnect(void)
{
	int ret;
	int i;
	/// Disconnect
	is_connected = 0;
	Form1->Label_status->Caption = L"Не подключено";
	Form1->Label_status->Font->Color = clRed;
	/// Connect
	Device_VID = (uint16_t)wcstoul(Form1->Edit_VID->Text.c_str(), NULL, 16);
	Device_PID = (uint16_t)wcstoul(Form1->Edit_PID->Text.c_str(), NULL, 16);
	Device_UsagePage = (uint16_t)wcstoul(Form1->Edit_UsagePage->Text.c_str(), NULL, 16);
	Device_Usage = (uint16_t)wcstoul(Form1->Edit_Usage->Text.c_str(), NULL, 16);
	ret = Check_ConnectDevice(Device_VID, Device_PID, Device_UsagePage, Device_Usage);
	if (ret < 0) return; // not connected
	is_connected = 1;
	Form1->Label_status->Caption = L"Подключено";
	Form1->Label_status->Font->Color = clGreen;
	Application->ProcessMessages();
}

void ReadReg(void)
{
	int ret;
	uint16_t reg_addr;
	uint8_t width = 16;
	uint16_t reg_data = 0;
	UnicodeString temp_str;

	Form1->Button_read->Enabled = false;
	Form1->Button_write->Enabled = false;
	Application->ProcessMessages();

	DeviceConnect();
	if (!is_connected) goto exit;
	Form1->Label_status->Caption = L"Чтение регистра...";
	Form1->Label_status->Font->Color = clBlue;
	Application->ProcessMessages();
	if (Form1->Edit_RegAddr->Text.Length() == 0) {
		Form1->Label_status->Caption = L"Введите адрес регистра";
		Form1->Label_status->Font->Color = clRed;
		goto exit;
	}
	reg_addr = (uint16_t)wcstoul(Form1->Edit_RegAddr->Text.c_str(), NULL, 16);
	if (Form1->CheckBox_RegWidth->Checked) width = 16;
	else width = 8;
	ret = Read_Register(reg_addr, width, &reg_data, Device_VID, Device_PID, Device_UsagePage, Device_Usage);
	if (ret) {
		temp_str.printf(L"Ошибка %d", ret);
		Form1->Label_status->Caption = temp_str;
		Form1->Label_status->Font->Color = clRed;
	} else {
		if (Form1->CheckBox_RegWidth->Checked) temp_str.printf(L"%04x", reg_data);
		else temp_str.printf(L"%02x", reg_data);
		Form1->Edit_RegDataHEX->Text = temp_str;
		Edit_RegDataHEX_changed();
		Form1->Label_status->Caption = L"Регистр считан";
		Form1->Label_status->Font->Color = clGreen;
	}

exit:
	Form1->Button_read->Enabled = true;
	Form1->Button_write->Enabled = true;
}

void WriteReg(void)
{
	int ret;
	uint16_t reg_addr;
	uint8_t width = 16;
	uint16_t reg_data = 0;
	UnicodeString temp_str;

	Form1->Button_read->Enabled = false;
	Form1->Button_write->Enabled = false;
	Application->ProcessMessages();

	DeviceConnect();
	if (!is_connected) goto exit;
	Form1->Label_status->Caption = L"Запись регистра...";
	Form1->Label_status->Font->Color = clBlue;
	Application->ProcessMessages();
	if (Form1->Edit_RegAddr->Text.Length() == 0) {
		Form1->Label_status->Caption = L"Введите адрес регистра";
		Form1->Label_status->Font->Color = clRed;
		goto exit;
	}
	reg_addr = (uint16_t)wcstoul(Form1->Edit_RegAddr->Text.c_str(), NULL, 16);
	if (Form1->CheckBox_RegWidth->Checked) width = 16;
	else width = 8;
	reg_data = (uint16_t)wcstoul(Form1->Edit_RegDataHEX->Text.c_str(), NULL, 16);
	ret = Write_Register(reg_addr, width, reg_data, Device_VID, Device_PID, Device_UsagePage, Device_Usage);
	if (ret) {
		temp_str.printf(L"Ошибка %d", ret);
		Form1->Label_status->Caption = temp_str;
		Form1->Label_status->Font->Color = clRed;
	} else {
		Form1->Label_status->Caption = L"Регистр записан";
		Form1->Label_status->Font->Color = clGreen;
	}

exit:
	Form1->Button_read->Enabled = true;
	Form1->Button_write->Enabled = true;
}


//---------------------------------------------------------------------------
void __fastcall TForm1::Edit_VIDKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (((Key >= L'0')&&(Key <= L'9'))
		||((Key >= L'a')&&(Key <= L'f'))
		||(Key == L'\b'))
	{
	 // OK
	} else {
		Key = 0;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
	if (LoadHIDLib()) {
		MessageBox(NULL, L"Ошибка! HID.DLL либо SETUPAPI.DLL не может быть загружен.", L"Ошибка", 0);
		Form1->Close();
		return;
	}
	DeviceConnect();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormDestroy(TObject *Sender)
{
	UnloadHIDLib();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Edit_RegDataHEXKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (((Key >= L'0')&&(Key <= L'9'))
		||((Key >= L'a')&&(Key <= L'f'))
		||(Key == L'\b'))
	{
		// OK
	} else {
		Key = 0;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Edit_RegDataHEXKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)
{
	Edit_RegDataHEX_changed();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Edit_RegDataBINKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (((Key >= L'0')&&(Key <= L'1'))
		||(Key == L'\b'))
	{
		// OK
	} else {
		Key = 0;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Edit_RegDataBINKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)

{
   Edit_RegDataBIN_changed();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Edit_RegDataDECKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (((Key >= L'0')&&(Key <= L'9'))
		||(Key == L'\b'))
	{
		// OK
	} else {
		Key = 0;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Edit_RegDataDECKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)

{
   Edit_RegDataDEC_changed();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button_readClick(TObject *Sender)
{
	ReadReg();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button_writeClick(TObject *Sender)
{
	WriteReg();
}
//---------------------------------------------------------------------------

