object FrmRen: TFrmRen
  Left = 561
  Top = 264
  BorderIcons = [biSystemMenu, biHelp]
  BorderStyle = bsDialog
  BorderWidth = 1
  Caption = 'File Renamer'
  ClientHeight = 324
  ClientWidth = 390
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnCreate = FormCreate
  OnHide = FormHide
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object btnRen: TButton
    Left = 233
    Top = 298
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = '&Rename'
    Default = True
    TabOrder = 1
    OnClick = btnRenClick
  end
  object btnClear: TButton
    Left = 1
    Top = 298
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = '&Clear'
    TabOrder = 0
    OnClick = btnClearClick
  end
  object btnCancel: TButton
    Left = 313
    Top = 298
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
  end
  object lvwFile: TListView
    Left = 0
    Top = 1
    Width = 390
    Height = 209
    Anchors = [akLeft, akTop, akRight, akBottom]
    Columns = <
      item
        AutoSize = True
        Caption = 'Original'
      end
      item
        AutoSize = True
        Caption = 'Renamed'
      end>
    ColumnClick = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    HideSelection = False
    ReadOnly = True
    ParentFont = False
    SortType = stText
    TabOrder = 3
    ViewStyle = vsReport
  end
  object grpFormat: TGroupBox
    Left = 0
    Top = 212
    Width = 390
    Height = 81
    Anchors = [akLeft, akRight, akBottom]
    Caption = 'Format String'
    TabOrder = 4
    object lblSong: TLabel
      Left = 8
      Top = 17
      Width = 109
      Height = 16
      Anchors = [akLeft, akBottom]
      AutoSize = False
      Caption = '%S - Song'
    end
    object lblGame: TLabel
      Left = 120
      Top = 17
      Width = 93
      Height = 16
      Anchors = [akLeft, akBottom]
      AutoSize = False
      Caption = '%G - Game'
    end
    object lblArtist: TLabel
      Left = 212
      Top = 17
      Width = 109
      Height = 16
      Anchors = [akLeft, akBottom]
      AutoSize = False
      Caption = '%A - Artist'
    end
    object lblOST: TLabel
      Left = 8
      Top = 33
      Width = 109
      Height = 16
      Anchors = [akLeft, akBottom]
      AutoSize = False
      Caption = '%T - OST Title'
    end
    object lblDisc: TLabel
      Left = 120
      Top = 33
      Width = 93
      Height = 16
      Anchors = [akLeft, akBottom]
      AutoSize = False
      Caption = '%D - Disc'
    end
    object lblTrack: TLabel
      Left = 212
      Top = 33
      Width = 109
      Height = 16
      Anchors = [akLeft, akBottom]
      AutoSize = False
      Caption = '%N - Track Number'
    end
    object txtFormat: TEdit
      Left = 8
      Top = 52
      Width = 298
      Height = 21
      Anchors = [akLeft, akRight, akBottom]
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      MaxLength = 63
      ParentFont = False
      TabOrder = 0
    end
    object btnPre: TButton
      Left = 313
      Top = 52
      Width = 68
      Height = 21
      Anchors = [akRight, akBottom]
      Caption = '&Preview'
      TabOrder = 1
      OnClick = btnPreClick
    end
  end
end
