object FrmMIDI: TFrmMIDI
  Left = 490
  Top = 352
  Width = 308
  Height = 197
  Caption = 'Log DSP Register Writes to MIDI'
  Color = clBtnFace
  Font.Charset = SHIFTJIS_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = '‚l‚r ‚oƒSƒVƒbƒN'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 12
  object trkTempo: TTrackBar
    Left = 0
    Top = 48
    Width = 264
    Height = 21
    Max = 256
    Min = 1
    Orientation = trHorizontal
    PageSize = 4
    Frequency = 8
    Position = 125
    SelEnd = 0
    SelStart = 0
    TabOrder = 0
    ThumbLength = 11
    TickMarks = tmBottomRight
    TickStyle = tsAuto
    OnChange = trkTempoChange
  end
  object grpTempo: TGroupBox
    Left = 4
    Top = 4
    Width = 257
    Height = 37
    Caption = ' Tempo '
    TabOrder = 1
    object rdoT0: TRadioButton
      Left = 6
      Top = 12
      Width = 56
      Height = 17
      Checked = True
      TabOrder = 0
      TabStop = True
    end
    object rdoT1: TRadioButton
      Left = 67
      Top = 12
      Width = 56
      Height = 17
      TabOrder = 1
    end
    object rdoT2: TRadioButton
      Left = 129
      Top = 12
      Width = 56
      Height = 17
      TabOrder = 2
    end
    object rdoUser: TRadioButton
      Left = 190
      Top = 12
      Width = 56
      Height = 17
      TabOrder = 3
    end
  end
  object btnOK: TButton
    Left = 92
    Top = 102
    Width = 75
    Height = 25
    Caption = 'O&K'
    Default = True
    TabOrder = 2
    OnClick = btnOKClick
  end
  object btnCancel: TButton
    Left = 172
    Top = 102
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 3
  end
end
