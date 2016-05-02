object Form1: TForm1
  Left = 271
  Top = 105
  Width = 774
  Height = 512
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 280
    Top = 8
    Width = 48
    Height = 16
    Caption = 'Label1'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Button2: TButton
    Left = 280
    Top = 56
    Width = 75
    Height = 25
    Caption = 'WriteData'
    TabOrder = 0
    OnClick = Button2Click
  end
  object Memo1: TMemo
    Left = 72
    Top = 0
    Width = 97
    Height = 473
    ScrollBars = ssVertical
    TabOrder = 1
  end
  object Button3: TButton
    Left = 280
    Top = 88
    Width = 75
    Height = 25
    Caption = 'readData'
    TabOrder = 2
    OnClick = Button3Click
  end
  object Edit2: TEdit
    Left = 280
    Top = 24
    Width = 121
    Height = 21
    TabOrder = 3
    Text = '12'
  end
  object Button4: TButton
    Left = 280
    Top = 120
    Width = 75
    Height = 25
    Caption = 'FirmUpd'
    TabOrder = 4
    OnClick = Button4Click
  end
  object Memo2: TMemo
    Left = 0
    Top = 0
    Width = 65
    Height = 473
    Lines.Strings = (
      'Memo2')
    TabOrder = 5
  end
  object Memo3: TMemo
    Left = 176
    Top = 0
    Width = 97
    Height = 473
    TabOrder = 6
  end
  object CheckBox1: TCheckBox
    Left = 280
    Top = 160
    Width = 57
    Height = 17
    Caption = 'FIFO IT'
    TabOrder = 7
  end
  object CheckBox2: TCheckBox
    Left = 342
    Top = 160
    Width = 57
    Height = 17
    Caption = 'FFOV'
    TabOrder = 8
  end
  object CheckBox3: TCheckBox
    Left = 398
    Top = 160
    Width = 57
    Height = 17
    Caption = 'WK-UP'
    TabOrder = 9
  end
  object CheckBox5: TCheckBox
    Left = 455
    Top = 160
    Width = 57
    Height = 17
    Caption = 'LBD'
    TabOrder = 10
  end
  object CheckBox4: TCheckBox
    Left = 280
    Top = 184
    Width = 57
    Height = 17
    Caption = 'FFEM'
    TabOrder = 11
  end
  object CheckBox6: TCheckBox
    Left = 342
    Top = 184
    Width = 57
    Height = 17
    Caption = 'DRSSI'
    TabOrder = 12
  end
  object CheckBox7: TCheckBox
    Left = 398
    Top = 184
    Width = 57
    Height = 17
    Caption = 'DQD'
    TabOrder = 13
  end
  object CheckBox8: TCheckBox
    Left = 455
    Top = 184
    Width = 57
    Height = 17
    Caption = 'CRL'
    TabOrder = 14
  end
  object CheckBox9: TCheckBox
    Left = 280
    Top = 208
    Width = 57
    Height = 17
    Caption = 'ATGL'
    TabOrder = 15
  end
  object CheckBox10: TCheckBox
    Left = 342
    Top = 208
    Width = 57
    Height = 17
    Caption = 'ASAME'
    TabOrder = 16
  end
  object Memo4: TMemo
    Left = 280
    Top = 232
    Width = 169
    Height = 241
    Lines.Strings = (
      'Memo4')
    ScrollBars = ssVertical
    TabOrder = 17
  end
  object Button1: TButton
    Left = 536
    Top = 64
    Width = 75
    Height = 25
    Caption = 'Button1'
    TabOrder = 18
    OnClick = Button1Click
  end
  object Button5: TButton
    Left = 536
    Top = 96
    Width = 75
    Height = 25
    Caption = 'Button5'
    TabOrder = 19
    OnClick = Button5Click
  end
  object Memo5: TMemo
    Left = 464
    Top = 232
    Width = 201
    Height = 241
    Lines.Strings = (
      'Memo5')
    ScrollBars = ssVertical
    TabOrder = 20
  end
  object Button6: TButton
    Left = 632
    Top = 176
    Width = 75
    Height = 25
    Caption = 'Button6'
    TabOrder = 21
    OnClick = Button6Click
  end
  object Edit1: TEdit
    Left = 624
    Top = 136
    Width = 121
    Height = 21
    TabOrder = 22
  end
  object ConTimer: TTimer
    Enabled = False
    Interval = 2000
    OnTimer = ConTimerTimer
    Left = 424
    Top = 24
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 10
    OnTimer = Timer1Timer
    Left = 440
    Top = 88
  end
end
