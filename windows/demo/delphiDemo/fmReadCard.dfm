object FormReadCard: TFormReadCard
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = #35835#21345#27979#35797
  ClientHeight = 430
  ClientWidth = 338
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnCreate = FormCreate
  OnHide = FormHide
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pnlIDInf: TPanel
    Left = 0
    Top = 0
    Width = 337
    Height = 423
    BevelOuter = bvLowered
    DoubleBuffered = True
    ParentDoubleBuffered = False
    TabOrder = 0
  end
  object ProgressBar1: TProgressBar
    Left = 0
    Top = 422
    Width = 337
    Height = 9
    DoubleBuffered = False
    Max = 15
    ParentDoubleBuffered = False
    MarqueeInterval = 0
    Step = 1
    State = pbsError
    TabOrder = 1
    Visible = False
  end
end
