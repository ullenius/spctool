object FrmSSI: TFrmSSI
  Left = 725
  Top = 226
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  BorderWidth = 1
  Caption = 'Sound Source Options'
  ClientHeight = 247
  ClientWidth = 293
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object lblRate: TLabel
    Left = 3
    Top = 4
    Width = 61
    Height = 13
    AutoSize = False
    Caption = 'Mid-C Rate'
  end
  object lblOfs: TLabel
    Left = 4
    Top = 132
    Width = 61
    Height = 13
    AutoSize = False
    Caption = 'Offset'
  end
  object lblLoop: TLabel
    Left = 4
    Top = 176
    Width = 61
    Height = 13
    AutoSize = False
    Caption = 'Loop'
  end
  object bvlSep: TBevel
    Left = 0
    Top = 120
    Width = 69
    Height = 17
    Shape = bsTopLine
  end
  object lstGroup: TListBox
    Left = 72
    Top = 2
    Width = 84
    Height = 212
    IntegralHeight = True
    ItemHeight = 13
    TabOrder = 2
    OnClick = lstGroupClick
  end
  object lstInstr: TListBox
    Left = 159
    Top = 54
    Width = 132
    Height = 160
    IntegralHeight = True
    ItemHeight = 13
    Items.Strings = (
      '')
    TabOrder = 3
  end
  object btnOK: TButton
    Left = 137
    Top = 220
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 4
    OnClick = btnOKClick
  end
  object btnCancel: TButton
    Left = 217
    Top = 220
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 5
  end
  object grpType: TRadioGroup
    Left = 160
    Top = 0
    Width = 130
    Height = 37
    Caption = 'Type'
    Columns = 2
    Items.Strings = (
      'Tone'
      'Drum')
    TabOrder = 1
    OnClick = grpTypeClick
  end
  object txtRate: TEdit
    Left = 4
    Top = 21
    Width = 61
    Height = 21
    MaxLength = 6
    TabOrder = 0
    OnExit = txtRateExit
  end
  object txtOfs: TEdit
    Left = 4
    Top = 148
    Width = 61
    Height = 21
    MaxLength = 4
    TabOrder = 6
    OnExit = txtOfsExit
  end
  object txtLoop: TEdit
    Left = 4
    Top = 192
    Width = 61
    Height = 21
    MaxLength = 5
    TabOrder = 7
    OnExit = txtLoopExit
  end
end
