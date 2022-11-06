object FrmDSP: TFrmDSP
  Tag = 31
  Left = 472
  Top = 461
  AutoSize = True
  BorderIcons = [biSystemMenu, biHelp]
  BorderStyle = bsDialog
  Caption = 'DSP Register Editor'
  ClientHeight = 329
  ClientWidth = 624
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  KeyPreview = True
  OldCreateOrder = False
  Position = poDefault
  OnClose = FormClose
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  OnKeyPress = FormKeyPress
  OnKeyUp = FormKeyUp
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object lblKON: TLabel
    Left = 224
    Top = 104
    Width = 60
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Key on'
    Layout = tlBottom
  end
  object lblKOF: TLabel
    Left = 224
    Top = 124
    Width = 60
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Key off'
    Layout = tlBottom
  end
  object lblEVol: TLabel
    Left = 480
    Top = 0
    Width = 37
    Height = 21
    Alignment = taCenter
    AutoSize = False
    Caption = 'Echo'
    Layout = tlCenter
  end
  object lblFIRT: TLabel
    Left = 432
    Top = 192
    Width = 192
    Height = 17
    AutoSize = False
    Caption = 'FIR Filter Coefficients'
    Layout = tlBottom
  end
  object lblEDLT: TLabel
    Left = 432
    Top = 140
    Width = 45
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Delay'
    Layout = tlCenter
  end
  object lblEDL: TLabel
    Left = 588
    Top = 140
    Width = 36
    Height = 21
    AutoSize = False
    Caption = '0ms'
    Color = clBtnFace
    ParentColor = False
    Layout = tlCenter
  end
  object lblEFBT: TLabel
    Left = 432
    Top = 164
    Width = 45
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Feed Bk'
    Layout = tlCenter
  end
  object lblEFB: TLabel
    Left = 588
    Top = 164
    Width = 36
    Height = 21
    AutoSize = False
    Caption = '0%'
    Layout = tlCenter
  end
  object lblEON: TLabel
    Left = 224
    Top = 148
    Width = 60
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Echo'
    Layout = tlBottom
  end
  object lblPMON: TLabel
    Left = 224
    Top = 168
    Width = 60
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Pitch Mod'
    Layout = tlBottom
  end
  object lblNON: TLabel
    Left = 224
    Top = 188
    Width = 60
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Noise'
    Layout = tlBottom
  end
  object lblMVol: TLabel
    Left = 432
    Top = 0
    Width = 41
    Height = 21
    Alignment = taCenter
    AutoSize = False
    Caption = 'Main'
    Layout = tlCenter
  end
  object lblDIR: TLabel
    Left = 524
    Top = 40
    Width = 57
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Source Dir'
    Layout = tlCenter
  end
  object lblESA: TLabel
    Left = 524
    Top = 64
    Width = 57
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Echo Addr'
    Layout = tlCenter
  end
  object bvlSep2: TBevel
    Left = 424
    Top = 0
    Width = 5
    Height = 329
    Shape = bsLeftLine
  end
  object lblFIR0: TLabel
    Left = 429
    Top = 216
    Width = 21
    Height = 12
    Alignment = taCenter
    AutoSize = False
    Caption = '0'
  end
  object lblFIR1: TLabel
    Left = 453
    Top = 216
    Width = 21
    Height = 12
    Alignment = taCenter
    AutoSize = False
    Caption = '0'
  end
  object lblFIR2: TLabel
    Left = 477
    Top = 216
    Width = 21
    Height = 12
    Alignment = taCenter
    AutoSize = False
    Caption = '0'
  end
  object lblFIR3: TLabel
    Left = 501
    Top = 216
    Width = 21
    Height = 12
    Alignment = taCenter
    AutoSize = False
    Caption = '0'
  end
  object lblFIR4: TLabel
    Left = 525
    Top = 216
    Width = 21
    Height = 12
    Alignment = taCenter
    AutoSize = False
    Caption = '0'
  end
  object lblFIR5: TLabel
    Left = 549
    Top = 216
    Width = 21
    Height = 12
    Alignment = taCenter
    AutoSize = False
    Caption = '0'
  end
  object lblFIR6: TLabel
    Left = 573
    Top = 216
    Width = 21
    Height = 13
    Alignment = taCenter
    AutoSize = False
    Caption = '0'
  end
  object lblFIR7: TLabel
    Left = 597
    Top = 216
    Width = 21
    Height = 12
    Alignment = taCenter
    AutoSize = False
    Caption = '0'
  end
  object lblMVolL: TLabel
    Left = 429
    Top = 24
    Width = 21
    Height = 12
    Alignment = taCenter
    AutoSize = False
    Caption = '0'
  end
  object lblMVolR: TLabel
    Left = 453
    Top = 24
    Width = 21
    Height = 12
    Alignment = taCenter
    AutoSize = False
    Caption = '0'
  end
  object lblEVolL: TLabel
    Left = 477
    Top = 24
    Width = 21
    Height = 12
    Alignment = taCenter
    AutoSize = False
    Caption = '0'
  end
  object lblEVolR: TLabel
    Left = 501
    Top = 24
    Width = 21
    Height = 12
    Alignment = taCenter
    AutoSize = False
    Caption = '0'
  end
  object bvlSpc: TBevel
    Left = 0
    Top = 4
    Width = 2
    Height = 321
    Shape = bsSpacer
  end
  object lblV0: TLabel
    Left = 292
    Top = 84
    Width = 13
    Height = 13
    Alignment = taCenter
    AutoSize = False
    Caption = '0'
  end
  object lblV1: TLabel
    Left = 308
    Top = 84
    Width = 13
    Height = 13
    Alignment = taCenter
    AutoSize = False
    Caption = '1'
  end
  object lblV2: TLabel
    Left = 324
    Top = 84
    Width = 13
    Height = 13
    Alignment = taCenter
    AutoSize = False
    Caption = '2'
  end
  object lblV3: TLabel
    Left = 340
    Top = 84
    Width = 13
    Height = 13
    Alignment = taCenter
    AutoSize = False
    Caption = '3'
  end
  object lblV4: TLabel
    Left = 356
    Top = 84
    Width = 13
    Height = 13
    Alignment = taCenter
    AutoSize = False
    Caption = '4'
  end
  object lblV5: TLabel
    Left = 372
    Top = 84
    Width = 13
    Height = 13
    Alignment = taCenter
    AutoSize = False
    Caption = '5'
  end
  object lblV6: TLabel
    Left = 388
    Top = 84
    Width = 13
    Height = 13
    Alignment = taCenter
    AutoSize = False
    Caption = '6'
  end
  object lblV7: TLabel
    Left = 404
    Top = 84
    Width = 13
    Height = 13
    Alignment = taCenter
    AutoSize = False
    Caption = '7'
  end
  object tabVoice: TTabControl
    Left = 5
    Top = 4
    Width = 212
    Height = 321
    TabOrder = 0
    Tabs.Strings = (
      '0'
      '1'
      '2'
      '3'
      '4'
      '5'
      '6'
      '7')
    TabIndex = 0
    TabStop = False
    TabWidth = 26
    OnChange = tabVoiceChange
  end
  object btnKON0: TButton
    Tag = 76
    Left = 292
    Top = 104
    Width = 13
    Height = 13
    TabOrder = 3
    TabStop = False
    OnMouseDown = btnKeyMouseDown
  end
  object btnKON1: TButton
    Tag = 332
    Left = 308
    Top = 104
    Width = 13
    Height = 13
    TabOrder = 4
    TabStop = False
    OnMouseDown = btnKeyMouseDown
  end
  object btnKON2: TButton
    Tag = 588
    Left = 324
    Top = 104
    Width = 13
    Height = 13
    TabOrder = 5
    TabStop = False
    OnMouseDown = btnKeyMouseDown
  end
  object btnKON3: TButton
    Tag = 844
    Left = 340
    Top = 104
    Width = 13
    Height = 13
    TabOrder = 6
    TabStop = False
    OnMouseDown = btnKeyMouseDown
  end
  object btnKON4: TButton
    Tag = 1100
    Left = 356
    Top = 104
    Width = 13
    Height = 13
    TabOrder = 7
    TabStop = False
    OnMouseDown = btnKeyMouseDown
  end
  object btnKON5: TButton
    Tag = 1356
    Left = 372
    Top = 104
    Width = 13
    Height = 13
    TabOrder = 8
    TabStop = False
    OnMouseDown = btnKeyMouseDown
  end
  object btnKON6: TButton
    Tag = 1612
    Left = 388
    Top = 104
    Width = 13
    Height = 13
    TabOrder = 9
    TabStop = False
    OnMouseDown = btnKeyMouseDown
  end
  object btnKON7: TButton
    Tag = 1868
    Left = 404
    Top = 104
    Width = 13
    Height = 13
    TabOrder = 10
    TabStop = False
    OnMouseDown = btnKeyMouseDown
  end
  object btnKOF0: TButton
    Tag = 92
    Left = 292
    Top = 124
    Width = 13
    Height = 13
    TabOrder = 11
    TabStop = False
    OnMouseDown = btnKeyMouseDown
  end
  object btnKOF1: TButton
    Tag = 348
    Left = 308
    Top = 124
    Width = 13
    Height = 13
    TabOrder = 12
    TabStop = False
    OnMouseDown = btnKeyMouseDown
  end
  object btnKOF2: TButton
    Tag = 604
    Left = 324
    Top = 124
    Width = 13
    Height = 13
    TabOrder = 13
    TabStop = False
    OnMouseDown = btnKeyMouseDown
  end
  object btnKOF3: TButton
    Tag = 860
    Left = 340
    Top = 124
    Width = 13
    Height = 13
    TabOrder = 14
    TabStop = False
    OnMouseDown = btnKeyMouseDown
  end
  object btnKOF4: TButton
    Tag = 1116
    Left = 356
    Top = 124
    Width = 13
    Height = 13
    TabOrder = 15
    TabStop = False
    OnMouseDown = btnKeyMouseDown
  end
  object btnKOF5: TButton
    Tag = 1372
    Left = 372
    Top = 124
    Width = 13
    Height = 13
    TabOrder = 16
    TabStop = False
    OnMouseDown = btnKeyMouseDown
  end
  object btnKOF6: TButton
    Tag = 1628
    Left = 388
    Top = 124
    Width = 13
    Height = 13
    TabOrder = 17
    TabStop = False
    OnMouseDown = btnKeyMouseDown
  end
  object btnKOF7: TButton
    Tag = 1884
    Left = 404
    Top = 124
    Width = 13
    Height = 13
    TabOrder = 18
    TabStop = False
    OnMouseDown = btnKeyMouseDown
  end
  object trkEDL: TTrackBar
    Tag = 125
    Left = 480
    Top = 140
    Width = 108
    Height = 21
    Max = 15
    Orientation = trHorizontal
    Frequency = 1
    Position = 0
    SelEnd = 0
    SelStart = 0
    TabOrder = 58
    TabStop = False
    ThumbLength = 11
    TickMarks = tmBottomRight
    TickStyle = tsAuto
    OnChange = trkEDLChange
  end
  object trkEFB: TTrackBar
    Tag = 13
    Left = 480
    Top = 168
    Width = 108
    Height = 17
    Max = 127
    Min = -128
    Orientation = trHorizontal
    PageSize = 12
    Frequency = 12
    Position = 0
    SelEnd = 0
    SelStart = 0
    TabOrder = 59
    TabStop = False
    ThumbLength = 11
    TickMarks = tmBottomRight
    TickStyle = tsAuto
    OnChange = trkEFBChange
  end
  object chkEON0: TCheckBox
    Tag = 77
    Left = 292
    Top = 148
    Width = 13
    Height = 17
    TabStop = False
    TabOrder = 19
    OnClick = chkSwitchClick
  end
  object chkEON1: TCheckBox
    Tag = 333
    Left = 308
    Top = 148
    Width = 13
    Height = 17
    TabStop = False
    TabOrder = 20
    OnClick = chkSwitchClick
  end
  object chkEON2: TCheckBox
    Tag = 589
    Left = 324
    Top = 148
    Width = 13
    Height = 17
    TabStop = False
    TabOrder = 21
    OnClick = chkSwitchClick
  end
  object chkEON3: TCheckBox
    Tag = 845
    Left = 340
    Top = 148
    Width = 13
    Height = 17
    TabStop = False
    TabOrder = 22
    OnClick = chkSwitchClick
  end
  object chkEON4: TCheckBox
    Tag = 1101
    Left = 356
    Top = 148
    Width = 13
    Height = 17
    TabStop = False
    TabOrder = 23
    OnClick = chkSwitchClick
  end
  object chkEON5: TCheckBox
    Tag = 1357
    Left = 372
    Top = 148
    Width = 13
    Height = 17
    TabStop = False
    TabOrder = 24
    OnClick = chkSwitchClick
  end
  object chkEON6: TCheckBox
    Tag = 1613
    Left = 388
    Top = 148
    Width = 13
    Height = 17
    TabStop = False
    TabOrder = 25
    OnClick = chkSwitchClick
  end
  object chkEON7: TCheckBox
    Tag = 1869
    Left = 404
    Top = 148
    Width = 13
    Height = 17
    TabStop = False
    TabOrder = 26
    OnClick = chkSwitchClick
  end
  object chkPMON0: TCheckBox
    Tag = 45
    Left = 292
    Top = 168
    Width = 13
    Height = 17
    TabStop = False
    TabOrder = 27
    OnClick = chkSwitchClick
  end
  object chkPMON1: TCheckBox
    Tag = 301
    Left = 308
    Top = 168
    Width = 13
    Height = 17
    TabStop = False
    TabOrder = 28
    OnClick = chkSwitchClick
  end
  object chkPMON2: TCheckBox
    Tag = 557
    Left = 324
    Top = 168
    Width = 13
    Height = 17
    TabStop = False
    TabOrder = 29
    OnClick = chkSwitchClick
  end
  object chkPMON3: TCheckBox
    Tag = 813
    Left = 340
    Top = 168
    Width = 13
    Height = 17
    TabStop = False
    TabOrder = 30
    OnClick = chkSwitchClick
  end
  object chkPMON4: TCheckBox
    Tag = 1069
    Left = 356
    Top = 168
    Width = 13
    Height = 17
    TabStop = False
    TabOrder = 31
    OnClick = chkSwitchClick
  end
  object chkPMON5: TCheckBox
    Tag = 1325
    Left = 372
    Top = 168
    Width = 13
    Height = 17
    TabStop = False
    TabOrder = 32
    OnClick = chkSwitchClick
  end
  object chkPMON6: TCheckBox
    Tag = 1581
    Left = 388
    Top = 168
    Width = 13
    Height = 17
    TabStop = False
    TabOrder = 33
    OnClick = chkSwitchClick
  end
  object chkPMON7: TCheckBox
    Tag = 1837
    Left = 404
    Top = 168
    Width = 13
    Height = 17
    TabStop = False
    TabOrder = 34
    OnClick = chkSwitchClick
  end
  object chkNON0: TCheckBox
    Tag = 61
    Left = 292
    Top = 188
    Width = 13
    Height = 17
    TabStop = False
    TabOrder = 35
    OnClick = chkSwitchClick
  end
  object chkNON1: TCheckBox
    Tag = 317
    Left = 308
    Top = 188
    Width = 13
    Height = 17
    TabStop = False
    TabOrder = 36
    OnClick = chkSwitchClick
  end
  object chkNON2: TCheckBox
    Tag = 573
    Left = 324
    Top = 188
    Width = 13
    Height = 17
    TabStop = False
    TabOrder = 37
    OnClick = chkSwitchClick
  end
  object chkNON3: TCheckBox
    Tag = 829
    Left = 340
    Top = 188
    Width = 13
    Height = 17
    TabStop = False
    TabOrder = 38
    OnClick = chkSwitchClick
  end
  object chkNON4: TCheckBox
    Tag = 1085
    Left = 356
    Top = 188
    Width = 13
    Height = 17
    TabStop = False
    TabOrder = 39
    OnClick = chkSwitchClick
  end
  object chkNON5: TCheckBox
    Tag = 1341
    Left = 372
    Top = 188
    Width = 13
    Height = 17
    TabStop = False
    TabOrder = 40
    OnClick = chkSwitchClick
  end
  object chkNON6: TCheckBox
    Tag = 1597
    Left = 388
    Top = 188
    Width = 13
    Height = 17
    TabStop = False
    TabOrder = 41
    OnClick = chkSwitchClick
  end
  object chkNON7: TCheckBox
    Tag = 1853
    Left = 404
    Top = 188
    Width = 13
    Height = 17
    TabStop = False
    TabOrder = 42
    OnClick = chkSwitchClick
  end
  object btnENDX: TButton
    Tag = 124
    Left = 224
    Top = 212
    Width = 64
    Height = 17
    Caption = 'EndX'
    TabOrder = 43
    TabStop = False
    OnClick = btnENDXClick
  end
  object chkENDX0: TCheckBox
    Left = 292
    Top = 212
    Width = 13
    Height = 17
    TabStop = False
    Enabled = False
    TabOrder = 44
  end
  object chkENDX1: TCheckBox
    Left = 308
    Top = 212
    Width = 13
    Height = 17
    TabStop = False
    Enabled = False
    TabOrder = 45
  end
  object chkENDX2: TCheckBox
    Left = 324
    Top = 212
    Width = 13
    Height = 17
    TabStop = False
    Enabled = False
    TabOrder = 46
  end
  object chkENDX3: TCheckBox
    Left = 340
    Top = 212
    Width = 13
    Height = 17
    TabStop = False
    Enabled = False
    TabOrder = 47
  end
  object chkENDX4: TCheckBox
    Left = 356
    Top = 212
    Width = 13
    Height = 17
    TabStop = False
    Enabled = False
    TabOrder = 48
  end
  object chkENDX5: TCheckBox
    Left = 372
    Top = 212
    Width = 13
    Height = 17
    TabStop = False
    Enabled = False
    TabOrder = 49
  end
  object chkENDX6: TCheckBox
    Left = 388
    Top = 212
    Width = 13
    Height = 17
    TabStop = False
    Enabled = False
    TabOrder = 50
  end
  object chkENDX7: TCheckBox
    Left = 404
    Top = 212
    Width = 13
    Height = 17
    TabStop = False
    Enabled = False
    TabOrder = 51
  end
  object grpFlg: TGroupBox
    Left = 224
    Top = 0
    Width = 194
    Height = 77
    Caption = 'Flags / Noise Clock'
    TabOrder = 2
    object lblNCK: TLabel
      Left = 92
      Top = 36
      Width = 85
      Height = 12
      Alignment = taCenter
      AutoSize = False
      Caption = '0Hz'
    end
    object chkRES: TCheckBox
      Tag = 1900
      Left = 8
      Top = 16
      Width = 77
      Height = 17
      Caption = 'Soft reset'
      TabOrder = 0
      OnClick = chkSwitchClick
    end
    object chkMUTE: TCheckBox
      Tag = 1644
      Left = 8
      Top = 36
      Width = 77
      Height = 17
      Caption = 'Mute'
      TabOrder = 1
      OnClick = chkSwitchClick
    end
    object chkECEN: TCheckBox
      Tag = 1388
      Left = 8
      Top = 56
      Width = 97
      Height = 17
      Caption = '!Echo enable'
      TabOrder = 2
      OnClick = chkSwitchClick
    end
    object trkNCK: TTrackBar
      Tag = 108
      Left = 81
      Top = 16
      Width = 108
      Height = 21
      Max = 31
      Orientation = trHorizontal
      PageSize = 4
      Frequency = 1
      Position = 0
      SelEnd = 0
      SelStart = 0
      TabOrder = 3
      ThumbLength = 11
      TickMarks = tmBottomRight
      TickStyle = tsAuto
      OnChange = trkNCKChange
    end
  end
  object txtDIR: TEdit
    Tag = 12
    Left = 588
    Top = 40
    Width = 33
    Height = 21
    TabStop = False
    Color = clBtnFace
    MaxLength = 4
    ReadOnly = True
    TabOrder = 56
  end
  object txtESA: TEdit
    Tag = 12
    Left = 588
    Top = 64
    Width = 33
    Height = 21
    TabStop = False
    Color = clBtnFace
    MaxLength = 4
    ReadOnly = True
    TabOrder = 57
  end
  object trkFIR0: TTrackBar
    Tag = 15
    Left = 432
    Top = 228
    Width = 16
    Height = 100
    Max = 128
    Min = -127
    Orientation = trVertical
    PageSize = 16
    Frequency = 1
    Position = 1
    SelEnd = 0
    SelStart = 0
    TabOrder = 60
    TabStop = False
    ThumbLength = 11
    TickMarks = tmBoth
    TickStyle = tsNone
    OnChange = trkFIRChange
  end
  object trkFIR1: TTrackBar
    Tag = 31
    Left = 456
    Top = 228
    Width = 16
    Height = 100
    Max = 128
    Min = -127
    Orientation = trVertical
    PageSize = 16
    Frequency = 1
    Position = 1
    SelEnd = 0
    SelStart = 0
    TabOrder = 61
    TabStop = False
    ThumbLength = 11
    TickMarks = tmBoth
    TickStyle = tsNone
    OnChange = trkFIRChange
  end
  object trkFIR2: TTrackBar
    Tag = 47
    Left = 480
    Top = 228
    Width = 16
    Height = 100
    Max = 128
    Min = -127
    Orientation = trVertical
    PageSize = 16
    Frequency = 1
    Position = 1
    SelEnd = 0
    SelStart = 0
    TabOrder = 62
    TabStop = False
    ThumbLength = 11
    TickMarks = tmBoth
    TickStyle = tsNone
    OnChange = trkFIRChange
  end
  object trkFIR3: TTrackBar
    Tag = 63
    Left = 504
    Top = 228
    Width = 16
    Height = 100
    Max = 128
    Min = -127
    Orientation = trVertical
    PageSize = 16
    Frequency = 1
    Position = 1
    SelEnd = 0
    SelStart = 0
    TabOrder = 63
    TabStop = False
    ThumbLength = 11
    TickMarks = tmBoth
    TickStyle = tsNone
    OnChange = trkFIRChange
  end
  object trkFIR4: TTrackBar
    Tag = 79
    Left = 528
    Top = 228
    Width = 16
    Height = 100
    Max = 128
    Min = -127
    Orientation = trVertical
    PageSize = 16
    Frequency = 1
    Position = 1
    SelEnd = 0
    SelStart = 0
    TabOrder = 64
    TabStop = False
    ThumbLength = 11
    TickMarks = tmBoth
    TickStyle = tsNone
    OnChange = trkFIRChange
  end
  object trkFIR5: TTrackBar
    Tag = 95
    Left = 552
    Top = 228
    Width = 16
    Height = 100
    Max = 128
    Min = -127
    Orientation = trVertical
    PageSize = 16
    Frequency = 1
    Position = 1
    SelEnd = 0
    SelStart = 0
    TabOrder = 65
    TabStop = False
    ThumbLength = 11
    TickMarks = tmBoth
    TickStyle = tsNone
    OnChange = trkFIRChange
  end
  object trkFIR6: TTrackBar
    Tag = 111
    Left = 576
    Top = 228
    Width = 16
    Height = 100
    Max = 128
    Min = -127
    Orientation = trVertical
    PageSize = 16
    Frequency = 1
    Position = 1
    SelEnd = 0
    SelStart = 0
    TabOrder = 66
    TabStop = False
    ThumbLength = 11
    TickMarks = tmBoth
    TickStyle = tsNone
    OnChange = trkFIRChange
  end
  object trkFIR7: TTrackBar
    Tag = 127
    Left = 600
    Top = 228
    Width = 16
    Height = 100
    Max = 128
    Min = -127
    Orientation = trVertical
    PageSize = 16
    Frequency = 1
    Position = 1
    SelEnd = 0
    SelStart = 0
    TabOrder = 67
    TabStop = False
    ThumbLength = 11
    TickMarks = tmBoth
    TickStyle = tsNone
    OnChange = trkFIRChange
  end
  object trkMVolL: TTrackBar
    Tag = 12
    Left = 432
    Top = 36
    Width = 16
    Height = 100
    Max = 128
    Min = -127
    Orientation = trVertical
    PageSize = 16
    Frequency = 1
    Position = 1
    SelEnd = 0
    SelStart = 0
    TabOrder = 52
    TabStop = False
    ThumbLength = 11
    TickMarks = tmBoth
    TickStyle = tsNone
    OnChange = trkVolChange
  end
  object trkMVolR: TTrackBar
    Tag = 28
    Left = 456
    Top = 36
    Width = 16
    Height = 100
    Max = 128
    Min = -127
    Orientation = trVertical
    PageSize = 16
    Frequency = 1
    Position = 1
    SelEnd = 0
    SelStart = 0
    TabOrder = 53
    TabStop = False
    ThumbLength = 11
    TickMarks = tmBoth
    TickStyle = tsNone
    OnChange = trkVolChange
  end
  object trkEVolL: TTrackBar
    Tag = 44
    Left = 480
    Top = 36
    Width = 16
    Height = 100
    Max = 128
    Min = -127
    Orientation = trVertical
    PageSize = 16
    Frequency = 1
    Position = 1
    SelEnd = 0
    SelStart = 0
    TabOrder = 54
    TabStop = False
    ThumbLength = 11
    TickMarks = tmBoth
    TickStyle = tsNone
    OnChange = trkVolChange
  end
  object trkEVolR: TTrackBar
    Tag = 60
    Left = 504
    Top = 36
    Width = 16
    Height = 100
    Max = 128
    Min = -127
    Orientation = trVertical
    PageSize = 16
    Frequency = 1
    Position = 1
    SelEnd = 0
    SelStart = 0
    TabOrder = 55
    TabStop = False
    ThumbLength = 11
    TickMarks = tmBoth
    TickStyle = tsNone
    OnChange = trkVolChange
  end
  object btnRelease: TButton
    Left = 344
    Top = 300
    Width = 73
    Height = 25
    Caption = 'Release'
    TabOrder = 68
    TabStop = False
    OnClick = btnReleaseClick
  end
  object pnlVoice: TPanel
    Left = 12
    Top = 32
    Width = 197
    Height = 285
    BevelOuter = bvNone
    TabOrder = 1
    object lblART: TLabel
      Left = 0
      Top = 92
      Width = 44
      Height = 21
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Attack'
      Layout = tlCenter
    end
    object lblDRT: TLabel
      Left = 0
      Top = 116
      Width = 44
      Height = 21
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Decay'
      Layout = tlCenter
    end
    object lblSLT: TLabel
      Left = 0
      Top = 140
      Width = 44
      Height = 21
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Level'
      Layout = tlCenter
    end
    object lblSRT: TLabel
      Left = 0
      Top = 164
      Width = 44
      Height = 21
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Sustain'
      Layout = tlCenter
    end
    object lblAR: TLabel
      Left = 156
      Top = 92
      Width = 44
      Height = 21
      AutoSize = False
      Caption = '4.0'
      Layout = tlCenter
    end
    object lblSL: TLabel
      Left = 156
      Top = 140
      Width = 44
      Height = 21
      AutoSize = False
      Caption = '1/8th'
      Layout = tlCenter
    end
    object lblSR: TLabel
      Left = 156
      Top = 164
      Width = 44
      Height = 21
      AutoSize = False
      Caption = 'inf'
      Layout = tlCenter
    end
    object lblDR: TLabel
      Left = 156
      Top = 116
      Width = 44
      Height = 21
      AutoSize = False
      Caption = '1062'
      Layout = tlCenter
    end
    object lblVolT: TLabel
      Left = 0
      Top = 0
      Width = 44
      Height = 21
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Volume'
      Layout = tlCenter
    end
    object lblPitch: TLabel
      Left = 0
      Top = 24
      Width = 44
      Height = 21
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Pitch'
      Layout = tlCenter
    end
    object lblEnvX: TLabel
      Left = 124
      Top = 0
      Width = 36
      Height = 21
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'EnvX'
      Layout = tlCenter
    end
    object lblOutX: TLabel
      Left = 124
      Top = 24
      Width = 36
      Height = 21
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'OutX'
      Layout = tlCenter
    end
    object lblSrc: TLabel
      Left = 0
      Top = 48
      Width = 44
      Height = 21
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Source'
      Layout = tlCenter
    end
    object trkAR: TTrackBar
      Tag = 5
      Left = 44
      Top = 92
      Width = 108
      Height = 21
      Max = 15
      Orientation = trHorizontal
      Frequency = 1
      Position = 0
      SelEnd = 0
      SelStart = 0
      TabOrder = 0
      TabStop = False
      ThumbLength = 11
      TickMarks = tmBottomRight
      TickStyle = tsAuto
      OnChange = trkARChange
    end
    object trkDR: TTrackBar
      Tag = 5
      Left = 44
      Top = 116
      Width = 108
      Height = 21
      Max = 7
      Orientation = trHorizontal
      PageSize = 1
      Frequency = 1
      Position = 0
      SelEnd = 0
      SelStart = 0
      TabOrder = 1
      TabStop = False
      ThumbLength = 11
      TickMarks = tmBottomRight
      TickStyle = tsAuto
      OnChange = trkDRChange
    end
    object trkSL: TTrackBar
      Tag = 6
      Left = 44
      Top = 140
      Width = 108
      Height = 21
      Max = 7
      Orientation = trHorizontal
      PageSize = 1
      Frequency = 1
      Position = 0
      SelEnd = 0
      SelStart = 0
      TabOrder = 2
      TabStop = False
      ThumbLength = 11
      TickMarks = tmBottomRight
      TickStyle = tsAuto
      OnChange = trkSLChange
    end
    object trkSR: TTrackBar
      Tag = 6
      Left = 44
      Top = 164
      Width = 108
      Height = 21
      Max = 31
      Orientation = trHorizontal
      PageSize = 4
      Frequency = 1
      Position = 0
      SelEnd = 0
      SelStart = 0
      TabOrder = 3
      TabStop = False
      ThumbLength = 11
      TickMarks = tmBottomRight
      TickStyle = tsAuto
      OnChange = trkSRChange
    end
    object chkADSR: TCheckBox
      Tag = 5
      Left = 0
      Top = 72
      Width = 97
      Height = 17
      TabStop = False
      Caption = 'ADSR'
      Checked = True
      State = cbChecked
      TabOrder = 4
      OnClick = chkADSRClick
    end
    object trkGain: TTrackBar
      Tag = 7
      Left = -4
      Top = 264
      Width = 206
      Height = 21
      Max = 127
      Orientation = trHorizontal
      Frequency = 2
      Position = 0
      SelEnd = 0
      SelStart = 0
      TabOrder = 5
      TabStop = False
      ThumbLength = 11
      TickMarks = tmBottomRight
      TickStyle = tsAuto
      OnChange = trkGainChange
    end
    object txtVolL: TEdit
      Left = 48
      Top = 0
      Width = 33
      Height = 21
      TabStop = False
      MaxLength = 4
      TabOrder = 6
      OnChange = txtVolChange
      OnKeyDown = txtVolKeyDown
      OnKeyPress = FormKeyPress
      OnKeyUp = FormKeyUp
    end
    object txtVolR: TEdit
      Tag = 1
      Left = 84
      Top = 0
      Width = 33
      Height = 21
      TabStop = False
      MaxLength = 4
      TabOrder = 7
      OnChange = txtVolChange
      OnKeyDown = txtVolKeyDown
      OnKeyPress = FormKeyPress
      OnKeyUp = FormKeyUp
    end
    object txtPitch: TEdit
      Tag = 2
      Left = 48
      Top = 24
      Width = 69
      Height = 21
      TabStop = False
      MaxLength = 5
      TabOrder = 8
      OnChange = txtPitchChange
      OnKeyDown = txtPitchKeyDown
      OnKeyPress = FormKeyPress
      OnKeyUp = FormKeyUp
    end
    object txtEnvX: TEdit
      Tag = 8
      Left = 164
      Top = 0
      Width = 33
      Height = 21
      TabStop = False
      Color = clBtnFace
      ReadOnly = True
      TabOrder = 9
    end
    object txtOutX: TEdit
      Tag = 9
      Left = 164
      Top = 24
      Width = 33
      Height = 21
      TabStop = False
      Color = clBtnFace
      ReadOnly = True
      TabOrder = 10
    end
    object txtSrc: TEdit
      Tag = 4
      Left = 48
      Top = 48
      Width = 33
      Height = 21
      TabStop = False
      MaxLength = 3
      TabOrder = 11
      OnChange = txtSrcChange
      OnKeyDown = txtPitchKeyDown
      OnKeyPress = FormKeyPress
      OnKeyUp = FormKeyUp
    end
    object grpGain: TGroupBox
      Left = 0
      Top = 188
      Width = 197
      Height = 74
      Caption = 'Gain Mode'
      TabOrder = 12
      object lblGain: TLabel
        Left = 104
        Top = 14
        Width = 85
        Height = 16
        Alignment = taRightJustify
        AutoSize = False
        Caption = '0'
        Layout = tlCenter
      end
      object rdoDirect: TRadioButton
        Left = 8
        Top = 16
        Width = 85
        Height = 17
        Caption = 'Direct'
        TabOrder = 0
        OnClick = grpGainClick
      end
      object rdoInc: TRadioButton
        Left = 8
        Top = 34
        Width = 85
        Height = 17
        Caption = '+Linear'
        TabOrder = 1
        OnClick = grpGainClick
      end
      object rdoBent: TRadioButton
        Left = 8
        Top = 52
        Width = 85
        Height = 17
        Caption = '+Bent line'
        TabOrder = 2
        OnClick = grpGainClick
      end
      object rdoDec: TRadioButton
        Left = 96
        Top = 34
        Width = 93
        Height = 17
        Caption = '-Linear'
        TabOrder = 3
        OnClick = grpGainClick
      end
      object rdoExp: TRadioButton
        Left = 96
        Top = 52
        Width = 93
        Height = 17
        Caption = '-Exponential'
        TabOrder = 4
        OnClick = grpGainClick
      end
    end
  end
end
