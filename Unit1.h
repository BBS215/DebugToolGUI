//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label_status;
	TEdit *Edit_VID;
	TEdit *Edit_PID;
	TEdit *Edit_UsagePage;
	TEdit *Edit_Usage;
	TButton *Button_read;
	TEdit *Edit_RegAddr;
	TLabel *Label5;
	TCheckBox *CheckBox_RegWidth;
	TEdit *Edit_RegDataHEX;
	TLabel *Label6;
	TButton *Button_write;
	TLabel *Label7;
	TEdit *Edit_RegDataBIN;
	TLabel *Label8;
	TEdit *Edit_RegDataDEC;
	TPanel *Panel1;
	TLabel *Label9;
	TLabel *Label10;
	TLabel *Label11;
	void __fastcall Edit_VIDKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall Edit_RegDataHEXKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall Edit_RegDataBINKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall Edit_RegDataHEXKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit_RegDataBINKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit_RegDataDECKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall Edit_RegDataDECKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Button_readClick(TObject *Sender);
	void __fastcall Button_writeClick(TObject *Sender);




private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
