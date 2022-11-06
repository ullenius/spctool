object FrmMap: TFrmMap
  Left = 449
  Top = 249
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Memory Map'
  ClientHeight = 550
  ClientWidth = 517
  Color = clBtnFace
  Font.Charset = SHIFTJIS_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = '‚l‚r ‚oƒSƒVƒbƒN'
  Font.Style = []
  OldCreateOrder = False
  Position = poDefault
  Scaled = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 12
  object lblMap: TLabel
    Left = 264
    Top = 535
    Width = 81
    Height = 13
    AutoSize = False
  end
  object lblTOfs: TLabel
    Left = 264
    Top = 522
    Width = 41
    Height = 13
    AutoSize = False
    Caption = 'Offset:'
  end
  object lblOfs: TLabel
    Left = 308
    Top = 522
    Width = 33
    Height = 13
    AutoSize = False
    Caption = '0000'
  end
  object pnlMap: TPanel
    Left = 1
    Top = 3
    Width = 514
    Height = 514
    BevelOuter = bvLowered
    ParentColor = True
    TabOrder = 0
    object pbMap: TPaintBox
      Left = 1
      Top = 1
      Width = 512
      Height = 512
      Color = clBlack
      ParentColor = False
      OnMouseMove = pbMapMouseMove
      OnPaint = pbMapPaint
    end
  end
  object btnMap: TButton
    Left = 359
    Top = 520
    Width = 75
    Height = 25
    Caption = '&Map'
    Default = True
    TabOrder = 1
    OnClick = btnMapClick
  end
  object pnlKey: TPanel
    Left = 1
    Top = 520
    Width = 261
    Height = 29
    BevelOuter = bvLowered
    Color = clBlack
    TabOrder = 2
    object lblCode: TLabel
      Left = 2
      Top = 2
      Width = 64
      Height = 13
      AutoSize = False
      Caption = 'Code'
      Color = clBlack
      Font.Charset = SHIFTJIS_CHARSET
      Font.Color = clLime
      Font.Height = -12
      Font.Name = '‚l‚r ‚oƒSƒVƒbƒN'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblData: TLabel
      Left = 2
      Top = 15
      Width = 64
      Height = 13
      AutoSize = False
      Caption = 'Data/Stack'
      Color = clBlack
      Font.Charset = SHIFTJIS_CHARSET
      Font.Color = clBlue
      Font.Height = -12
      Font.Name = '‚l‚r ‚oƒSƒVƒbƒN'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblDir: TLabel
      Left = 66
      Top = 2
      Width = 64
      Height = 13
      AutoSize = False
      Caption = 'Src Dir'
      Color = clBlack
      Font.Charset = SHIFTJIS_CHARSET
      Font.Color = clRed
      Font.Height = -12
      Font.Name = '‚l‚r ‚oƒSƒVƒbƒN'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblSrc: TLabel
      Left = 66
      Top = 15
      Width = 64
      Height = 13
      AutoSize = False
      Caption = 'Src Data'
      Color = clBlack
      Font.Charset = SHIFTJIS_CHARSET
      Font.Color = clFuchsia
      Font.Height = -12
      Font.Name = '‚l‚r ‚oƒSƒVƒbƒN'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblEcho: TLabel
      Left = 130
      Top = 2
      Width = 64
      Height = 13
      AutoSize = False
      Caption = 'Echo'
      Color = clBlack
      Font.Charset = SHIFTJIS_CHARSET
      Font.Color = clYellow
      Font.Height = -12
      Font.Name = '‚l‚r ‚oƒSƒVƒbƒN'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblIPL: TLabel
      Left = 130
      Top = 15
      Width = 64
      Height = 13
      AutoSize = False
      Caption = 'IPL ROM'
      Color = clBlack
      Font.Charset = SHIFTJIS_CHARSET
      Font.Color = clAqua
      Font.Height = -12
      Font.Name = '‚l‚r ‚oƒSƒVƒbƒN'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblFunc: TLabel
      Left = 194
      Top = 2
      Width = 64
      Height = 13
      AutoSize = False
      Caption = 'Func Reg'
      Color = clBlack
      Font.Charset = SHIFTJIS_CHARSET
      Font.Color = clWhite
      Font.Height = -12
      Font.Name = '‚l‚r ‚oƒSƒVƒbƒN'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblFF: TLabel
      Left = 194
      Top = 15
      Width = 64
      Height = 13
      AutoSize = False
      Caption = '!= $FF'
      Color = clBlack
      Font.Charset = SHIFTJIS_CHARSET
      Font.Color = clGray
      Font.Height = -12
      Font.Name = '‚l‚r ‚oƒSƒVƒbƒN'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
  end
  object btnClose: TButton
    Left = 440
    Top = 520
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 3
  end
end
