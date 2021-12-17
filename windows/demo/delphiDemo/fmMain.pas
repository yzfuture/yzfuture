unit fmMain;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, duNFCReader,
  Vcl.ComCtrls, Vcl.Samples.Spin, Vcl.StdCtrls, Vcl.Controls, Vcl.ExtCtrls,
  Vcl.Imaging.pngimage, System.Classes, Vcl.Graphics, Vcl.Forms, Vcl.Dialogs;

type
  TForm4 = class(TForm)
    imgIDInfo: TImage;
    ImageIDCardPhoto: TImage;
    lbValidityDate: TLabel;
    lbDept: TLabel;
    lbIDNum: TLabel;
    lbAddress: TLabel;
    lbDay: TLabel;
    lbMonth: TLabel;
    lbYear: TLabel;
    lbNation: TLabel;
    lbSex: TLabel;
    lbName: TLabel;
    btnRead: TButton;
    radbtn: TRadioGroup;
    lbAppKey: TLabel;
    lbAppSecret: TLabel;
    lbAppUserData: TLabel;
    edtAppKey: TEdit;
    edtAppSecret: TEdit;
    edtAppUserData: TEdit;
    meoLog: TMemo;
    edtTimeOut: TSpinEdit;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    edtReadIndex: TSpinEdit;
    ProgressBar1: TProgressBar;
    Label4: TLabel;
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure btnReadClick(Sender: TObject);
    procedure radbtnClick(Sender: TObject);
  private
    { Private declarations }
    procedure WriteLog(ALog: string);
    procedure LoadIDInfo(pIDInfo: pTwoIdInfoStruct);
  public

    { Public declarations }
  end;

var
  Form4: TForm4;

implementation



{$R *.dfm}

procedure cardReadProgress(nProgress: Integer); stdcall;
begin
  Form4.ProgressBar1.Position := nProgress;
end;
//

procedure TForm4.LoadIDInfo(pIDInfo: pTwoIdInfoStruct);
var
  arrBmpBuffer: array[0..38861] of Byte;
  ATmpStr: string;
  AIdValidityDate: string;
  memTmp: TMemoryStream;
begin
  lbName.Caption := Trim(pIDInfo.arrTwoIdName);
  lbSex.Caption := cstsex[StrToInt(Trim(pIDInfo.arrTwoIdSex))]; // 性别
  ATmpStr := Trim(pIDInfo.arrTwoIdNation);
  if StrToIntDef(ATmpStr, 0) in [1..56] then
  begin
    lbNation.Caption := cstVnation1[StrToInt(ATmpStr)];
  end
  else
  begin
    lbNation.Caption := '其它';
  end;

  ATmpStr := Trim(pIDInfo.arrTwoIdBirthday);
  lbYear.Caption := Trim(Copy(ATmpStr, 1, 4));
  lbMonth.Caption := Trim(Copy(ATmpStr, 5, 2));
  lbDay.Caption := Trim(Copy(ATmpStr, 7, 2));
  lbAddress.Caption := Trim(pIDInfo.arrTwoIdAddress);
  lbIDNum.Caption := Trim(pIDInfo.arrTwoIdNo);
  lbDept.Caption := Trim(pIDInfo.arrTwoIdSignedDepartment);
  ATmpStr := Trim(pIDInfo.arrTwoIdValidityPeriodBegin);
  AIdValidityDate := Copy(ATmpStr, 1, 4) + '.' + Copy(ATmpStr, 5, 2) + '.' + Copy(ATmpStr, 7, 2);
  ATmpStr := Trim(pIDInfo.arrTwoIdValidityPeriodEnd);
  if Length(ATmpStr) = 8 then
  begin
    AIdValidityDate := AIdValidityDate + '-' + Copy(ATmpStr, 1, 4) + '.' + Copy(ATmpStr, 5, 2) + '.' + Copy(ATmpStr, 7, 2);
  end
  else
    AIdValidityDate := AIdValidityDate + '-' + ATmpStr;

  lbValidityDate.Caption := AIdValidityDate;

  FillChar(arrBmpBuffer, SizeOf(arrBmpBuffer), 0);

  if Wlt2Bmp(@pIDInfo.arrTwoIdPhoto, @arrBmpBuffer) > 0 then
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
    WriteLog('图片解密失败！');
  end;
end;

procedure TForm4.radbtnClick(Sender: TObject);
begin
  if radbtn.ItemIndex = 0 then
  begin
    edtAppKey.Enabled := True;
    edtAppSecret.Enabled := True;
    edtAppUserData.Enabled := True;
    lbAppKey.Enabled := True;
    lbAppSecret.Enabled := True;
    lbAppUserData.Enabled := True;
    edtReadIndex.Value := 0;
  end
  else
  begin
    edtAppKey.Enabled := False;
    edtAppSecret.Enabled := False;
    edtAppUserData.Enabled := False;
    lbAppKey.Enabled := False;
    lbAppSecret.Enabled := False;
    lbAppUserData.Enabled := False;
    edtReadIndex.Value := 1001;
  end;
end;

procedure TForm4.btnReadClick(Sender: TObject);
var
  AIdInfoStruct: pTwoIdInfoStruct;
  AStart: DWORD;
  ADeviceHandle: Integer;
  memTmp: TMemoryStream;
  ANewIDCardSN: array[0..48 - 1] of AnsiChar;
  AIDSNLen: Integer;
  AErrCode:Integer;
  AMove: Boolean;
  szAppKey, szAppSecret, szAppUserData: AnsiString;
var
  strIDLen: Integer;
begin
  szAppKey := Trim(edtAppKey.Text);
  szAppSecret := Trim(edtAppSecret.Text);
  szAppUserData := Trim(edtAppUserData.Text);

  ProgressBar1.Position := 0;

  // 登录只做展示用，请自己按需要加入到程序逻辑中
  if not loginCardServer('id.yzfuture.cn', 443, PAnsiChar(szAppKey), PAnsiChar(szAppSecret),PAnsiChar(szAppUserData), @AErrCode) then
  begin
     WriteLog('登录失败！');
     Exit;
  end;

  if radbtn.ItemIndex = 0 then //标准读卡器
  begin
    setDeviceType(0);
  end
  else
  begin
    setDeviceType(1);
  end;

  ADeviceHandle := cardOpenDevice(edtTimeOut.Value, @AErrCode, edtReadIndex.Value);
  if ADeviceHandle <= 0 then
  begin
    WriteLog('打开设备失败!');
    Exit;
  end;
  try
    if not setCardType(ADeviceHandle, 1) then
    begin

      WriteLog('设置卡类型失败！');

      Exit;
    end;

    if not cardFindCard(ADeviceHandle, @AMove) then
    begin

      WriteLog('寻卡失败，未找到身份证！');

      Exit;
    end;

    AIDSNLen := SizeOf(ANewIDCardSN);

    if not cardGetCardSN(ADeviceHandle, ANewIDCardSN, @AIDSNLen) then
    begin

      WriteLog('读取身份证SN失败');
      Exit;
    end;

    GetMem(AIdInfoStruct, SizeOf(TTwoIdInfoStruct));
    FillChar(AIdInfoStruct^, SizeOf(TTwoIdInfoStruct), 0);

    try
      if cardReadTwoCard(ADeviceHandle, @cardReadProgress, AIdInfoStruct) then //读卡成功。
      begin
        ProgressBar1.Position := ProgressBar1.Max;
        cardBeep(ADeviceHandle);     //读卡器蜂鸣
        LoadIDInfo(AIdInfoStruct);
        WriteLog('读卡成功!');
      end
      else
      begin
        WriteLog('读卡失败!');
      end;

    finally

      FreeMem(AIdInfoStruct, SizeOf(TTwoIdInfoStruct));
    end;

  finally

    if (ADeviceHandle > 0) then
    begin
      cardCloseDevice(ADeviceHandle);

    end;

  end;

end;

procedure TForm4.FormCreate(Sender: TObject);
begin
  cardReadInit;   //初始化
  meoLog.Clear;
end;

procedure TForm4.FormDestroy(Sender: TObject);
begin
  logoutCardServer;//登出
  cardReadUninit;  //反初始化
end;

procedure TForm4.WriteLog(ALog: string);
begin
  meoLog.Lines.Add(FormatDateTime('YYYY-MM-DD hh:nn:ss zzz', Now) + ':' + ALog);
  meoLog.SelStart := Length(meoLog.Text);
end;

end.

