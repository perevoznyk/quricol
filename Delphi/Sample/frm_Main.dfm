object frmMain: TfrmMain
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'QR Code Generator'
  ClientHeight = 184
  ClientWidth = 459
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Shape1: TShape
    Left = 8
    Top = 8
    Width = 106
    Height = 106
  end
  object Label1: TLabel
    Left = 130
    Top = 8
    Width = 206
    Height = 13
    Caption = 'Internet address (http://www.mysite.com)'
  end
  object Bevel1: TBevel
    Left = 8
    Top = 128
    Width = 443
    Height = 1
    Shape = bsTopLine
  end
  object imgCode: TImage
    Left = 11
    Top = 11
    Width = 100
    Height = 100
    PopupMenu = PopupMenu1
    Proportional = True
    Stretch = True
  end
  object Label2: TLabel
    Left = 130
    Top = 56
    Width = 32
    Height = 13
    Caption = 'Border'
  end
  object Label3: TLabel
    Left = 216
    Top = 56
    Width = 39
    Height = 13
    Caption = 'Dot Size'
  end
  object edtText: TEdit
    Left = 130
    Top = 27
    Width = 321
    Height = 21
    TabOrder = 0
  end
  object btnCreate: TButton
    Left = 214
    Top = 144
    Width = 75
    Height = 25
    Action = actCreate
    Caption = '&Generate'
    Default = True
    TabOrder = 1
  end
  object btnSave: TButton
    Left = 295
    Top = 144
    Width = 75
    Height = 25
    Action = actSave
    Caption = 'S&ave'
    TabOrder = 2
  end
  object btnClose: TButton
    Left = 376
    Top = 144
    Width = 75
    Height = 25
    Action = actClose
    Cancel = True
    Caption = '&Close'
    TabOrder = 3
  end
  object edtMargin: TSpinEdit
    Left = 130
    Top = 73
    Width = 70
    Height = 22
    MaxValue = 100
    MinValue = 1
    TabOrder = 4
    Value = 4
  end
  object edtSize: TSpinEdit
    Left = 214
    Top = 73
    Width = 70
    Height = 22
    MaxValue = 100
    MinValue = 1
    TabOrder = 5
    Value = 3
  end
  object ActionList1: TActionList
    Left = 48
    Top = 144
    object actClose: TAction
      Caption = '&Afsluiten'
      OnExecute = actCloseExecute
    end
    object actSave: TAction
      Caption = 'O&pslaan'
      OnExecute = actSaveExecute
      OnUpdate = actSaveUpdate
    end
    object actCreate: TAction
      Caption = 'Aa&nmaken'
      OnExecute = actCreateExecute
      OnUpdate = actCreateUpdate
    end
    object actCopy: TAction
      Caption = 'Kopieren naar het klembord'
      OnExecute = actCopyExecute
      OnUpdate = actCopyUpdate
    end
  end
  object SaveDialog: TSaveDialog
    DefaultExt = 'bmp'
    Filter = 
      'Windows Bitmap (*.bmp)|*.bmp|JPEG file (*.jpg)|*.jpg|PNG file (*' +
      '.png)|*.png'
    Title = 'Bewaren als afbeelding'
    Left = 16
    Top = 144
  end
  object PopupMenu1: TPopupMenu
    Left = 80
    Top = 144
    object Opslaan1: TMenuItem
      Action = actSave
    end
    object Kopierennaarhetklembord1: TMenuItem
      Action = actCopy
    end
  end
end
