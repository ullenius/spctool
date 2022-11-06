object FrmProg: TFrmProg
  Left = 589
  Top = 541
  AutoSize = True
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 2
  ClientHeight = 113
  ClientWidth = 260
  Color = clBtnFace
  Font.Charset = SHIFTJIS_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = '‚l‚r ‚oƒSƒVƒbƒN'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 12
  object pbCurrent: TPaintBox
    Left = 0
    Top = 36
    Width = 260
    Height = 20
  end
  object lblStatus: TLabel
    Left = 0
    Top = 20
    Width = 192
    Height = 16
    AutoSize = False
  end
  object lblProgress: TLabel
    Left = 228
    Top = 20
    Width = 32
    Height = 16
    Alignment = taRightJustify
    AutoSize = False
  end
  object lblInfo: TLabel
    Left = 0
    Top = 0
    Width = 260
    Height = 16
    AutoSize = False
  end
  object pbTotal: TPaintBox
    Left = 0
    Top = 64
    Width = 260
    Height = 20
  end
  object btnCancel: TButton
    Left = 90
    Top = 88
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    TabOrder = 0
    OnClick = btnCancelClick
  end
end
