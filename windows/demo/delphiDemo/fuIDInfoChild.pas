unit fuIDInfoChild;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes,
  Vcl.Graphics, Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.Imaging.pngimage, Vcl.ExtCtrls,
  Vcl.StdCtrls;

type
  TfmIDInfoChild = class(TForm)
    imgIDInfo: TImage;
    lbName: TLabel;
    lbSex: TLabel;
    lbNation: TLabel;
    lbYear: TLabel;
    lbMonth: TLabel;
    lbDay: TLabel;
    lbAddress: TLabel;
    lbIDNum: TLabel;
    ImageIDCardPhoto: TImage;
    lbDept: TLabel;
    lbValidityDate: TLabel;
  private
    { Private declarations }
  public
    { Public declarations }
    procedure ClearUI();
    procedure LoadIDInfo(pIDInfo: Pointer);
  end;

implementation

{$R *.dfm}
uses
  duNFCReader;

{ TfmIDInfoChild }

procedure TfmIDInfoChild.ClearUI;
begin
  lbName.Caption := '';
  lbSex.Caption := '';
  lbNation.Caption := '';
  lbYear.Caption := '';
  lbMonth.Caption := '';
  lbDay.Caption := '';
  lbAddress.Caption := '';
  lbIDNum.Caption := '';
  ImageIDCardPhoto.Visible := False;
  if ImageIDCardPhoto.Picture <> nil then
    ImageIDCardPhoto.Picture := nil;
  ImageIDCardPhoto.Visible := True;
  lbDept.Caption := '';
  lbValidityDate.Caption := '';
end;

procedure TfmIDInfoChild.LoadIDInfo(pIDInfo: Pointer);
var
  arrBmpBuffer: array[0..38861] of Byte;
  AReadIDCardInfo: pYZWLReadIDCardInfo;
  ATmpStr: string;
  AIdValidityDate: string;
  memTmp: TMemoryStream;
begin
  AReadIDCardInfo := pYZWLReadIDCardInfo(pIDInfo);
  lbName.Caption := Trim(AReadIDCardInfo.sttTwoIdInfo.arrTwoIdName);
  lbSex.Caption := cstsex[StrToInt(Trim(AReadIDCardInfo.sttTwoIdInfo.arrTwoIdSex))]; // 性别
  ATmpStr := Trim(AReadIDCardInfo.sttTwoIdInfo.arrTwoIdNation);
  if StrToIntDef(ATmpStr, 0) in [1..56] then
  begin
    lbNation.Caption := cstVnation1[StrToInt(ATmpStr)];
  end
  else
  begin
    lbNation.Caption := '其它';
  end;

  ATmpStr := Trim(AReadIDCardInfo.sttTwoIdInfo.arrTwoIdBirthday);
  lbYear.Caption := Trim(Copy(ATmpStr, 1, 4));
  lbMonth.Caption := Trim(Copy(ATmpStr, 5, 2));
  lbDay.Caption := Trim(Copy(ATmpStr, 7, 2));
  lbAddress.Caption := Trim(AReadIDCardInfo.sttTwoIdInfo.arrTwoIdAddress);
  lbIDNum.Caption := Trim(AReadIDCardInfo.sttTwoIdInfo.arrTwoIdNo);
  lbDept.Caption := Trim(AReadIDCardInfo.sttTwoIdInfo.arrTwoIdSignedDepartment);
  ATmpStr := Trim(AReadIDCardInfo.sttTwoIdInfo.arrTwoIdValidityPeriodBegin);
  AIdValidityDate := Copy(ATmpStr, 1, 4) + '.' + Copy(ATmpStr, 5, 2) + '.' + Copy(ATmpStr, 7, 2);
  ATmpStr := Trim(AReadIDCardInfo.sttTwoIdInfo.arrTwoIdValidityPeriodEnd);
  if Length(ATmpStr) = 8 then
  begin
    AIdValidityDate := AIdValidityDate + '-' + Copy(ATmpStr, 1, 4) + '.' + Copy(ATmpStr, 5, 2) + '.' + Copy(ATmpStr, 7, 2);
  end
  else
    AIdValidityDate := AIdValidityDate + '-' + ATmpStr;

  lbValidityDate.Caption := AIdValidityDate;

  FillChar(arrBmpBuffer, SizeOf(arrBmpBuffer), 0);
  if TIDReadCard.WltToBmp(@AReadIDCardInfo.sttTwoIdInfo.arrTwoIdPhoto, @arrBmpBuffer) > 0 then
  begin

    memTmp := TMemoryStream.Create;
    try
      memTmp.SetSize(SizeOf(arrBmpBuffer));
      memTmp.WriteBuffer(arrBmpBuffer, SizeOf(arrBmpBuffer));
      memTmp.Seek(0, soFromBeginning);
      ImageIDCardPhoto.Picture.Bitmap.LoadFromStream(memTmp);
    finally
      memTmp.Free;
    end;
    ImageIDCardPhoto.Picture.Bitmap.transparent := True;
    ImageIDCardPhoto.Picture.Bitmap.transparentcolor := ImageIDCardPhoto.Picture.Bitmap.Canvas.pixels[1, 1];
  end
  else
  begin
  //  Memo1.Lines.Add('图片解密失败！');
  end;
end;

end.

