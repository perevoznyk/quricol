program QRGenerator;

uses
  Forms,
  frm_Main in 'frm_Main.pas' {frmMain},
  QuricolCode in '..\Source\QuricolCode.pas',
  QuricolAPI in '..\Source\QuricolAPI.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.Title := 'QR Code generator';
  Application.CreateForm(TfrmMain, frmMain);
  Application.Run;
end.
