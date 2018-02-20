unit frm_Main;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, StdCtrls, ActnList, JPeg, Menus, ClipBrd, Spin;

type
  TfrmMain = class(TForm)
    Shape1: TShape;
    edtText: TEdit;
    Label1: TLabel;
    Bevel1: TBevel;
    btnCreate: TButton;
    btnSave: TButton;
    btnClose: TButton;
    ActionList1: TActionList;
    actClose: TAction;
    actSave: TAction;
    actCreate: TAction;
    SaveDialog: TSaveDialog;
    imgCode: TImage;
    PopupMenu1: TPopupMenu;
    Opslaan1: TMenuItem;
    actCopy: TAction;
    Kopierennaarhetklembord1: TMenuItem;
    edtMargin: TSpinEdit;
    Label2: TLabel;
    edtSize: TSpinEdit;
    Label3: TLabel;
    procedure actSaveUpdate(Sender: TObject);
    procedure actCreateUpdate(Sender: TObject);
    procedure actCloseExecute(Sender: TObject);
    procedure actCreateExecute(Sender: TObject);
    procedure actSaveExecute(Sender: TObject);
    procedure actCopyUpdate(Sender: TObject);
    procedure actCopyExecute(Sender: TObject);
    procedure FormCreate(Sender: TObject);
  private
    { Private declarations }
    procedure UpdateImage;
  public
    { Public declarations }
  end;

var
  frmMain: TfrmMain;

implementation

uses QuricolCode, ShlObj;

{$R *.dfm}

function GetMyPicturesFolder : string;
var
  Path : array[0..255] of char;
begin
  SHGetSpecialFolderPath(0, @Path[0], CSIDL_MYPICTURES, true);
  Result := Path;
end;

procedure TfrmMain.actCloseExecute(Sender: TObject);
begin
  Close;
end;

procedure TfrmMain.actCopyExecute(Sender: TObject);
var
  B : TBitmap;
begin
  B := TQRCode.GetBitmapImage(edtText.Text, edtMargin.Value, edtSize.Value);
  Clipboard.Assign(B);
  B.Free;
end;

procedure TfrmMain.actCopyUpdate(Sender: TObject);
begin
  actCopy.Enabled := (not imgCode.Picture.Bitmap.Empty);
end;

procedure TfrmMain.actCreateExecute(Sender: TObject);
begin
  UpdateImage;
end;

procedure TfrmMain.actCreateUpdate(Sender: TObject);
begin
  actCreate.Enabled := edtText.Text <> '';
end;

procedure TfrmMain.actSaveExecute(Sender: TObject);
var
  Ext : string;
  Jpg : TJPEGImage;
  B : TBitmap;
begin
  if SaveDialog.Execute then
   begin
     Ext := ExtractFileExt(SaveDialog.FileName);
     if SameText(ext, '.png') then
       TQRCode.GeneratePngFile(SaveDialog.FileName, edtText.Text, edtMargin.Value, edtSize.Value)
     else if SameText(ext, '.bmp') then
       TQRCode.GenerateBitmapFile(SaveDialog.FileName, edtText.Text, edtMargin.Value, edtSize.Value)
     else
       begin
         Jpg := TJPEGImage.Create;
         B := TQRCode.GetBitmapImage(edtText.Text, edtMargin.Value, edtSize.Value);
         Jpg.Assign(B);
         B.Free;
         Jpg.SaveToFile(SaveDialog.FileName);
         Jpg.Free;
       end;
   end;
end;

procedure TfrmMain.actSaveUpdate(Sender: TObject);
begin
  actSave.Enabled := (edtText.Text <> '') and (not imgCode.Picture.Bitmap.Empty);
end;

procedure TfrmMain.FormCreate(Sender: TObject);
begin
  SaveDialog.InitialDir := GetMyPicturesFolder;
end;

procedure TfrmMain.UpdateImage;
 var
  B : TBitmap;
begin
  B := TQRCode.GetBitmapImage(edtText.Text, edtMargin.Value, edtSize.Value);
  imgCode.Picture.Bitmap.Assign(B);
  B.Free;
end;


end.
