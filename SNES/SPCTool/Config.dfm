object FrmConfig: TFrmConfig
  Left = 542
  Top = 281
  BorderIcons = [biSystemMenu, biHelp]
  BorderStyle = bsDialog
  BorderWidth = 2
  Caption = 'Preferences'
  ClientHeight = 289
  ClientWidth = 345
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pgcCfg: TPageControl
    Left = 0
    Top = 0
    Width = 345
    Height = 257
    ActivePage = tabDSP
    TabOrder = 0
    object tabDSP: TTabSheet
      HelpContext = 210
      Caption = 'DSP'
      object lblRate: TLabel
        Left = 4
        Top = 52
        Width = 80
        Height = 21
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Sample Rate'
        Layout = tlCenter
        WordWrap = True
      end
      object lblBits: TLabel
        Left = 4
        Top = 76
        Width = 80
        Height = 21
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Sample Size'
        Layout = tlCenter
        WordWrap = True
      end
      object lblInter: TLabel
        Left = 172
        Top = 52
        Width = 80
        Height = 21
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Interpolation'
        Layout = tlCenter
        WordWrap = True
      end
      object lblTEcho: TLabel
        Left = 172
        Top = 154
        Width = 129
        Height = 25
        AutoSize = False
        Caption = 'Echo feedback crosstalk'
        Layout = tlCenter
        WordWrap = True
      end
      object lblEcho: TLabel
        Left = 296
        Top = 154
        Width = 33
        Height = 25
        Alignment = taRightJustify
        AutoSize = False
        Caption = '-50%'
        Layout = tlCenter
      end
      object lblTStereo: TLabel
        Left = 172
        Top = 98
        Width = 129
        Height = 25
        AutoSize = False
        Caption = 'Stereo separation'
        Layout = tlCenter
        WordWrap = True
      end
      object lblStereo: TLabel
        Left = 296
        Top = 98
        Width = 33
        Height = 25
        Alignment = taRightJustify
        AutoSize = False
        Caption = '50'
        Layout = tlCenter
      end
      object cboRate: TComboBox
        Left = 88
        Top = 52
        Width = 77
        Height = 21
        HelpContext = 2111
        DropDownCount = 6
        ItemHeight = 13
        MaxLength = 5
        PopupMenu = popHelp
        TabOrder = 0
        OnChange = ItemChange
        OnExit = cboRateExit
        Items.Strings = (
          '8000'
          '11025'
          '16000'
          '22050'
          '32000'
          '44100'
          '48000'
          '64000'
          '88200'
          '96000')
      end
      object cboBits: TComboBox
        Left = 88
        Top = 76
        Width = 77
        Height = 21
        HelpContext = 2112
        Style = csDropDownList
        ItemHeight = 13
        PopupMenu = popHelp
        TabOrder = 1
        OnChange = ItemChange
        Items.Strings = (
          '16-bit'
          '32-bit')
      end
      object cboInter: TComboBox
        Left = 256
        Top = 52
        Width = 77
        Height = 21
        HelpContext = 2114
        Style = csDropDownList
        ItemHeight = 13
        PopupMenu = popHelp
        TabOrder = 2
        OnChange = ItemChange
        Items.Strings = (
          'None'
          'Linear'
          'Cubic'
          'Gaussian'
          'Sinc')
      end
      object trkEcho: TTrackBar
        Tag = -16384
        Left = 168
        Top = 180
        Width = 169
        Height = 25
        HelpContext = 2132
        Max = 100
        Min = -100
        Orientation = trHorizontal
        PageSize = 10
        PopupMenu = popHelp
        Frequency = 10
        Position = -50
        SelEnd = 0
        SelStart = 0
        TabOrder = 11
        ThumbLength = 15
        TickMarks = tmBottomRight
        TickStyle = tsAuto
        OnChange = trkEchoChange
      end
      object trkStereo: TTrackBar
        Tag = 32768
        Left = 168
        Top = 124
        Width = 169
        Height = 25
        HelpContext = 2131
        Max = 100
        Orientation = trHorizontal
        PageSize = 5
        PopupMenu = popHelp
        Frequency = 5
        Position = 50
        SelEnd = 0
        SelStart = 0
        TabOrder = 10
        ThumbLength = 15
        TickMarks = tmBottomRight
        TickStyle = tsAuto
        OnChange = trkStereoChange
      end
      object chkSurr: TCheckBox
        Left = 4
        Top = 168
        Width = 161
        Height = 17
        HelpContext = 2122
        Caption = '"Surround" sound'
        PopupMenu = popHelp
        TabOrder = 7
        OnClick = ItemChange
      end
      object chkReverse: TCheckBox
        Left = 4
        Top = 188
        Width = 161
        Height = 17
        HelpContext = 2123
        Caption = 'Reverse stereo'
        PopupMenu = popHelp
        TabOrder = 8
        OnClick = ItemChange
      end
      object chkADPCM: TCheckBox
        Left = 4
        Top = 208
        Width = 185
        Height = 17
        HelpContext = 2124
        Caption = 'Use old sample decompression'
        PopupMenu = popHelp
        TabOrder = 9
        OnClick = ItemChange
      end
      object chkNoEcho: TCheckBox
        Left = 4
        Top = 148
        Width = 161
        Height = 17
        HelpContext = 2125
        Caption = 'Disable echo'
        TabOrder = 6
        OnClick = ItemChange
      end
      object grpPreset: TRadioGroup
        Left = 4
        Top = 4
        Width = 329
        Height = 35
        Caption = ' DSP Emulation '
        Columns = 3
        Items.Strings = (
          'High quality'
          'Like the SNES'
          'Custom')
        TabOrder = 12
        OnClick = grpPresetClick
      end
      object chkLow: TCheckBox
        Left = 4
        Top = 108
        Width = 161
        Height = 17
        Caption = 'Analog simulation'
        TabOrder = 4
        OnClick = chkLowClick
      end
      object chkMix: TCheckBox
        Left = 196
        Top = 80
        Width = 137
        Height = 17
        Caption = 'High quality mixing'
        TabOrder = 3
        OnClick = chkMixClick
      end
      object chkFilter: TCheckBox
        Left = 4
        Top = 128
        Width = 161
        Height = 17
        Caption = 'Voice source filtering'
        TabOrder = 5
        OnClick = ItemChange
      end
    end
    object tabMixing: TTabSheet
      HelpContext = 230
      Caption = 'Mixing'
      ImageIndex = 2
      object lblTAmp: TLabel
        Left = 92
        Top = 20
        Width = 59
        Height = 13
        Caption = 'Amplification'
      end
      object lblTThresh: TLabel
        Left = 92
        Top = 64
        Width = 82
        Height = 13
        Caption = 'Output Threshold'
      end
      object lblAmp: TLabel
        Left = 156
        Top = 20
        Width = 28
        Height = 13
        Caption = '6.0dB'
      end
      object lblThresh: TLabel
        Left = 180
        Top = 64
        Width = 34
        Height = 13
        Caption = '0.00dB'
      end
      object bvlMixing: TBevel
        Left = 4
        Top = 9
        Width = 329
        Height = 9
        Shape = bsTopLine
      end
      object lblMixing: TLabel
        Left = 12
        Top = 4
        Width = 36
        Height = 13
        Caption = ' Mixing '
      end
      object lblTMin: TLabel
        Left = 264
        Top = 20
        Width = 32
        Height = 13
        AutoSize = False
        Caption = 'Min'
      end
      object lblTMax: TLabel
        Left = 300
        Top = 20
        Width = 32
        Height = 13
        AutoSize = False
        Caption = 'Max'
      end
      object grpAAR: TRadioGroup
        Left = 4
        Top = 20
        Width = 81
        Height = 81
        HelpContext = 2310
        Caption = 'AAR'
        ItemIndex = 1
        Items.Strings = (
          'Off'
          'On'
          'Increase')
        PopupMenu = popHelp
        TabOrder = 0
        TabStop = True
        OnClick = grpAARClick
      end
      object trkAmp: TTrackBar
        Left = 88
        Top = 36
        Width = 173
        Height = 25
        HelpContext = 2321
        Max = 180
        Min = -60
        Orientation = trHorizontal
        PageSize = 10
        PopupMenu = popHelp
        Frequency = 10
        Position = 60
        SelEnd = 180
        SelStart = 0
        TabOrder = 1
        ThumbLength = 15
        TickMarks = tmBottomRight
        TickStyle = tsAuto
        OnChange = trkAmpChange
      end
      object trkThresh: TTrackBar
        Left = 88
        Top = 80
        Width = 173
        Height = 25
        HelpContext = 2322
        Max = 60
        Min = -60
        Orientation = trHorizontal
        PageSize = 5
        PopupMenu = popHelp
        Frequency = 5
        Position = 0
        SelEnd = 0
        SelStart = 0
        TabOrder = 4
        ThumbLength = 15
        TickMarks = tmBottomRight
        TickStyle = tsAuto
        OnChange = trkThreshChange
      end
      object txtMinAmp: TEdit
        Left = 264
        Top = 39
        Width = 33
        Height = 21
        HelpContext = 2341
        MaxLength = 2
        PopupMenu = popHelp
        TabOrder = 2
        OnChange = ItemChange
        OnExit = txtMinAmpExit
      end
      object txtMaxAmp: TEdit
        Left = 300
        Top = 39
        Width = 33
        Height = 21
        HelpContext = 2342
        MaxLength = 3
        PopupMenu = popHelp
        TabOrder = 3
        OnChange = ItemChange
        OnExit = txtMaxAmpExit
      end
      object chkReset: TCheckBox
        Left = 4
        Top = 108
        Width = 325
        Height = 17
        HelpContext = 2333
        Caption = 'Reset level at the start of a song'
        PopupMenu = popHelp
        TabOrder = 5
        OnClick = ItemChange
      end
    end
    object tabTime: TTabSheet
      HelpContext = 220
      Caption = 'Time'
      ImageIndex = 1
      object lblSep: TLabel
        Left = 115
        Top = 32
        Width = 7
        Height = 13
        Alignment = taCenter
        AutoSize = False
        Caption = ':'
      end
      object lblSong: TLabel
        Left = 4
        Top = 28
        Width = 80
        Height = 21
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Song Length'
        Layout = tlCenter
        WordWrap = True
      end
      object lblFade: TLabel
        Left = 4
        Top = 52
        Width = 80
        Height = 21
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Fade Length'
        Layout = tlCenter
        WordWrap = True
      end
      object bvlTime: TBevel
        Left = 4
        Top = 9
        Width = 329
        Height = 9
        Shape = bsTopLine
      end
      object lblTime: TLabel
        Left = 12
        Top = 4
        Width = 68
        Height = 13
        Caption = ' Time Options '
      end
      object lblFFT: TLabel
        Left = 172
        Top = 28
        Width = 129
        Height = 21
        AutoSize = False
        Caption = 'Fast-Forward Speed'
        Layout = tlCenter
        WordWrap = True
      end
      object lblFF: TLabel
        Left = 296
        Top = 28
        Width = 33
        Height = 21
        Alignment = taRightJustify
        AutoSize = False
        Caption = '4x'
        Layout = tlCenter
      end
      object txtSongMin: TEdit
        Left = 88
        Top = 28
        Width = 25
        Height = 21
        HelpContext = 2211
        MaxLength = 2
        PopupMenu = popHelp
        TabOrder = 0
        OnChange = ItemChange
        OnExit = txtSongMinExit
      end
      object txtSongSec: TEdit
        Left = 124
        Top = 28
        Width = 41
        Height = 21
        HelpContext = 2211
        MaxLength = 5
        PopupMenu = popHelp
        TabOrder = 1
        OnChange = ItemChange
        OnExit = CheckTime
      end
      object txtFade: TEdit
        Left = 88
        Top = 52
        Width = 77
        Height = 21
        HelpContext = 2211
        MaxLength = 5
        PopupMenu = popHelp
        TabOrder = 2
        OnChange = ItemChange
        OnExit = CheckTime
      end
      object chkFast: TCheckBox
        Left = 4
        Top = 108
        Width = 325
        Height = 17
        HelpContext = 2223
        Caption = 'Use a faster seeking method (breaks some songs)'
        PopupMenu = popHelp
        TabOrder = 3
        OnClick = ItemChange
      end
      object chkBin: TCheckBox
        Left = 4
        Top = 128
        Width = 325
        Height = 17
        HelpContext = 2221
        Caption = 'Default to binary ID666 tags'
        PopupMenu = popHelp
        TabOrder = 4
        OnClick = ItemChange
      end
      object trkFF: TTrackBar
        Left = 168
        Top = 52
        Width = 169
        Height = 25
        HelpContext = 2131
        Max = 16
        Min = 2
        Orientation = trHorizontal
        PageSize = 1
        PopupMenu = popHelp
        Frequency = 1
        Position = 4
        SelEnd = 0
        SelStart = 0
        TabOrder = 5
        ThumbLength = 15
        TickMarks = tmBottomRight
        TickStyle = tsAuto
        OnChange = trkFFChange
      end
      object chkNext: TCheckBox
        Left = 4
        Top = 88
        Width = 325
        Height = 17
        Caption = 'Automatically advance to the next file when a song ends'
        Enabled = False
        TabOrder = 6
        OnClick = ItemChange
      end
    end
    object tabSrc: TTabSheet
      Caption = 'Source'
      ImageIndex = 5
      object bvlWave: TBevel
        Left = 4
        Top = 9
        Width = 329
        Height = 9
        Shape = bsTopLine
      end
      object lblSrc: TLabel
        Left = 12
        Top = 4
        Width = 123
        Height = 13
        Caption = ' Sound Source Detection '
      end
      object lblMin: TLabel
        Left = 4
        Top = 28
        Width = 80
        Height = 21
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Minimum Size'
        Layout = tlCenter
        WordWrap = True
      end
      object lblMax: TLabel
        Left = 4
        Top = 52
        Width = 80
        Height = 21
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Maximum Size'
        Layout = tlCenter
        WordWrap = True
      end
      object chkLinear: TCheckBox
        Left = 4
        Top = 88
        Width = 329
        Height = 17
        Caption = 'First block must use linear compression (i.e. filter 0)'
        TabOrder = 2
        OnClick = chkLinearClick
      end
      object chkSilence: TCheckBox
        Left = 4
        Top = 108
        Width = 329
        Height = 17
        Caption = 'First block must contain silence'
        Enabled = False
        TabOrder = 3
        OnClick = ItemChange
      end
      object chkRange: TCheckBox
        Left = 4
        Top = 128
        Width = 329
        Height = 17
        Caption = 'Range nybble can'#39't be greater than 12'
        TabOrder = 4
        OnClick = ItemChange
      end
      object txtMinSize: TEdit
        Left = 88
        Top = 28
        Width = 77
        Height = 21
        TabOrder = 0
        OnChange = ItemChange
        OnExit = txtMinSizeExit
      end
      object txtMaxSize: TEdit
        Left = 88
        Top = 52
        Width = 77
        Height = 21
        TabOrder = 1
        OnChange = ItemChange
        OnExit = txtMaxSizeExit
      end
      object chkDP: TCheckBox
        Left = 4
        Top = 148
        Width = 329
        Height = 17
        Caption = 'No blocks in direct pages (SPC'#39's only)'
        TabOrder = 5
        OnClick = ItemChange
      end
      object chkInit: TCheckBox
        Left = 4
        Top = 192
        Width = 329
        Height = 17
        Caption = 'Insert initial silence block when importing sound sources'
        TabOrder = 6
      end
    end
    object tabDebug: TTabSheet
      HelpContext = 240
      Caption = 'Debug'
      ImageIndex = 3
      object bvlDebug: TBevel
        Left = 4
        Top = 9
        Width = 329
        Height = 9
        Shape = bsTopLine
      end
      object lblDebug: TLabel
        Left = 12
        Top = 4
        Width = 64
        Height = 13
        Caption = ' Disassembly '
      end
      object lstDisasm: TListBox
        Left = 4
        Top = 20
        Width = 329
        Height = 49
        TabStop = False
        Color = clBlack
        ExtendedSelect = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clSilver
        Font.Height = 11
        Font.Name = 'Lucida Console'
        Font.Pitch = fpFixed
        Font.Style = []
        ItemHeight = 11
        Items.Strings = (
          '0536 8F 40 F2  Mov  [0F2h],40h    ;F2->02'
          '0539 77 18     Cmp  A,[[18h]+Y]   ;18->1080->1'
          '0538 D0 E7     BNE  0524          ;-25'
          '053D 6A 15 AB  And1 C,~[1B15],5   ;1B15.5->1')
        ParentFont = False
        TabOrder = 0
      end
      object chkPC: TCheckBox
        Left = 4
        Top = 148
        Width = 160
        Height = 17
        Caption = 'Program Counter'
        Checked = True
        State = cbChecked
        TabOrder = 4
        OnClick = lstDisasmChange
      end
      object chkOpcode: TCheckBox
        Left = 4
        Top = 168
        Width = 160
        Height = 17
        Caption = 'Code bytes'
        Checked = True
        State = cbChecked
        TabOrder = 5
        OnClick = lstDisasmChange
      end
      object grpCase: TRadioGroup
        Left = 116
        Top = 72
        Width = 104
        Height = 73
        Caption = 'Case'
        ItemIndex = 1
        Items.Strings = (
          'lower'
          'Mixed'
          'UPPER')
        TabOrder = 2
        TabStop = True
        OnClick = lstDisasmChange
      end
      object chkCmnt: TCheckBox
        Left = 4
        Top = 188
        Width = 160
        Height = 17
        Caption = 'Print comments'
        Checked = True
        State = cbChecked
        TabOrder = 6
        OnClick = lstDisasmChange
      end
      object grpHex: TRadioGroup
        Left = 228
        Top = 72
        Width = 104
        Height = 73
        Caption = 'Hex Format'
        ItemIndex = 1
        Items.Strings = (
          '$FF'
          '0FFh'
          '0xFF')
        TabOrder = 3
        TabStop = True
        OnClick = lstDisasmChange
      end
      object grpSyn: TRadioGroup
        Left = 4
        Top = 72
        Width = 104
        Height = 55
        Caption = 'Syntax'
        ItemIndex = 1
        Items.Strings = (
          'ISAS'
          'SPCTool')
        TabOrder = 1
        TabStop = True
        OnClick = lstDisasmChange
      end
    end
    object tabDev: TTabSheet
      Caption = 'Devices'
      ImageIndex = 4
      object bvlHW: TBevel
        Left = 4
        Top = 141
        Width = 329
        Height = 9
        Shape = bsTopLine
      end
      object bvlOut: TBevel
        Left = 4
        Top = 9
        Width = 329
        Height = 9
        Shape = bsTopLine
      end
      object lblOut: TLabel
        Left = 12
        Top = 4
        Width = 38
        Height = 13
        Caption = ' Output '
      end
      object lblDev: TLabel
        Left = 4
        Top = 28
        Width = 80
        Height = 21
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Ouput Device'
        Layout = tlCenter
        WordWrap = True
      end
      object lblBuf: TLabel
        Left = 4
        Top = 56
        Width = 80
        Height = 21
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Buffer Size'
        Layout = tlCenter
        WordWrap = True
      end
      object lblBufSize: TLabel
        Left = 168
        Top = 56
        Width = 41
        Height = 21
        AutoSize = False
        Caption = '60ms'
        Layout = tlCenter
      end
      object lblHW: TLabel
        Left = 12
        Top = 136
        Width = 84
        Height = 13
        Caption = ' SNES Hardware '
      end
      object lblLPT: TLabel
        Left = 4
        Top = 184
        Width = 80
        Height = 21
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Comm. Port'
        Layout = tlCenter
        WordWrap = True
      end
      object lblHWT: TLabel
        Left = 4
        Top = 160
        Width = 80
        Height = 21
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Device'
        Layout = tlCenter
        WordWrap = True
      end
      object cboDev: TComboBox
        Left = 88
        Top = 28
        Width = 177
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = ItemChange
        Items.Strings = (
          'Wave Mapper')
      end
      object trkBuf: TTrackBar
        Left = 84
        Top = 56
        Width = 81
        Height = 21
        Max = 6
        Orientation = trHorizontal
        PageSize = 1
        Frequency = 1
        Position = 1
        SelEnd = 0
        SelStart = 0
        TabOrder = 1
        ThumbLength = 11
        TickMarks = tmBottomRight
        TickStyle = tsAuto
        OnChange = trkBufChange
      end
      object chkForce: TCheckBox
        Left = 4
        Top = 88
        Width = 329
        Height = 17
        Caption = 'Force direct output'
        TabOrder = 2
        OnClick = ItemChange
      end
      object cboLPT: TComboBox
        Left = 88
        Top = 184
        Width = 77
        Height = 21
        HelpContext = 2111
        Style = csDropDownList
        DropDownCount = 6
        ItemHeight = 13
        MaxLength = 5
        PopupMenu = popHelp
        TabOrder = 3
        OnChange = ItemChange
        Items.Strings = (
          'LPT1'
          'LPT2'
          'LPT3')
      end
      object cboHW: TComboBox
        Left = 88
        Top = 160
        Width = 177
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 4
        OnChange = cboHWChange
        Items.Strings = (
          'none'
          'Direct SNES APU'
          'Super Magicom / WildCard')
      end
    end
    object TabOther: TTabSheet
      Caption = 'Other'
      ImageIndex = 6
      object bvlVis: TBevel
        Left = 4
        Top = 9
        Width = 329
        Height = 9
        Shape = bsTopLine
      end
      object bvlReset: TBevel
        Left = 4
        Top = 162
        Width = 329
        Height = 9
        Shape = bsTopLine
      end
      object lblCfg: TLabel
        Left = 12
        Top = 156
        Width = 68
        Height = 13
        Caption = ' Configuration '
      end
      object lblReset: TLabel
        Left = 4
        Top = 180
        Width = 249
        Height = 21
        AutoSize = False
        Caption = 'Reset configuration to default settings'
        Layout = tlCenter
      end
      object lblVis: TLabel
        Left = 12
        Top = 4
        Width = 64
        Height = 13
        Caption = ' Visualization '
      end
      object btnReset: TButton
        Left = 260
        Top = 178
        Width = 73
        Height = 25
        Caption = 'Reset'
        TabOrder = 0
        OnClick = btnResetClick
      end
      object grpVis: TRadioGroup
        Left = 4
        Top = 20
        Width = 81
        Height = 81
        Caption = ' Rate '
        Items.Strings = (
          'Disabled'
          '25Hz'
          '50Hz')
        TabOrder = 1
        OnClick = ItemChange
      end
    end
  end
  object btnHelp: TButton
    Left = 0
    Top = 264
    Width = 73
    Height = 25
    Caption = '&Help'
    TabOrder = 1
    OnClick = btnHelpClick
  end
  object btnOK: TButton
    Left = 112
    Top = 264
    Width = 73
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 2
    OnClick = btnOKClick
  end
  object btnCancel: TButton
    Left = 192
    Top = 264
    Width = 73
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 3
    OnClick = btnCancelClick
  end
  object btnApply: TButton
    Left = 272
    Top = 264
    Width = 73
    Height = 25
    Caption = '&Apply'
    Enabled = False
    TabOrder = 4
    OnClick = btnApplyClick
  end
  object popHelp: TPopupMenu
    Left = 80
    Top = 260
    object itmWhat: TMenuItem
      Caption = 'What'#39's this?'
      OnClick = itmWhatClick
    end
  end
end
