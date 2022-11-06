object FrmProf: TFrmProf
  Left = 470
  Top = 352
  Width = 394
  Height = 310
  Caption = 'Profile'
  Color = clBtnFace
  Font.Charset = SHIFTJIS_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = '‚l‚r ‚oƒSƒVƒbƒN'
  Font.Style = []
  OldCreateOrder = False
  Scaled = False
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 12
  object tabProf: TTabControl
    Left = 4
    Top = 4
    Width = 297
    Height = 274
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 0
    Tabs.Strings = (
      'Instruction'
      'Branch'
      'Function'
      'DSP'
      'CPU Usage')
    TabIndex = 0
    OnChange = tabProfChange
    object lvwProf: TListView
      Left = 4
      Top = 24
      Width = 288
      Height = 243
      Anchors = [akLeft, akTop, akRight, akBottom]
      Columns = <
        item
          Caption = 'Opcode'
        end
        item
          Caption = 'Instruction'
        end
        item
          Alignment = taRightJustify
          Caption = 'Taken'
        end
        item
          Alignment = taRightJustify
          Caption = 'Not Taken'
        end>
      HideSelection = False
      OwnerData = True
      ReadOnly = True
      RowSelect = True
      TabOrder = 0
      ViewStyle = vsReport
      OnColumnClick = lvwProfColumnClick
      OnData = lvwProfData
    end
  end
  object btnReset: TButton
    Left = 307
    Top = 225
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = '&Reset'
    TabOrder = 1
    OnClick = btnResetClick
  end
  object btnResetAll: TButton
    Left = 307
    Top = 253
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'Reset &All'
    TabOrder = 2
    OnClick = btnResetAllClick
  end
end
