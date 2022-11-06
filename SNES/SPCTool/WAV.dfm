object FrmWav: TFrmWav
  Left = 855
  Top = 290
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Record Output to .WAV'
  ClientHeight = 374
  ClientWidth = 328
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  Scaled = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object lblRate: TLabel
    Left = 4
    Top = 212
    Width = 68
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Sample Rate'
    Layout = tlCenter
    WordWrap = True
  end
  object lblBits: TLabel
    Left = 4
    Top = 240
    Width = 68
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Sample Size'
    Layout = tlCenter
    WordWrap = True
  end
  object lblInter: TLabel
    Left = 176
    Top = 212
    Width = 68
    Height = 21
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'Interpolation'
    Layout = tlCenter
    WordWrap = True
  end
  object lblStereoT: TLabel
    Left = 176
    Top = 272
    Width = 101
    Height = 13
    AutoSize = False
    Caption = 'Stereo separation'
    Layout = tlCenter
    WordWrap = True
  end
  object lblStereo: TLabel
    Left = 284
    Top = 272
    Width = 33
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = '50'
    Layout = tlCenter
  end
  object lblEchoT: TLabel
    Left = 176
    Top = 328
    Width = 101
    Height = 13
    AutoSize = False
    Caption = 'EFB crosstalk'
    Layout = tlCenter
    WordWrap = True
  end
  object lblEcho: TLabel
    Left = 284
    Top = 328
    Width = 33
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = '-50%'
    Layout = tlCenter
  end
  object bvlAdvanced: TBevel
    Left = 1
    Top = 120
    Width = 326
    Height = 17
    Shape = bsTopLine
    Style = bsRaised
  end
  object btnCancel: TButton
    Left = 248
    Top = 88
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object btnOK: TButton
    Left = 167
    Top = 88
    Width = 75
    Height = 25
    Caption = 'Go!'
    Default = True
    ModalResult = 1
    TabOrder = 0
  end
  object cboBits: TComboBox
    Left = 76
    Top = 240
    Width = 77
    Height = 21
    HelpContext = 2112
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 2
    OnExit = cboBitsExit
    Items.Strings = (
      '16-bit'
      '32-bit float')
  end
  object cboRate: TComboBox
    Left = 76
    Top = 212
    Width = 77
    Height = 21
    HelpContext = 2111
    DropDownCount = 6
    ItemHeight = 13
    MaxLength = 5
    TabOrder = 3
    OnExit = cboRateExit
    Items.Strings = (
      '32000'
      '44100'
      '48000'
      '64000'
      '88200'
      '96000'
      '176400'
      '192000')
  end
  object cboInter: TComboBox
    Left = 248
    Top = 212
    Width = 77
    Height = 21
    HelpContext = 2114
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 4
    OnExit = cboInterExit
    Items.Strings = (
      'None'
      'Linear'
      'Cubic'
      'Gaussian'
      'Sinc')
  end
  object chkAnalog: TCheckBox
    Left = 4
    Top = 290
    Width = 169
    Height = 17
    HelpContext = 2121
    Caption = 'Analog simulation'
    TabOrder = 5
    OnClick = chkAnalogClick
  end
  object chkOldSmp: TCheckBox
    Left = 4
    Top = 354
    Width = 169
    Height = 17
    HelpContext = 2124
    Caption = 'Use old sample decompression'
    TabOrder = 6
  end
  object chkNoEcho: TCheckBox
    Left = 4
    Top = 334
    Width = 169
    Height = 17
    HelpContext = 2125
    Caption = 'Disable echo'
    TabOrder = 7
  end
  object trkStereo: TTrackBar
    Tag = 32768
    Left = 172
    Top = 288
    Width = 153
    Height = 25
    HelpContext = 2131
    Max = 100
    Orientation = trHorizontal
    PageSize = 5
    Frequency = 5
    Position = 50
    SelEnd = 0
    SelStart = 0
    TabOrder = 8
    ThumbLength = 15
    TickMarks = tmBottomRight
    TickStyle = tsAuto
    OnChange = trkStereoChange
  end
  object trkEcho: TTrackBar
    Tag = -16384
    Left = 172
    Top = 344
    Width = 153
    Height = 25
    HelpContext = 2132
    Max = 100
    Min = -100
    Orientation = trHorizontal
    PageSize = 10
    Frequency = 10
    Position = -50
    SelEnd = 0
    SelStart = 0
    TabOrder = 9
    ThumbLength = 15
    TickMarks = tmBottomRight
    TickStyle = tsAuto
    OnChange = trkEchoChange
  end
  object grpNorm: TRadioGroup
    Tag = 2
    Left = 4
    Top = 128
    Width = 320
    Height = 77
    Caption = ' Normalization '
    ItemIndex = 2
    Items.Strings = (
      'Off (use amplification level)'
      'Each song individually'
      'All songs as a group')
    TabOrder = 10
  end
  object grpMode: TRadioGroup
    Left = 4
    Top = 4
    Width = 320
    Height = 77
    Caption = ' What do you want to do? '
    Items.Strings = (
      'Make high quality .WAV files for a CD'
      'Make .WAV files that simulate the quality of the SNES'
      'Choose my own settings')
    TabOrder = 11
    OnClick = grpModeClick
  end
  object chkMix: TCheckBox
    Left = 4
    Top = 270
    Width = 169
    Height = 17
    Caption = 'High quality mixing'
    TabOrder = 12
    OnClick = chkMixClick
  end
  object chkFilter: TCheckBox
    Left = 4
    Top = 312
    Width = 169
    Height = 17
    Caption = 'Voice source filtering'
    TabOrder = 13
  end
end
