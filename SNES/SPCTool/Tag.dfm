object FrmTag: TFrmTag
  Left = 814
  Top = 390
  BorderIcons = [biSystemMenu, biHelp]
  BorderStyle = bsDialog
  Caption = 'ID666 Tag Editor'
  ClientHeight = 451
  ClientWidth = 576
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  HelpFile = 'Context.hlp'
  KeyPreview = True
  OldCreateOrder = False
  Position = poMainFormCenter
  Scaled = False
  ShowHint = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnKeyPress = FormKeyPress
  OnKeyUp = FormKeyUp
  OnPaint = FormPaint
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object lblSong: TLabel
    Left = 4
    Top = 4
    Width = 53
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Song'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblGame: TLabel
    Left = 4
    Top = 32
    Width = 53
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Game'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblArtist: TLabel
    Left = 4
    Top = 60
    Width = 53
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Artist'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblPub: TLabel
    Left = 4
    Top = 88
    Width = 53
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Publisher'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblDate: TLabel
    Left = 320
    Top = 32
    Width = 53
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'on'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblDumper: TLabel
    Left = 320
    Top = 4
    Width = 53
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Dumped by'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblDisc: TLabel
    Left = 4
    Top = 172
    Width = 53
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Disc'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblOST: TLabel
    Left = 4
    Top = 144
    Width = 53
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'OST'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblTrack: TLabel
    Left = 116
    Top = 172
    Width = 29
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Track'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblEmu: TLabel
    Left = 320
    Top = 60
    Width = 53
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'with'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblCmnt: TLabel
    Left = 4
    Top = 200
    Width = 53
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Comments'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblMute: TLabel
    Left = 320
    Top = 88
    Width = 53
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Muted'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
    WordWrap = True
  end
  object lblIntro: TLabel
    Left = 320
    Top = 144
    Width = 53
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Intro'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblIntroSep: TLabel
    Left = 421
    Top = 144
    Width = 3
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = ':'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblLoop: TLabel
    Left = 320
    Top = 172
    Width = 53
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Loop'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblLoopSep: TLabel
    Left = 421
    Top = 172
    Width = 3
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = ':'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblEnd: TLabel
    Left = 320
    Top = 200
    Width = 53
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'End'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblFade: TLabel
    Left = 320
    Top = 228
    Width = 53
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Fade'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblEndSep: TLabel
    Left = 421
    Top = 200
    Width = 3
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = ':'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblMute1: TLabel
    Left = 395
    Top = 88
    Width = 8
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = '1'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblMute8: TLabel
    Left = 548
    Top = 88
    Width = 8
    Height = 21
    AutoSize = False
    Caption = '8'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblLoopX: TLabel
    Left = 488
    Top = 172
    Width = 5
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'x'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object bvlTime: TBevel
    Left = 320
    Top = 254
    Width = 253
    Height = 11
    Shape = bsTopLine
  end
  object lblTime: TLabel
    Left = 320
    Top = 264
    Width = 53
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Time'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object lblCopy: TLabel
    Left = 4
    Top = 116
    Width = 53
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Copyright'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblAmp: TLabel
    Left = 320
    Top = 116
    Width = 53
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Amp'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object bvlID6: TBevel
    Left = 4
    Top = 364
    Width = 569
    Height = 13
    Shape = bsTopLine
    Style = bsRaised
  end
  object lblIDumperT: TLabel
    Left = 320
    Top = 372
    Width = 53
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Dumped:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblIDateT: TLabel
    Left = 320
    Top = 392
    Width = 53
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'on:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblIEmuT: TLabel
    Left = 320
    Top = 412
    Width = 53
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'with:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblISong2T: TLabel
    Left = 320
    Top = 432
    Width = 53
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Song:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblIFadeT: TLabel
    Left = 448
    Top = 432
    Width = 53
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Fade:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblISong: TLabel
    Tag = 32
    Left = 64
    Top = 372
    Width = 249
    Height = 13
    AutoSize = False
    Layout = tlCenter
  end
  object lblIGame: TLabel
    Tag = 32
    Left = 64
    Top = 392
    Width = 249
    Height = 13
    AutoSize = False
    Layout = tlCenter
  end
  object lblIArtist: TLabel
    Tag = 32
    Left = 64
    Top = 412
    Width = 249
    Height = 13
    AutoSize = False
    Layout = tlCenter
  end
  object lblICmnt: TLabel
    Tag = 32
    Left = 64
    Top = 432
    Width = 249
    Height = 13
    AutoSize = False
    Layout = tlCenter
  end
  object lblIDumper: TLabel
    Tag = 16
    Left = 380
    Top = 372
    Width = 189
    Height = 13
    AutoSize = False
    Layout = tlCenter
  end
  object lblIDate: TLabel
    Tag = 11
    Left = 380
    Top = 392
    Width = 189
    Height = 13
    AutoSize = False
    Layout = tlCenter
  end
  object lblISong2: TLabel
    Left = 380
    Top = 432
    Width = 61
    Height = 13
    AutoSize = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblIFade: TLabel
    Left = 508
    Top = 432
    Width = 61
    Height = 13
    AutoSize = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblIEmu: TLabel
    Tag = 12
    Left = 380
    Top = 412
    Width = 189
    Height = 13
    AutoSize = False
    Layout = tlCenter
  end
  object pbSong: TPaintBox
    Left = 63
    Top = 8
    Width = 13
    Height = 13
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnMouseDown = pbChkMouseDown
    OnMouseMove = pbChkMouseMove
    OnPaint = pbChkPaint
  end
  object pbGame: TPaintBox
    Left = 63
    Top = 36
    Width = 13
    Height = 13
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnMouseDown = pbChkMouseDown
    OnMouseMove = pbChkMouseMove
    OnPaint = pbChkPaint
  end
  object pbArtist: TPaintBox
    Left = 63
    Top = 64
    Width = 13
    Height = 13
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnMouseDown = pbChkMouseDown
    OnMouseMove = pbChkMouseMove
    OnPaint = pbChkPaint
  end
  object pbPub: TPaintBox
    Left = 63
    Top = 92
    Width = 13
    Height = 13
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnMouseDown = pbChkMouseDown
    OnMouseMove = pbChkMouseMove
    OnPaint = pbChkPaint
  end
  object pbCopy: TPaintBox
    Left = 63
    Top = 120
    Width = 13
    Height = 13
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnMouseDown = pbChkMouseDown
    OnMouseMove = pbChkMouseMove
    OnPaint = pbChkPaint
  end
  object pbOST: TPaintBox
    Left = 63
    Top = 148
    Width = 13
    Height = 13
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnMouseDown = pbChkMouseDown
    OnMouseMove = pbChkMouseMove
    OnPaint = pbChkPaint
  end
  object pbDisc: TPaintBox
    Left = 63
    Top = 176
    Width = 13
    Height = 13
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnMouseDown = pbChkMouseDown
    OnMouseMove = pbChkMouseMove
    OnPaint = pbChkPaint
  end
  object pbTrack: TPaintBox
    Left = 151
    Top = 176
    Width = 13
    Height = 13
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnMouseDown = pbChkMouseDown
    OnMouseMove = pbChkMouseMove
    OnPaint = pbChkPaint
  end
  object pbCmnt: TPaintBox
    Left = 63
    Top = 204
    Width = 13
    Height = 13
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnMouseDown = pbChkMouseDown
    OnMouseMove = pbChkMouseMove
    OnPaint = pbChkPaint
  end
  object pbDumper: TPaintBox
    Left = 379
    Top = 8
    Width = 13
    Height = 13
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnMouseDown = pbChkMouseDown
    OnMouseMove = pbChkMouseMove
    OnPaint = pbChkPaint
  end
  object pbDate: TPaintBox
    Left = 379
    Top = 36
    Width = 13
    Height = 13
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnMouseDown = pbChkMouseDown
    OnMouseMove = pbChkMouseMove
    OnPaint = pbChkPaint
  end
  object pbEmu: TPaintBox
    Left = 379
    Top = 64
    Width = 13
    Height = 13
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnMouseDown = pbChkMouseDown
    OnMouseMove = pbChkMouseMove
    OnPaint = pbChkPaint
  end
  object pbMute: TPaintBox
    Left = 379
    Top = 92
    Width = 13
    Height = 13
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnMouseDown = pbChkMouseDown
    OnMouseMove = pbChkMouseMove
    OnPaint = pbChkPaint
  end
  object pbAmp: TPaintBox
    Left = 379
    Top = 120
    Width = 13
    Height = 13
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnMouseDown = pbChkMouseDown
    OnMouseMove = pbChkMouseMove
    OnPaint = pbChkPaint
  end
  object pbIntro: TPaintBox
    Left = 379
    Top = 148
    Width = 13
    Height = 13
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnMouseDown = pbChkMouseDown
    OnMouseMove = pbChkMouseMove
    OnPaint = pbChkPaint
  end
  object pbLoop: TPaintBox
    Left = 379
    Top = 176
    Width = 13
    Height = 13
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnMouseDown = pbChkMouseDown
    OnMouseMove = pbChkMouseMove
    OnPaint = pbChkPaint
  end
  object pbEnd: TPaintBox
    Left = 379
    Top = 204
    Width = 13
    Height = 13
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnMouseDown = pbChkMouseDown
    OnMouseMove = pbChkMouseMove
    OnPaint = pbChkPaint
  end
  object pbFade: TPaintBox
    Left = 379
    Top = 232
    Width = 13
    Height = 13
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnMouseDown = pbChkMouseDown
    OnMouseMove = pbChkMouseMove
    OnPaint = pbChkPaint
  end
  object lblTimeSep: TLabel
    Left = 421
    Top = 260
    Width = 3
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = ':'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblISongT: TLabel
    Left = 4
    Top = 372
    Width = 53
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Song:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblIGameT: TLabel
    Left = 4
    Top = 392
    Width = 53
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Game:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblIArtistT: TLabel
    Left = 4
    Top = 412
    Width = 53
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Artist:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object lblICommentT: TLabel
    Left = 4
    Top = 432
    Width = 53
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Comments:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
  end
  object pbSongD: TPaintBox
    Left = 295
    Top = 6
    Width = 16
    Height = 17
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnClick = pbDiffClick
    OnMouseDown = pbDiffMouseDown
    OnMouseUp = pbDiffMouseUp
    OnPaint = pbDiffPaint
  end
  object pbGameD: TPaintBox
    Left = 295
    Top = 34
    Width = 16
    Height = 17
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnClick = pbDiffClick
    OnMouseDown = pbDiffMouseDown
    OnMouseUp = pbDiffMouseUp
    OnPaint = pbDiffPaint
  end
  object pbArtistD: TPaintBox
    Left = 295
    Top = 62
    Width = 16
    Height = 17
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnClick = pbDiffClick
    OnMouseDown = pbDiffMouseDown
    OnMouseUp = pbDiffMouseUp
    OnPaint = pbDiffPaint
  end
  object pbPubD: TPaintBox
    Left = 295
    Top = 90
    Width = 16
    Height = 17
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnClick = pbDiffClick
    OnMouseDown = pbDiffMouseDown
    OnMouseUp = pbDiffMouseUp
    OnPaint = pbDiffPaint
  end
  object pbOSTD: TPaintBox
    Left = 295
    Top = 146
    Width = 16
    Height = 17
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnClick = pbDiffClick
    OnMouseDown = pbDiffMouseDown
    OnMouseUp = pbDiffMouseUp
    OnPaint = pbDiffPaint
  end
  object pbDumperD: TPaintBox
    Left = 555
    Top = 6
    Width = 16
    Height = 17
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnClick = pbDiffClick
    OnMouseDown = pbDiffMouseDown
    OnMouseUp = pbDiffMouseUp
    OnPaint = pbDiffPaint
  end
  object txtSong: TEdit
    Tag = 1
    Left = 82
    Top = 6
    Width = 213
    Height = 17
    BorderStyle = bsNone
    TabOrder = 0
    OnChange = ID6Change
  end
  object btnOK: TButton
    Left = 420
    Top = 332
    Width = 73
    Height = 25
    Caption = 'OK'
    Default = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ModalResult = 1
    ParentFont = False
    TabOrder = 37
    OnClick = btnOKClick
  end
  object btnCancel: TButton
    Left = 500
    Top = 332
    Width = 73
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ModalResult = 2
    ParentFont = False
    TabOrder = 38
  end
  object chkMute1: TCheckBox
    Tag = 13
    Left = 406
    Top = 88
    Width = 13
    Height = 21
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 13
  end
  object chkMute2: TCheckBox
    Tag = 13
    Left = 424
    Top = 88
    Width = 13
    Height = 21
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 14
  end
  object chkMute3: TCheckBox
    Tag = 13
    Left = 442
    Top = 88
    Width = 13
    Height = 21
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 15
  end
  object chkMute4: TCheckBox
    Tag = 13
    Left = 460
    Top = 88
    Width = 13
    Height = 21
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 16
  end
  object chkMute5: TCheckBox
    Tag = 13
    Left = 478
    Top = 88
    Width = 13
    Height = 21
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 17
  end
  object chkMute6: TCheckBox
    Tag = 13
    Left = 496
    Top = 88
    Width = 13
    Height = 21
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 18
  end
  object chkMute7: TCheckBox
    Tag = 13
    Left = 514
    Top = 88
    Width = 13
    Height = 21
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 19
  end
  object chkMute8: TCheckBox
    Tag = 13
    Left = 532
    Top = 88
    Width = 13
    Height = 21
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 20
  end
  object chkExt: TCheckBox
    Left = 380
    Top = 288
    Width = 189
    Height = 17
    Caption = 'Extended Information'
    Checked = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    State = cbChecked
    TabOrder = 35
    OnClick = chkExtClick
  end
  object chkBin: TCheckBox
    Left = 380
    Top = 308
    Width = 189
    Height = 17
    Caption = 'Binary Format'
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 36
  end
  object btnID6: TButton
    Left = 4
    Top = 332
    Width = 73
    Height = 25
    Caption = 'ID666 >>'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 39
    OnClick = btnID6Click
  end
  object txtMuteFocus: TEdit
    Tag = 13
    Left = 513
    Top = 88
    Width = 0
    Height = 0
    AutoSelect = False
    AutoSize = False
    BorderStyle = bsNone
    Color = clBtnFace
    TabOrder = 12
    Text = 'txtMuteFocus'
    OnEnter = txtMuteFocusEnter
    OnExit = txtMuteFocusExit
    OnKeyDown = txtMuteFocusKeyDown
    OnKeyPress = FormKeyPress
    OnKeyUp = FormKeyUp
  end
  object btnIntro: TButton
    Tag = 15
    Left = 320
    Top = 144
    Width = 53
    Height = 21
    Hint = 'F5'
    Caption = 'Intro'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 30
    TabStop = False
    OnClick = btnIntroClick
  end
  object btnLoop: TButton
    Tag = 16
    Left = 320
    Top = 172
    Width = 53
    Height = 21
    Hint = 'F6'
    Caption = 'Loop'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 31
    TabStop = False
    OnClick = btnLoopClick
  end
  object btnEnd: TButton
    Tag = 17
    Left = 320
    Top = 200
    Width = 53
    Height = 21
    Hint = 'F7'
    Caption = 'End'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 32
    TabStop = False
    OnClick = btnEndClick
  end
  object btnFade: TButton
    Tag = 18
    Left = 320
    Top = 228
    Width = 53
    Height = 21
    Hint = 'F8'
    Caption = 'Fade'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 33
    TabStop = False
    OnClick = btnFadeClick
  end
  object btnTime: TButton
    Left = 320
    Top = 260
    Width = 53
    Height = 21
    Caption = '&Time'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 34
    OnClick = btnTimeClick
  end
  object txtGame: TEdit
    Tag = 2
    Left = 82
    Top = 34
    Width = 213
    Height = 17
    BorderStyle = bsNone
    TabOrder = 1
    OnChange = ID6Change
  end
  object txtArtist: TEdit
    Tag = 3
    Left = 82
    Top = 62
    Width = 213
    Height = 17
    BorderStyle = bsNone
    TabOrder = 2
    OnChange = ID6Change
  end
  object txtPub: TEdit
    Tag = 4
    Left = 82
    Top = 90
    Width = 213
    Height = 17
    BorderStyle = bsNone
    TabOrder = 3
  end
  object txtCopy: TEdit
    Tag = 5
    Left = 80
    Top = 116
    Width = 37
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    MaxLength = 4
    ParentFont = False
    TabOrder = 4
    OnExit = txtCopyExit
  end
  object txtOST: TEdit
    Tag = 6
    Left = 82
    Top = 146
    Width = 213
    Height = 17
    BorderStyle = bsNone
    TabOrder = 5
  end
  object txtDisc: TEdit
    Tag = 7
    Left = 80
    Top = 172
    Width = 25
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    MaxLength = 1
    ParentFont = False
    TabOrder = 6
    OnExit = txtDiscExit
  end
  object txtTrack: TEdit
    Tag = 8
    Left = 168
    Top = 172
    Width = 33
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    MaxLength = 3
    ParentFont = False
    TabOrder = 7
    OnExit = txtTrackExit
  end
  object mmoCmnt: TMemo
    Tag = 9
    Left = 80
    Top = 200
    Width = 233
    Height = 125
    ScrollBars = ssVertical
    TabOrder = 8
    OnChange = ID6Change
  end
  object txtDump: TEdit
    Tag = 10
    Left = 398
    Top = 6
    Width = 157
    Height = 17
    BorderStyle = bsNone
    TabOrder = 9
    OnChange = ID6Change
  end
  object txtDate: TEdit
    Tag = 11
    Left = 396
    Top = 32
    Width = 73
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 10
    OnEnter = txtDateEnter
    OnExit = txtDateExit
    OnKeyPress = txtDateKeyPress
  end
  object cboEmu: TComboBox
    Tag = 12
    Left = 396
    Top = 60
    Width = 89
    Height = 21
    Style = csDropDownList
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ItemHeight = 13
    ParentFont = False
    TabOrder = 11
    OnChange = ID6Change
  end
  object txtAmp: TEdit
    Tag = 14
    Left = 396
    Top = 116
    Width = 33
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    MaxLength = 4
    ParentFont = False
    TabOrder = 21
    OnExit = txtAmpExit
  end
  object txtIntroMin: TEdit
    Tag = 15
    Left = 396
    Top = 144
    Width = 21
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    MaxLength = 2
    ParentFont = False
    TabOrder = 22
    OnExit = txtMinExit
  end
  object txtIntroSec: TEdit
    Tag = 15
    Left = 428
    Top = 144
    Width = 57
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    MaxLength = 8
    ParentFont = False
    TabOrder = 23
    OnExit = txtSecExit
  end
  object txtLoopMin: TEdit
    Tag = 16
    Left = 396
    Top = 172
    Width = 21
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    MaxLength = 2
    ParentFont = False
    TabOrder = 24
    OnExit = txtMinExit
  end
  object txtLoopSec: TEdit
    Tag = 16
    Left = 428
    Top = 172
    Width = 57
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    MaxLength = 8
    ParentFont = False
    TabOrder = 25
    OnExit = txtSecExit
  end
  object txtLoopNum: TEdit
    Tag = 16
    Left = 497
    Top = 172
    Width = 21
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    MaxLength = 1
    ParentFont = False
    TabOrder = 26
    OnExit = txtLoopNumExit
  end
  object txtEndMin: TEdit
    Tag = 17
    Left = 396
    Top = 200
    Width = 21
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    MaxLength = 2
    ParentFont = False
    TabOrder = 27
    OnExit = txtMinExit
  end
  object txtEndSec: TEdit
    Tag = 17
    Left = 428
    Top = 200
    Width = 57
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    MaxLength = 8
    ParentFont = False
    TabOrder = 28
    OnExit = txtSecExit
  end
  object txtFadeSec: TEdit
    Tag = 18
    Left = 428
    Top = 228
    Width = 57
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    MaxLength = 8
    ParentFont = False
    TabOrder = 29
    OnExit = txtSecExit
  end
  object txtTimeMin: TEdit
    Left = 396
    Top = 260
    Width = 21
    Height = 21
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    MaxLength = 2
    ParentFont = False
    ReadOnly = True
    TabOrder = 40
    OnExit = txtMinExit
  end
  object txtTimeSec: TEdit
    Left = 428
    Top = 260
    Width = 57
    Height = 21
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    MaxLength = 8
    ParentFont = False
    ReadOnly = True
    TabOrder = 41
    OnExit = txtSecExit
  end
  object popDiff: TPopupMenu
    AutoHotkeys = maManual
    AutoLineReduction = maManual
    AutoPopup = False
    Left = 84
    Top = 204
  end
end
