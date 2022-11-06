object FrmROM: TFrmROM
  Left = 577
  Top = 328
  BorderIcons = [biSystemMenu, biHelp]
  BorderStyle = bsDialog
  BorderWidth = 1
  Caption = 'Create ROM'
  ClientHeight = 215
  ClientWidth = 356
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
  object lvwFile: TListView
    Left = 0
    Top = 0
    Width = 356
    Height = 186
    AllocBy = 63
    Anchors = [akLeft, akTop, akRight, akBottom]
    Columns = <
      item
        AutoSize = True
        Caption = 'Boot'
      end
      item
        AutoSize = True
        Caption = 'File'
      end
      item
        AutoSize = True
        Caption = 'Title'
      end>
    ColumnClick = False
    TabOrder = 0
    ViewStyle = vsReport
    OnEdited = lvwFileEdited
  end
  object chkPad: TCheckBox
    Left = 1
    Top = 193
    Width = 116
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = 'Pad ROM to 2Mb'
    TabOrder = 1
  end
  object btnSave: TButton
    Left = 200
    Top = 189
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = '&Save'
    Default = True
    ModalResult = 1
    TabOrder = 3
    OnClick = btnSaveClick
  end
  object btnCancel: TButton
    Left = 279
    Top = 189
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 4
  end
  object btnUpload: TButton
    Left = 121
    Top = 189
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = '&Upload'
    TabOrder = 2
    OnClick = btnUploadClick
  end
  object dlgSave: TSaveDialog
    Filter = 
      'Super WildCard (*.swc)|*.swc|Super Magicom (*.smc)|*.smc|Multi G' +
      'ame Doctor / Raw Image (*.mgd)|*.mgd'
    Left = 8
    Top = 24
  end
end
