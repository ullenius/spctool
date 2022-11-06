object FrmMain: TFrmMain
  Left = 276
  Top = 291
  AutoScroll = False
  BorderIcons = [biSystemMenu, biMinimize, biMaximize, biHelp]
  Caption = 'SPCTool'
  ClientHeight = 554
  ClientWidth = 796
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Icon.Data = {
    0000010001002020100000000000E80200001600000028000000200000004000
    0000010004000000000080020000000000000000000000000000000000000000
    0000000080000080000000808000800000008000800080800000C0C0C0008080
    80000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF007777
    7777777777777777777777772222700777722222222222222222777720027007
    7722222AAAAAAA2222222777200277777222222A22222AAA2222227772227777
    222222AA22AA222AA22222277777777222222AA22AAAA222AA22222277777722
    22222A22A2222A222AA222222777722200000000002A00000000002222777222
    0000000000A2007777770022222772220077777700AA00000000002222277222
    000000000022007777770022222772AA0077777700AA000000000022222772A2
    00000000002222AA222A22AA222772A20000000000222222A222A22A222772A2
    A2A2A2A222222222A2222A2AA22772A2A2A2A2A2222222222AA22A222A2772A2
    A2A2A22A2222222222A22A222A2772A2A22A2A2A2222222222A200000A2772A2
    00000000002E2000022A07070A2772A200000000002E2070022207070A2772AA
    00777770002E2070022207070A27722A00000000002220700222070702277222
    0000000000222000022207070227722200777770002222222222070702277222
    0000000000222222222200000227722200000000002222222222000002277222
    2222222222222202000222222277722222222222222002220702220227777200
    0000000000200022000222227777720000000000002222020222022770077222
    2222222222222222222222777007777777777777777777777777777777770000
    0000600000066000000600000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    000000000000000000000000000000000000000000060000000600000000}
  Menu = mainMenu
  OldCreateOrder = False
  Position = poDefaultPosOnly
  Scaled = False
  ShowHint = True
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnMouseMove = FormMouseMove
  DesignSize = (
    796
    554)
  PixelsPerInch = 96
  TextHeight = 13
  object bvlTag: TBevel
    Left = 0
    Top = 67
    Width = 796
    Height = 3
    Align = alTop
    Shape = bsSpacer
    Style = bsRaised
  end
  object pbPlay: TPaintBox
    Left = 570
    Top = 465
    Width = 24
    Height = 24
    Hint = 'Play'
    Anchors = [akTop, akRight]
    Enabled = False
    OnClick = pbPlayClick
    OnMouseDown = BtnMouseDown
    OnMouseUp = BtnMouseUp
    OnPaint = pbBtnPaint
  end
  object pbStop: TPaintBox
    Left = 598
    Top = 465
    Width = 24
    Height = 24
    Hint = 'Stop'
    Anchors = [akTop, akRight]
    Enabled = False
    OnClick = pbStopClick
    OnMouseDown = BtnMouseDown
    OnMouseUp = BtnMouseUp
    OnPaint = pbBtnPaint
  end
  object pbImport: TPaintBox
    Left = 626
    Top = 465
    Width = 24
    Height = 24
    Hint = 'Import'
    Anchors = [akTop, akRight]
    Enabled = False
    OnClick = itmImportClick
    OnMouseDown = BtnMouseDown
    OnMouseUp = BtnMouseUp
    OnPaint = pbBtnPaint
  end
  object pbExport: TPaintBox
    Left = 654
    Top = 465
    Width = 24
    Height = 24
    Hint = 'Export'
    Anchors = [akTop, akRight]
    Enabled = False
    OnClick = itmExportClick
    OnMouseDown = BtnMouseDown
    OnMouseUp = BtnMouseUp
    OnPaint = pbBtnPaint
  end
  object pbZoom: TPaintBox
    Left = 771
    Top = 465
    Width = 24
    Height = 24
    Hint = 'Zoom'
    Anchors = [akTop, akRight]
    Enabled = False
    OnClick = pbZoomClick
    OnMouseDown = BtnMouseDown
    OnMouseUp = BtnMouseUp
    OnPaint = pbBtnPaint
  end
  object pbReset: TPaintBox
    Left = 398
    Top = 288
    Width = 24
    Height = 24
    Hint = 'Reset'
    Anchors = [akTop, akRight]
    Enabled = False
    OnClick = itmResetClick
    OnMouseDown = BtnMouseDown
    OnMouseUp = BtnMouseUp
    OnPaint = pbBtnPaint
  end
  object pbRun: TPaintBox
    Left = 426
    Top = 288
    Width = 24
    Height = 24
    Hint = 'Run'
    Anchors = [akTop, akRight]
    Enabled = False
    OnClick = itmRunClick
    OnMouseDown = BtnMouseDown
    OnMouseUp = BtnMouseUp
    OnPaint = pbBtnPaint
  end
  object pbHalt: TPaintBox
    Left = 454
    Top = 288
    Width = 24
    Height = 24
    Hint = 'Halt'
    Anchors = [akTop, akRight]
    Enabled = False
    OnClick = itmHaltClick
    OnMouseDown = BtnMouseDown
    OnMouseUp = BtnMouseUp
    OnPaint = pbBtnPaint
  end
  object pbAbort: TPaintBox
    Left = 482
    Top = 288
    Width = 24
    Height = 24
    Hint = 'Stop'
    Anchors = [akTop, akRight]
    Enabled = False
    OnClick = itmAbortClick
    OnMouseDown = BtnMouseDown
    OnMouseUp = BtnMouseUp
    OnPaint = pbBtnPaint
  end
  object pbFF: TPaintBox
    Left = 510
    Top = 288
    Width = 24
    Height = 24
    Hint = 'Fast Forward'
    Anchors = [akTop, akRight]
    Enabled = False
    OnMouseDown = BtnMouseDown
    OnMouseUp = BtnMouseUp
    OnPaint = pbBtnPaint
  end
  object pbOpen: TPaintBox
    Left = 594
    Top = 288
    Width = 24
    Height = 24
    Hint = 'Open'
    Anchors = [akTop, akRight]
    OnClick = itmOpenClick
    OnMouseDown = BtnMouseDown
    OnMouseUp = BtnMouseUp
    OnPaint = pbBtnPaint
  end
  object pbSave: TPaintBox
    Left = 622
    Top = 288
    Width = 24
    Height = 24
    Hint = 'Save'
    Anchors = [akTop, akRight]
    Enabled = False
    OnClick = itmSaveClick
    OnMouseDown = BtnMouseDown
    OnMouseUp = BtnMouseUp
    OnPaint = pbBtnPaint
  end
  object pbWAV: TPaintBox
    Left = 650
    Top = 288
    Width = 24
    Height = 24
    Hint = 'Record DSP'
    Anchors = [akTop, akRight]
    Enabled = False
    OnClick = itmWAVClick
    OnMouseDown = BtnMouseDown
    OnMouseUp = BtnMouseUp
    OnPaint = pbBtnPaint
  end
  object pbTimer: TPaintBox
    Left = 772
    Top = 288
    Width = 24
    Height = 24
    Hint = 'Enable Timer'
    Anchors = [akTop, akRight]
    OnClick = pbTimerClick
    OnMouseDown = BtnMouseDown
    OnMouseUp = BtnMouseUp
    OnPaint = pbBtnPaint
  end
  object bvlSep: TBevel
    Left = 378
    Top = 370
    Width = 418
    Height = 23
    Anchors = [akTop, akRight]
    Shape = bsTopLine
  end
  object lblCLeft: TLabel
    Left = 403
    Top = 70
    Width = 24
    Height = 13
    Alignment = taCenter
    Anchors = [akTop, akRight]
    AutoSize = False
    Caption = 'L'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 8388704
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object lblCRight: TLabel
    Left = 430
    Top = 70
    Width = 24
    Height = 13
    Alignment = taCenter
    Anchors = [akTop, akRight]
    AutoSize = False
    Caption = 'R'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 8388704
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object lblCMode: TLabel
    Left = 644
    Top = 70
    Width = 32
    Height = 13
    Alignment = taCenter
    Anchors = [akTop, akRight]
    AutoSize = False
    Caption = 'Mode'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 8388704
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object lblCEnv: TLabel
    Left = 680
    Top = 70
    Width = 20
    Height = 13
    Alignment = taCenter
    Anchors = [akTop, akRight]
    AutoSize = False
    Caption = 'Env'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 8388704
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object lblCSrc: TLabel
    Left = 700
    Top = 70
    Width = 32
    Height = 13
    Alignment = taCenter
    Anchors = [akTop, akRight]
    AutoSize = False
    Caption = 'Src'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 8388704
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object lblCRate: TLabel
    Left = 728
    Top = 70
    Width = 40
    Height = 13
    Alignment = taCenter
    Anchors = [akTop, akRight]
    AutoSize = False
    Caption = 'Rate'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 8388704
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object lblCSw: TLabel
    Left = 770
    Top = 70
    Width = 20
    Height = 13
    Alignment = taCenter
    Anchors = [akTop, akRight]
    AutoSize = False
    Caption = 'Sw'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 8388704
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object bvlTime: TBevel
    Left = 398
    Top = 265
    Width = 398
    Height = 12
    Anchors = [akTop, akRight]
  end
  object lblIn: TLabel
    Left = 658
    Top = 320
    Width = 28
    Height = 17
    Alignment = taCenter
    Anchors = [akTop, akRight]
    AutoSize = False
    Caption = 'In'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 8388704
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblOut: TLabel
    Left = 658
    Top = 344
    Width = 28
    Height = 17
    Alignment = taCenter
    Anchors = [akTop, akRight]
    AutoSize = False
    Caption = 'Out'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 8388704
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblSpeed: TLabel
    Left = 600
    Top = 320
    Width = 37
    Height = 17
    Anchors = [akTop, akRight]
    AutoSize = False
    Caption = '1.00x'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 8388704
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblPitch: TLabel
    Left = 600
    Top = 344
    Width = 37
    Height = 17
    Anchors = [akTop, akRight]
    AutoSize = False
    Caption = '0.00'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 8388704
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object Image1: TImage
    Left = 369
    Top = 322
    Width = 32
    Height = 12
    Anchors = [akTop, akRight]
    Picture.Data = {
      07544269746D6170B6040000424DB60400000000000036000000280000002000
      00000C000000010018000000000080040000C30E0000C30E0000000000000000
      0000FF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF
      FF00FF0000000000000000000000000000000000000000000000000000000000
      00000000FF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
      00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF
      FF00FF0000008080808080808080808080808080808080808080808080808080
      80000000FF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
      00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF
      FF00FF000000BFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBF8080
      80000000FF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
      00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF
      FF00FF0000000000000000000000000000000000000000000000000000000000
      00000000FF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
      00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF
      FF00FF808080000000BFBFBFBFBFBFBFBFBF000000808080BFBFBFBFBFBF0000
      00808080FF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
      00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF
      FF00FFFF00FF808080000000BFBFBFBFBFBF8080800000008080800000008080
      80FF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
      00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF
      FF00FFFF00FFFF00FF000000BFBFBFBFBFBFBFBFBF808080000000808080FF00
      FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
      00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF
      FF00FFFF00FFFF00FF808080000000BFBFBFBFBFBFBFBFBF8080800000008080
      80FF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
      00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF
      FF00FFFF00FFFF00FFFF00FF000000BFBFBFBFBFBFBFBFBF0000008080800000
      00808080FF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
      00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF
      FF00FFFF00FFFF00FFFF00FF808080000000BFBFBF000000808080FF00FF8080
      80000000FF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
      00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF
      FF00FFFF00FFFF00FFFF00FFFF00FF000000BFBFBF000000FF00FFFF00FFFF00
      FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
      00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF
      FF00FFFF00FFFF00FFFF00FFFF00FF808080000000808080FF00FFFF00FFFF00
      FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
      00FF}
    Transparent = True
  end
  object Image2: TImage
    Left = 369
    Top = 346
    Width = 32
    Height = 12
    Anchors = [akTop, akRight]
    Picture.Data = {
      07544269746D6170B6040000424DB60400000000000036000000280000002000
      00000C000000010018000000000080040000C30E0000C30E0000000000000000
      0000FF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF
      FF00FFFF00FF000000000000000000FF00FFFF00FFFF00FFFF00FFFF00FFFF00
      FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
      00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF
      FF00FF000000000000000000000000000000FF00FFFF00FFFF00FFFF00FFFF00
      FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
      00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF
      FF00FF000000FFFFFFBFBFBF000000000000FF00FFFF00FFFF00FFFF00FFFF00
      FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
      00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF
      FF00FFFF00FF000000000000000000000000FF00FFFF00FFFF00FFFF00FFFF00
      FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
      00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF
      FF00FFFF00FFFF00FFFF00FFFF00FF000000FF00FFFF00FFFF00FFFF00FFFF00
      FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
      00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF
      FF00FFFF00FFFF00FFFF00FFFF00FF000000FF00FFFF00FFFF00FFFF00FFFF00
      FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
      00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF
      FF00FFFF00FFFF00FFFF00FFFF00FF000000FF00FFFF00FFFF00FFFF00FFFF00
      FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
      00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF
      FF00FFFF00FFFF00FFFF00FFFF00FF000000FF00FFFF00FFFF00FFFF00FFFF00
      FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
      00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF
      FF00FFFF00FFFF00FFFF00FFFF00FF0000008080800000000000000000008080
      80808080FF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
      00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF
      FF00FFFF00FFFF00FFFF00FFFF00FF0000000000000000000000000000000000
      00000000808080FF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
      00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF
      FF00FFFF00FFFF00FFFF00FFFF00FF000000000000000000808080FF00FFFF00
      FF808080000000808080FF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
      00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF
      FF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00
      FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
      00FF}
    Transparent = True
  end
  object pbTime: TPaintBox
    Left = 399
    Top = 266
    Width = 395
    Height = 10
    Anchors = [akTop, akRight]
    OnMouseDown = pbTimeMouseDown
    OnMouseMove = pbTimeMouseMove
    OnPaint = pbTimePaint
  end
  object pbUpload: TPaintBox
    Left = 538
    Top = 288
    Width = 24
    Height = 24
    Hint = 'Run'
    Anchors = [akTop, akRight]
    Enabled = False
    Visible = False
    OnClick = pbUploadClick
    OnMouseDown = BtnMouseDown
    OnMouseUp = BtnMouseUp
    OnPaint = pbBtnPaint
  end
  object pbMIDI: TPaintBox
    Left = 678
    Top = 288
    Width = 24
    Height = 24
    Hint = 'Record DSP'
    Anchors = [akTop, akRight]
    Enabled = False
    OnClick = itmMIDIClick
    OnMouseDown = BtnMouseDown
    OnMouseUp = BtnMouseUp
    OnPaint = pbBtnPaint
  end
  object pnlTag: TPanel
    Left = 0
    Top = 0
    Width = 796
    Height = 67
    Align = alTop
    BevelInner = bvLowered
    BevelOuter = bvNone
    Color = clBlack
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clSilver
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    OnResize = pnlTagResize
    object lblSongT: TLabel
      Left = 4
      Top = 19
      Width = 36
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Song  '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object lblGameT: TLabel
      Left = 332
      Top = 4
      Width = 52
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Game  '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object lblArtistT: TLabel
      Left = 4
      Top = 34
      Width = 36
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Artist  '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object lblPubT: TLabel
      Left = 332
      Top = 19
      Width = 52
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Publisher  '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object lblOSTT: TLabel
      Left = 4
      Top = 4
      Width = 36
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'OST  '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object lblDiscT: TLabel
      Left = 4
      Top = 49
      Width = 36
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Disc  '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      Visible = False
    end
    object lblTrackT: TLabel
      Left = 44
      Top = 49
      Width = 40
      Height = 13
      Caption = '  Track  '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      Visible = False
    end
    object lblDumperT: TLabel
      Left = 324
      Top = 49
      Width = 60
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Dumped by  '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      Visible = False
    end
    object lblEmuT: TLabel
      Left = 428
      Top = 49
      Width = 31
      Height = 13
      Caption = '  with  '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      Visible = False
    end
    object lblDateT: TLabel
      Left = 393
      Top = 49
      Width = 24
      Height = 13
      Alignment = taRightJustify
      Caption = '  on  '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      Visible = False
    end
    object lblCmntT: TLabel
      Left = 592
      Top = 4
      Width = 54
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Comments  '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object lblSong: TLabel
      Left = 40
      Top = 19
      Width = 289
      Height = 14
      AutoSize = False
      Color = clBlack
      ParentColor = False
    end
    object lblGame: TLabel
      Left = 384
      Top = 4
      Width = 201
      Height = 14
      AutoSize = False
      Color = clBlack
      ParentColor = False
    end
    object lblArtist: TLabel
      Left = 40
      Top = 34
      Width = 289
      Height = 14
      AutoSize = False
      Color = clBlack
      ParentColor = False
    end
    object lblPub: TLabel
      Left = 404
      Top = 19
      Width = 181
      Height = 14
      AutoSize = False
      Color = clBlack
      ParentColor = False
    end
    object lblOST: TLabel
      Left = 40
      Top = 4
      Width = 289
      Height = 14
      AutoSize = False
      Color = clBlack
      ParentColor = False
    end
    object lblDisc: TLabel
      Left = 40
      Top = 49
      Width = 3
      Height = 13
      Color = clBlack
      ParentColor = False
    end
    object lblTrack: TLabel
      Left = 87
      Top = 49
      Width = 3
      Height = 13
      Caption = ' '
      Color = clBlack
      ParentColor = False
    end
    object lblDumper: TLabel
      Left = 384
      Top = 49
      Width = 3
      Height = 13
      Color = clBlack
      ParentColor = False
    end
    object lblDate: TLabel
      Left = 420
      Top = 49
      Width = 3
      Height = 13
      Color = clBlack
      ParentColor = False
    end
    object lblEmu: TLabel
      Left = 464
      Top = 49
      Width = 3
      Height = 13
      Color = clBlack
      ParentColor = False
    end
    object lblCopy: TLabel
      Left = 384
      Top = 19
      Width = 12
      Height = 13
      Caption = #169' '
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      Visible = False
    end
    object mmoCmnts: TMemo
      Left = 592
      Top = 20
      Width = 200
      Height = 41
      TabStop = False
      BorderStyle = bsNone
      Color = clBlack
      MaxLength = 255
      ReadOnly = True
      TabOrder = 0
      WantReturns = False
    end
  end
  object lvwFile: TListView
    Left = 0
    Top = 70
    Width = 132
    Height = 484
    Align = alLeft
    Anchors = [akLeft, akTop, akRight, akBottom]
    Color = clBlack
    Columns = <
      item
        Caption = 'Files'
        Tag = 70
        Width = 132
      end>
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clSilver
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    HideSelection = False
    MultiSelect = True
    RowSelect = True
    ParentFont = False
    TabOrder = 1
    ViewStyle = vsReport
    OnColumnClick = lvwFileColumnClick
    OnDblClick = itmEditClick
    OnEdited = lvwFileEdited
    OnEditing = lvwFileEditing
    OnResize = lvwFileResize
    OnSelectItem = lvwFileSelectItem
  end
  object pnlSrc: TPanel
    Left = 378
    Top = 464
    Width = 188
    Height = 69
    Anchors = [akTop, akRight]
    BevelOuter = bvLowered
    Color = clBlack
    UseDockManager = False
    FullRepaint = False
    TabOrder = 2
    OnClick = itmSetOptClick
    object lblTRate: TLabel
      Left = 4
      Top = 4
      Width = 45
      Height = 13
      AutoSize = False
      Caption = 'Rate'
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      OnClick = itmSetOptClick
    end
    object lblTType: TLabel
      Left = 88
      Top = 36
      Width = 45
      Height = 13
      AutoSize = False
      Caption = 'Type'
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      OnClick = itmSetOptClick
    end
    object lblTProg: TLabel
      Left = 4
      Top = 20
      Width = 45
      Height = 13
      AutoSize = False
      Caption = 'Program'
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      OnClick = itmSetOptClick
    end
    object lblType: TLabel
      Left = 132
      Top = 36
      Width = 52
      Height = 13
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      OnClick = itmSetOptClick
    end
    object lblRate: TLabel
      Left = 48
      Top = 4
      Width = 77
      Height = 13
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      OnClick = itmSetOptClick
    end
    object lblProg: TLabel
      Left = 48
      Top = 20
      Width = 136
      Height = 13
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      OnClick = itmSetOptClick
    end
    object lblTBank: TLabel
      Left = 4
      Top = 36
      Width = 45
      Height = 13
      AutoSize = False
      Caption = 'Bank'
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      OnClick = itmSetOptClick
    end
    object lblBank: TLabel
      Left = 48
      Top = 36
      Width = 36
      Height = 13
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      OnClick = itmSetOptClick
    end
    object lblTADSR: TLabel
      Left = 4
      Top = 52
      Width = 45
      Height = 13
      AutoSize = False
      Caption = 'ADSR'
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      OnClick = itmSetOptClick
    end
    object lblADSR: TLabel
      Left = 48
      Top = 52
      Width = 136
      Height = 13
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      OnClick = itmSetOptClick
    end
    object lblTCRC: TLabel
      Left = 124
      Top = 4
      Width = 28
      Height = 13
      AutoSize = False
      Caption = 'CRC'
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      OnClick = itmSetOptClick
    end
    object lblCRC: TLabel
      Left = 152
      Top = 4
      Width = 32
      Height = 13
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      OnClick = itmSetOptClick
    end
  end
  object trkAmp: TTrackBar
    Left = 379
    Top = 81
    Width = 14
    Height = 185
    Hint = 'Mixing Level'
    Anchors = [akTop, akRight]
    Max = 60
    Min = -180
    Orientation = trVertical
    PageSize = 10
    Frequency = 5
    Position = 60
    SelEnd = 0
    SelStart = 0
    TabOrder = 3
    TabStop = False
    ThumbLength = 11
    TickMarks = tmBoth
    TickStyle = tsNone
    OnChange = trkAmpChange
  end
  object txtIn0: TEdit
    Left = 688
    Top = 320
    Width = 24
    Height = 20
    Hint = 'Port 0'
    TabStop = False
    Anchors = [akTop, akRight]
    AutoSize = False
    MaxLength = 2
    TabOrder = 4
    OnChange = txtInChange
  end
  object txtIn1: TEdit
    Tag = 1
    Left = 716
    Top = 320
    Width = 24
    Height = 20
    Hint = 'Port 1'
    TabStop = False
    Anchors = [akTop, akRight]
    AutoSize = False
    MaxLength = 2
    TabOrder = 5
    OnChange = txtInChange
  end
  object txtIn2: TEdit
    Tag = 2
    Left = 744
    Top = 320
    Width = 24
    Height = 20
    Hint = 'Port 2'
    TabStop = False
    Anchors = [akTop, akRight]
    AutoSize = False
    MaxLength = 2
    TabOrder = 6
    OnChange = txtInChange
  end
  object txtIn3: TEdit
    Tag = 3
    Left = 772
    Top = 320
    Width = 24
    Height = 20
    Hint = 'Port 3'
    TabStop = False
    Anchors = [akTop, akRight]
    AutoSize = False
    MaxLength = 2
    TabOrder = 7
    OnChange = txtInChange
  end
  object txtOut0: TEdit
    Left = 688
    Top = 344
    Width = 24
    Height = 20
    Hint = 'Port 0'
    TabStop = False
    Anchors = [akTop, akRight]
    AutoSize = False
    Color = clBtnFace
    ReadOnly = True
    TabOrder = 8
  end
  object txtOut1: TEdit
    Left = 716
    Top = 344
    Width = 24
    Height = 20
    Hint = 'Port 1'
    TabStop = False
    Anchors = [akTop, akRight]
    AutoSize = False
    Color = clBtnFace
    ReadOnly = True
    TabOrder = 9
  end
  object txtOut2: TEdit
    Left = 744
    Top = 344
    Width = 24
    Height = 20
    Hint = 'Port 2'
    TabStop = False
    Anchors = [akTop, akRight]
    AutoSize = False
    Color = clBtnFace
    ReadOnly = True
    TabOrder = 10
  end
  object txtOut3: TEdit
    Left = 772
    Top = 344
    Width = 24
    Height = 20
    Hint = 'Port 3'
    TabStop = False
    Anchors = [akTop, akRight]
    AutoSize = False
    Color = clBtnFace
    ReadOnly = True
    TabOrder = 11
  end
  object trkSpeed: TTrackBar
    Left = 393
    Top = 322
    Width = 202
    Height = 16
    Hint = 'Speed'
    Anchors = [akTop, akRight]
    Max = 48
    Min = -48
    Orientation = trHorizontal
    PageSize = 4
    Frequency = 4
    Position = 0
    SelEnd = 0
    SelStart = 0
    TabOrder = 12
    TabStop = False
    ThumbLength = 11
    TickMarks = tmBoth
    TickStyle = tsNone
    OnChange = trkSpeedChange
  end
  object trkPitch: TTrackBar
    Left = 393
    Top = 346
    Width = 202
    Height = 16
    Hint = 'Pitch'
    Anchors = [akTop, akRight]
    Max = 48
    Min = -48
    Orientation = trHorizontal
    PageSize = 4
    Frequency = 4
    Position = 0
    SelEnd = 0
    SelStart = 0
    TabOrder = 13
    TabStop = False
    ThumbLength = 11
    TickMarks = tmBoth
    TickStyle = tsNone
    OnChange = trkPitchChange
  end
  object pgcSrc: TPageControl
    Left = 135
    Top = 70
    Width = 240
    Height = 483
    ActivePage = tabSrc
    Anchors = [akTop, akRight, akBottom]
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 8388704
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabIndex = 0
    TabOrder = 14
    OnChange = pgcSrcChange
    object tabSrc: TTabSheet
      Caption = 'Sound Source Directory'
      object lvwSrc: TListView
        Left = 0
        Top = 0
        Width = 232
        Height = 455
        Align = alClient
        AllocBy = 256
        Color = clBlack
        Columns = <
          item
            Caption = 'Idx'
            Width = 32
          end
          item
            Alignment = taRightJustify
            Caption = 'Offset'
            Width = 56
          end
          item
            Caption = 'Len'
            Width = 42
          end
          item
            Caption = 'Loop'
            Width = 42
          end
          item
            Caption = 'Filters'
            Width = 40
          end>
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clSilver
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        HideSelection = False
        IconOptions.WrapText = False
        MultiSelect = True
        ReadOnly = True
        RowSelect = True
        ParentFont = False
        PopupMenu = popSrc
        TabOrder = 0
        ViewStyle = vsReport
        OnColumnClick = lvwSrcColumnClick
        OnCustomDrawItem = lvwSrcCustomDrawItem
        OnDblClick = lvwSrcDblClick
        OnExit = lvwSrcExit
        OnSelectItem = lvwSrcSelectItem
      end
    end
    object tabInfo: TTabSheet
      Caption = 'Information'
      ImageIndex = 1
      object lvwInfo: TListView
        Left = 0
        Top = 0
        Width = 232
        Height = 455
        Align = alClient
        Color = clBlack
        Columns = <
          item
            Caption = 'CRC'
            Width = 36
          end
          item
            Caption = 'Len'
            Width = 42
          end
          item
            Caption = 'Loop'
            Width = 42
          end
          item
            Caption = 'Rate'
            Width = 48
          end
          item
            Caption = 'Prog'
            Width = 44
          end>
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clSilver
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ReadOnly = True
        RowSelect = True
        ParentFont = False
        TabOrder = 0
        ViewStyle = vsReport
      end
    end
  end
  object pnlMeter: TPanel
    Left = 462
    Top = 86
    Width = 176
    Height = 175
    Anchors = [akTop, akRight]
    BevelOuter = bvLowered
    Color = clBlack
    TabOrder = 15
    DesignSize = (
      176
      175)
    object pbMeter: TPaintBox
      Left = 21
      Top = 4
      Width = 152
      Height = 166
      Color = clBlack
      ParentColor = False
      OnPaint = pbMeterPaint
    end
    object lblVoice0: TLabel
      Left = 2
      Top = 4
      Width = 16
      Height = 16
      Alignment = taCenter
      Anchors = [akTop, akRight]
      AutoSize = False
      Caption = '0'
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      Layout = tlCenter
      OnMouseDown = lblVoiceMouseDown
    end
    object lblVoice1: TLabel
      Tag = 1
      Left = 2
      Top = 20
      Width = 16
      Height = 16
      Alignment = taCenter
      Anchors = [akTop, akRight]
      AutoSize = False
      Caption = '1'
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      Layout = tlCenter
      OnMouseDown = lblVoiceMouseDown
    end
    object lblVoice2: TLabel
      Tag = 2
      Left = 2
      Top = 36
      Width = 16
      Height = 16
      Alignment = taCenter
      Anchors = [akTop, akRight]
      AutoSize = False
      Caption = '2'
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      Layout = tlCenter
      OnMouseDown = lblVoiceMouseDown
    end
    object lblVoice3: TLabel
      Tag = 3
      Left = 2
      Top = 52
      Width = 16
      Height = 16
      Alignment = taCenter
      Anchors = [akTop, akRight]
      AutoSize = False
      Caption = '3'
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      Layout = tlCenter
      OnMouseDown = lblVoiceMouseDown
    end
    object lblVoice4: TLabel
      Tag = 4
      Left = 2
      Top = 68
      Width = 16
      Height = 20
      Alignment = taCenter
      Anchors = [akTop, akRight]
      AutoSize = False
      Caption = '4'
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      Layout = tlCenter
      OnMouseDown = lblVoiceMouseDown
    end
    object lblVoice5: TLabel
      Tag = 5
      Left = 2
      Top = 84
      Width = 16
      Height = 16
      Alignment = taCenter
      Anchors = [akTop, akRight]
      AutoSize = False
      Caption = '5'
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      Layout = tlCenter
      OnMouseDown = lblVoiceMouseDown
    end
    object lblVoice6: TLabel
      Tag = 6
      Left = 2
      Top = 100
      Width = 16
      Height = 16
      Alignment = taCenter
      Anchors = [akTop, akRight]
      AutoSize = False
      Caption = '6'
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      Layout = tlCenter
      OnMouseDown = lblVoiceMouseDown
    end
    object lblVoice7: TLabel
      Tag = 7
      Left = 2
      Top = 116
      Width = 16
      Height = 16
      Alignment = taCenter
      Anchors = [akTop, akRight]
      AutoSize = False
      Caption = '7'
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      Layout = tlCenter
      OnMouseDown = lblVoiceMouseDown
    end
    object lblChnL: TLabel
      Left = 2
      Top = 140
      Width = 16
      Height = 13
      Alignment = taCenter
      Anchors = [akTop, akRight]
      AutoSize = False
      Caption = 'L'
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblChnR: TLabel
      Left = 2
      Top = 156
      Width = 16
      Height = 13
      Alignment = taCenter
      Anchors = [akTop, akRight]
      AutoSize = False
      Caption = 'R'
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
  end
  object pnlWave: TPanel
    Left = 378
    Top = 377
    Width = 418
    Height = 83
    Anchors = [akTop, akRight]
    BevelOuter = bvLowered
    Color = clBlack
    TabOrder = 16
    DesignSize = (
      418
      83)
    object pbWave: TPaintBox
      Left = 1
      Top = 1
      Width = 416
      Height = 64
      OnMouseDown = pbWaveMouseDown
      OnPaint = pbWavePaint
    end
    object barWave: TScrollBar
      Left = 1
      Top = 66
      Width = 416
      Height = 16
      Anchors = [akTop, akRight]
      Enabled = False
      LargeChange = 25
      PageSize = 26
      TabOrder = 0
      OnChange = barWaveChange
    end
  end
  object pnlVol: TPanel
    Left = 398
    Top = 86
    Width = 60
    Height = 175
    Anchors = [akTop, akRight]
    BevelOuter = bvLowered
    Color = clBlack
    TabOrder = 17
    object lblVolL0: TLabel
      Left = 4
      Top = 4
      Width = 22
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblVolR0: TLabel
      Left = 30
      Top = 4
      Width = 22
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblVolL1: TLabel
      Left = 4
      Top = 20
      Width = 22
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblVolR1: TLabel
      Left = 30
      Top = 20
      Width = 22
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblVolL2: TLabel
      Left = 4
      Top = 36
      Width = 22
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblVolR2: TLabel
      Left = 30
      Top = 36
      Width = 22
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblVolL3: TLabel
      Left = 4
      Top = 52
      Width = 22
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblVolR3: TLabel
      Left = 30
      Top = 52
      Width = 22
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblVolL4: TLabel
      Left = 4
      Top = 68
      Width = 22
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblVolR4: TLabel
      Left = 30
      Top = 68
      Width = 22
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblVolL5: TLabel
      Left = 4
      Top = 84
      Width = 22
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblVolR5: TLabel
      Left = 30
      Top = 84
      Width = 22
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblVolL6: TLabel
      Left = 4
      Top = 100
      Width = 22
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblVolR6: TLabel
      Left = 30
      Top = 100
      Width = 22
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblVolL7: TLabel
      Left = 4
      Top = 116
      Width = 22
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblVolR7: TLabel
      Left = 30
      Top = 116
      Width = 22
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblMVolL: TLabel
      Left = 4
      Top = 140
      Width = 22
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblEVolL: TLabel
      Left = 30
      Top = 140
      Width = 22
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblMVolR: TLabel
      Left = 4
      Top = 156
      Width = 22
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblEVolR: TLabel
      Left = 30
      Top = 156
      Width = 22
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
  end
  object pnlVoice: TPanel
    Left = 642
    Top = 86
    Width = 154
    Height = 175
    Anchors = [akTop, akRight]
    BevelOuter = bvLowered
    Color = clBlack
    TabOrder = 18
    object lblTotal: TLabel
      Left = 146
      Top = 156
      Width = 3
      Height = 13
      Alignment = taRightJustify
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblTime: TLabel
      Left = 4
      Top = 156
      Width = 64
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      OnClick = lblTimeClick
    end
    object lblAmpT: TLabel
      Left = 4
      Top = 140
      Width = 41
      Height = 13
      AutoSize = False
      Caption = 'Amp'
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblAmp: TLabel
      Left = 44
      Top = 140
      Width = 33
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblTimeSet: TLabel
      Left = 86
      Top = 140
      Width = 64
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblSw7: TLabel
      Left = 128
      Top = 117
      Width = 24
      Height = 13
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = 11
      Font.Name = 'Lucida Console'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblRate7: TLabel
      Left = 88
      Top = 116
      Width = 36
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblSrc7: TLabel
      Left = 64
      Top = 116
      Width = 18
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblEnv7: TLabel
      Left = 40
      Top = 116
      Width = 16
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblMode7: TLabel
      Left = 4
      Top = 116
      Width = 32
      Height = 13
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblMode6: TLabel
      Left = 4
      Top = 100
      Width = 32
      Height = 13
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblEnv6: TLabel
      Left = 40
      Top = 100
      Width = 16
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblSrc6: TLabel
      Left = 64
      Top = 100
      Width = 18
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblRate6: TLabel
      Left = 88
      Top = 100
      Width = 36
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblSw6: TLabel
      Left = 128
      Top = 101
      Width = 24
      Height = 13
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = 11
      Font.Name = 'Lucida Console'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblSw5: TLabel
      Left = 128
      Top = 85
      Width = 24
      Height = 13
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = 11
      Font.Name = 'Lucida Console'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblRate5: TLabel
      Left = 88
      Top = 84
      Width = 36
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblSrc5: TLabel
      Left = 64
      Top = 84
      Width = 18
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblEnv5: TLabel
      Left = 40
      Top = 84
      Width = 16
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblMode5: TLabel
      Left = 4
      Top = 84
      Width = 32
      Height = 13
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblMode4: TLabel
      Left = 4
      Top = 68
      Width = 32
      Height = 13
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblEnv4: TLabel
      Left = 40
      Top = 68
      Width = 16
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblSrc4: TLabel
      Left = 64
      Top = 68
      Width = 18
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblRate4: TLabel
      Left = 88
      Top = 68
      Width = 36
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblSw4: TLabel
      Left = 128
      Top = 69
      Width = 24
      Height = 13
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = 11
      Font.Name = 'Lucida Console'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblSw3: TLabel
      Left = 128
      Top = 53
      Width = 24
      Height = 13
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = 11
      Font.Name = 'Lucida Console'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblRate3: TLabel
      Left = 88
      Top = 52
      Width = 36
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblSrc3: TLabel
      Left = 64
      Top = 52
      Width = 18
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblEnv3: TLabel
      Left = 40
      Top = 52
      Width = 16
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblMode3: TLabel
      Left = 4
      Top = 52
      Width = 32
      Height = 13
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblMode2: TLabel
      Left = 4
      Top = 36
      Width = 32
      Height = 13
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblEnv2: TLabel
      Left = 40
      Top = 36
      Width = 16
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblSrc2: TLabel
      Left = 64
      Top = 36
      Width = 18
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblRate2: TLabel
      Left = 88
      Top = 36
      Width = 36
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblSw2: TLabel
      Left = 128
      Top = 37
      Width = 24
      Height = 13
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = 11
      Font.Name = 'Lucida Console'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblSw1: TLabel
      Left = 128
      Top = 21
      Width = 24
      Height = 13
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = 11
      Font.Name = 'Lucida Console'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblRate1: TLabel
      Left = 88
      Top = 20
      Width = 36
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblSrc1: TLabel
      Left = 64
      Top = 20
      Width = 18
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblEnv1: TLabel
      Left = 40
      Top = 20
      Width = 16
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblMode1: TLabel
      Left = 4
      Top = 20
      Width = 32
      Height = 13
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblMode0: TLabel
      Left = 4
      Top = 4
      Width = 32
      Height = 13
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblEnv0: TLabel
      Left = 40
      Top = 4
      Width = 16
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblSrc0: TLabel
      Left = 64
      Top = 4
      Width = 18
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblRate0: TLabel
      Left = 88
      Top = 4
      Width = 36
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
    object lblSw0: TLabel
      Left = 128
      Top = 5
      Width = 24
      Height = 13
      AutoSize = False
      Color = clBlack
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clSilver
      Font.Height = 11
      Font.Name = 'Lucida Console'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentColor = False
      ParentFont = False
    end
  end
  object mainMenu: TMainMenu
    Left = 4
    Top = 4
    object mnuFile: TMenuItem
      Caption = '&File'
      object itmOpen: TMenuItem
        Caption = '&Open...'
        ShortCut = 16463
        OnClick = itmOpenClick
      end
      object itmClose: TMenuItem
        Caption = '&Close'
        Enabled = False
        OnClick = itmCloseClick
      end
      object itmCloseA: TMenuItem
        Caption = 'Close All'
        Enabled = False
        OnClick = itmCloseAClick
      end
      object sepF1: TMenuItem
        Caption = '-'
      end
      object itmSave: TMenuItem
        Caption = '&Save'
        Enabled = False
        ShortCut = 16467
        OnClick = itmSaveClick
      end
      object itmSaveAs: TMenuItem
        Caption = 'Save &As...'
        Enabled = False
        OnClick = itmSaveAsClick
      end
      object itmSaveA: TMenuItem
        Caption = 'Save All'
        Enabled = False
        ShortCut = 24641
        OnClick = itmSaveAClick
      end
      object sepF2: TMenuItem
        Caption = '-'
      end
      object itmFile1: TMenuItem
        Caption = '&1'
        Visible = False
        OnClick = itmFileClick
      end
      object itmFile2: TMenuItem
        Tag = 1
        Caption = '&2'
        Visible = False
        OnClick = itmFileClick
      end
      object itmFile3: TMenuItem
        Tag = 2
        Caption = '&3'
        Visible = False
        OnClick = itmFileClick
      end
      object itmFile4: TMenuItem
        Tag = 3
        Caption = '&4'
        Visible = False
        OnClick = itmFileClick
      end
      object itmFile5: TMenuItem
        Tag = 4
        Caption = '&5'
        Visible = False
        OnClick = itmFileClick
      end
      object itmFile6: TMenuItem
        Tag = 5
        Caption = '&6'
        Visible = False
        OnClick = itmFileClick
      end
      object itmFile7: TMenuItem
        Tag = 6
        Caption = '&7'
        Visible = False
        OnClick = itmFileClick
      end
      object itmFile8: TMenuItem
        Tag = 7
        Caption = '&8'
        Visible = False
        OnClick = itmFileClick
      end
      object itmFile9: TMenuItem
        Tag = 8
        Caption = '&9'
        Visible = False
        OnClick = itmFileClick
      end
      object sepF3: TMenuItem
        Caption = '-'
        Visible = False
      end
      object itmExit: TMenuItem
        Caption = 'E&xit'
        OnClick = itmExitClick
      end
    end
    object mnuEmu: TMenuItem
      Caption = '&Emulation'
      object itmRun: TMenuItem
        Caption = 'Run'
        Enabled = False
        ShortCut = 120
        OnClick = itmRunClick
      end
      object itmHalt: TMenuItem
        Caption = 'Halt'
        Enabled = False
        ShortCut = 121
        OnClick = itmHaltClick
      end
      object itmReset: TMenuItem
        Caption = 'Reset'
        Enabled = False
        ShortCut = 8312
        OnClick = itmResetClick
      end
      object itmAbort: TMenuItem
        Caption = 'Stop'
        Enabled = False
        ShortCut = 123
        OnClick = itmAbortClick
      end
      object sepE1: TMenuItem
        Caption = '-'
      end
      object itmTimer: TMenuItem
        Caption = 'Use Timer'
        ShortCut = 122
        OnClick = itmTimerClick
      end
      object sepE2: TMenuItem
        Caption = '-'
      end
      object itmDebug: TMenuItem
        Caption = 'Debug...'
        Enabled = False
        ShortCut = 118
        OnClick = itmDebugClick
      end
      object itmDSP: TMenuItem
        Caption = 'DSP Editor...'
        Enabled = False
        ShortCut = 119
        OnClick = itmDSPClick
      end
      object itmProf: TMenuItem
        Caption = '&Profile...'
        OnClick = itmProfClick
      end
    end
    object mnuSource: TMenuItem
      Caption = '&Source'
      object itmImport: TMenuItem
        Caption = '&Import...'
        Enabled = False
        OnClick = itmImportClick
      end
      object itmExport: TMenuItem
        Caption = 'E&xport...'
        Enabled = False
        OnClick = itmExportClick
      end
      object itmMute: TMenuItem
        Caption = 'Mute'
        Enabled = False
        ShortCut = 115
        OnClick = itmMuteClick
      end
      object itmSilence: TMenuItem
        Caption = 'Silence'
        Enabled = False
        ShortCut = 8307
        OnClick = itmSilenceClick
      end
      object sepS1: TMenuItem
        Caption = '-'
      end
      object itmSetOpt: TMenuItem
        Caption = 'Set &Options...'
        Enabled = False
        OnClick = itmSetOptClick
      end
      object itmLoadInfo: TMenuItem
        Caption = '&Load Information...'
        OnClick = itmLoadInfoClick
      end
      object itmSaveInfo: TMenuItem
        Caption = '&Save Information...'
        OnClick = itmSaveInfoClick
      end
      object sepS2: TMenuItem
        Caption = '-'
      end
      object itmRescan: TMenuItem
        Caption = 'Rescan...'
        ShortCut = 16466
      end
    end
    object mnuTools: TMenuItem
      Caption = '&Tools'
      object itmEdit: TMenuItem
        Caption = 'Tag &Editor...'
        Enabled = False
        ShortCut = 114
        OnClick = itmEditClick
      end
      object itmWAV: TMenuItem
        Caption = 'Record to &WAV...'
        Enabled = False
        OnClick = itmWAVClick
      end
      object itmMIDI: TMenuItem
        Caption = 'Log to &MIDI...'
        Enabled = False
        OnClick = itmMIDIClick
      end
      object itmROM: TMenuItem
        Caption = 'Create &ROM...'
        Enabled = False
        OnClick = itmROMClick
      end
      object itmRename: TMenuItem
        Caption = 'File Re&namer...'
        OnClick = itmRenameClick
      end
      object sepT1: TMenuItem
        Caption = '-'
      end
      object itmConfig: TMenuItem
        Caption = '&Preferences...'
        ShortCut = 16464
        OnClick = itmConfigClick
      end
    end
    object mnuHelp: TMenuItem
      Caption = '&Help'
      object itmHelp: TMenuItem
        Caption = '&Help Topics'
        OnClick = itmHelpClick
      end
      object sepH1: TMenuItem
        Caption = '-'
      end
      object itmAbout: TMenuItem
        Caption = '&About...'
        OnClick = itmAboutClick
      end
    end
  end
  object dlgSave: TSaveDialog
    Options = [ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Left = 100
    Top = 92
  end
  object dlgImport: TOpenDialog
    Filter = 
      'SNES Compressed Wave (*.brr)|*.brr|Microsoft Wave (*.wav)|*.wav|' +
      'All files (*.*)|*.*'
    Options = [ofHideReadOnly, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Title = 'Import Sound...'
    Left = 340
    Top = 116
  end
  object dlgExport: TSaveDialog
    Filter = 'SNES Compressed Wave (*.brr)|*.brr|Microsoft Wave (*.wav)|*.wav'
    Options = [ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Title = 'Export Sound...'
    Left = 340
    Top = 144
  end
  object popSrc: TPopupMenu
    OnPopup = popSrcPopup
    Left = 148
    Top = 116
    object itmSShow: TMenuItem
      Caption = 'S&how'
      OnClick = itmSShowClick
    end
    object itmSHide: TMenuItem
      Caption = '&Hide'
      OnClick = itmSHideClick
    end
    object itmSMute: TMenuItem
      Caption = '&Mute'
      OnClick = itmMuteClick
    end
    object itmSSilence: TMenuItem
      Caption = '&Silence'
      OnClick = itmSilenceClick
    end
    object itmSOptions: TMenuItem
      Caption = '&Options...'
      OnClick = itmSetOptClick
    end
  end
  object popTime: TPopupMenu
    AutoPopup = False
    OnPopup = popTimePopup
    Left = 716
    Top = 288
    object itmTSetCur: TMenuItem
      Caption = 'Save current position'
      OnClick = itmTSetCurClick
    end
    object itmSetPos: TMenuItem
      Caption = 'Set starting position to'
      OnClick = itmSetPosClick
    end
  end
  object popFile: TPopupMenu
    AutoHotkeys = maManual
    AutoLineReduction = maManual
    Left = 8
    Top = 92
    object itmFSong: TMenuItem
      Tag = 83
      Caption = 'Song'
      OnClick = itmFClick
    end
    object itmFGame: TMenuItem
      Tag = 71
      Caption = 'Game'
      OnClick = itmFClick
    end
    object itmFArtist: TMenuItem
      Tag = 65
      Caption = 'Artist'
      OnClick = itmFClick
    end
    object itmFPub: TMenuItem
      Tag = 80
      Caption = 'Publisher'
      OnClick = itmFClick
    end
    object itmFOST: TMenuItem
      Tag = 79
      Caption = 'OST'
      OnClick = itmFClick
    end
    object itmFTrack: TMenuItem
      Tag = 84
      Caption = 'Track'
      OnClick = itmFClick
    end
    object itmFDumper: TMenuItem
      Tag = 85
      Caption = 'Dumper'
      OnClick = itmFClick
    end
    object itmFDate: TMenuItem
      Tag = 68
      Caption = 'Date'
      OnClick = itmFClick
    end
    object itmFEmu: TMenuItem
      Tag = 69
      Caption = 'Emulator'
      OnClick = itmFClick
    end
    object itmFCmnt: TMenuItem
      Tag = 67
      Caption = 'Comments'
      OnClick = itmFClick
    end
    object itmFLen: TMenuItem
      Tag = 76
      Caption = 'Length'
      OnClick = itmFClick
    end
  end
  object dlgSaveInfo: TSaveDialog
    Filter = 'Sound Source Information (*.ssi)|*.ssi|All Files (*.*)|*.*'
    Options = [ofHideReadOnly, ofPathMustExist, ofNoReadOnlyReturn, ofEnableSizing]
    Title = 'Save Sound Source Information...'
    Left = 339
    Top = 494
  end
  object dlgOpenInfo: TOpenDialog
    DefaultExt = 'ssi'
    Filter = 'Sound Source Information (*.ssi)|*.ssi|All Files (*.*)|*.*'
    Options = [ofHideReadOnly, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Title = 'Load Sound Source Information...'
    Left = 339
    Top = 466
  end
end
