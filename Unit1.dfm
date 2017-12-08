object Form1: TForm1
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'DebugToolGUI'
  ClientHeight = 200
  ClientWidth = 369
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 19
    Top = 13
    Width = 64
    Height = 13
    Caption = 'VendorID: 0x'
  end
  object Label2: TLabel
    Left = 139
    Top = 13
    Width = 67
    Height = 13
    Caption = 'ProductID: 0x'
  end
  object Label3: TLabel
    Left = 8
    Top = 37
    Width = 76
    Height = 13
    Caption = 'Usage Page: 0x'
  end
  object Label4: TLabel
    Left = 157
    Top = 37
    Width = 49
    Height = 13
    Caption = 'Usage: 0x'
  end
  object Label_status: TLabel
    Left = 208
    Top = 76
    Width = 80
    Height = 13
    Caption = #1053#1077' '#1087#1086#1076#1082#1083#1102#1095#1077#1085#1086
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object Label5: TLabel
    Left = 7
    Top = 76
    Width = 99
    Height = 13
    Caption = #1040#1076#1088#1077#1089' '#1088#1077#1075#1080#1089#1090#1088#1072': 0x'
  end
  object Label6: TLabel
    Left = 21
    Top = 103
    Width = 85
    Height = 13
    Caption = #1044#1072#1085#1085#1099#1077': HEX: 0x'
  end
  object Label7: TLabel
    Left = 70
    Top = 157
    Width = 36
    Height = 13
    Caption = 'BIN: 0b'
  end
  object Label8: TLabel
    Left = 67
    Top = 130
    Width = 24
    Height = 13
    Caption = 'DEC:'
  end
  object Label9: TLabel
    Left = 111
    Top = 181
    Width = 96
    Height = 13
    Caption = '5432109876543210'
  end
  object Label10: TLabel
    Left = 111
    Top = 172
    Width = 36
    Height = 13
    Caption = '111111'
  end
  object Label11: TLabel
    Left = 90
    Top = 176
    Width = 16
    Height = 13
    Caption = 'Bit:'
  end
  object Edit_VID: TEdit
    Left = 86
    Top = 10
    Width = 44
    Height = 21
    CharCase = ecLowerCase
    MaxLength = 4
    TabOrder = 0
    Text = '043b'
    OnKeyPress = Edit_VIDKeyPress
  end
  object Edit_PID: TEdit
    Left = 208
    Top = 10
    Width = 44
    Height = 21
    CharCase = ecLowerCase
    MaxLength = 4
    TabOrder = 1
    Text = '0325'
    OnKeyPress = Edit_VIDKeyPress
  end
  object Edit_UsagePage: TEdit
    Left = 86
    Top = 34
    Width = 44
    Height = 21
    CharCase = ecLowerCase
    MaxLength = 4
    TabOrder = 2
    Text = 'ff00'
    OnKeyPress = Edit_VIDKeyPress
  end
  object Edit_Usage: TEdit
    Left = 208
    Top = 34
    Width = 44
    Height = 21
    CharCase = ecLowerCase
    MaxLength = 4
    TabOrder = 3
    Text = '03'
    OnKeyPress = Edit_VIDKeyPress
  end
  object Button_read: TButton
    Left = 231
    Top = 98
    Width = 112
    Height = 25
    Caption = #1057#1095#1080#1090#1072#1090#1100' '#1088#1077#1075#1080#1089#1090#1088
    TabOrder = 4
    OnClick = Button_readClick
  end
  object Edit_RegAddr: TEdit
    Left = 108
    Top = 73
    Width = 48
    Height = 21
    MaxLength = 4
    TabOrder = 5
    OnKeyPress = Edit_VIDKeyPress
  end
  object CheckBox_RegWidth: TCheckBox
    Left = 162
    Top = 102
    Width = 50
    Height = 17
    Caption = '16 '#1073#1080#1090
    TabOrder = 6
  end
  object Edit_RegDataHEX: TEdit
    Left = 108
    Top = 100
    Width = 48
    Height = 21
    MaxLength = 4
    TabOrder = 7
    Text = '00'
    OnKeyPress = Edit_RegDataHEXKeyPress
    OnKeyUp = Edit_RegDataHEXKeyUp
  end
  object Button_write: TButton
    Left = 231
    Top = 125
    Width = 112
    Height = 25
    Caption = #1047#1072#1087#1080#1089#1072#1090#1100' '#1088#1077#1075#1080#1089#1090#1088
    TabOrder = 8
    OnClick = Button_writeClick
  end
  object Edit_RegDataBIN: TEdit
    Left = 108
    Top = 154
    Width = 110
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    MaxLength = 16
    ParentFont = False
    TabOrder = 9
    Text = '0000000000000000'
    OnKeyPress = Edit_RegDataBINKeyPress
    OnKeyUp = Edit_RegDataBINKeyUp
  end
  object Edit_RegDataDEC: TEdit
    Left = 108
    Top = 127
    Width = 48
    Height = 21
    MaxLength = 5
    TabOrder = 10
    Text = '0'
    OnKeyPress = Edit_RegDataDECKeyPress
    OnKeyUp = Edit_RegDataDECKeyUp
  end
  object Panel1: TPanel
    Left = 8
    Top = 61
    Width = 353
    Height = 5
    TabOrder = 11
  end
end
