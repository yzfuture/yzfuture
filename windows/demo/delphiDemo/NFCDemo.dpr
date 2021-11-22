program NFCDemo;

uses
  Vcl.Forms,
  fmMain in 'fmMain.pas' {Form4},
  duNFCReader in 'duNFCReader.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TForm4, Form4);
  Application.Run;
end.
