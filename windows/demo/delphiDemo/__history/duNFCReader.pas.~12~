unit duNFCReader;

interface

uses
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Winapi.Messages, System.Classes,
  syncobjs, SysUtils, Winapi.Windows, jpeg;

const
  cstVnation1: array[1..56] of string = ('汉', '蒙古', '回', '藏', '维吾尔', '苗', '彝', '壮', '布依', '朝鲜', '满', '侗', '瑶', '白', '土家', '哈尼', '哈萨克', '傣', '黎', '傈僳', '佤', '畲', '高山', '拉祜', '水', '东乡', '纳西', '景颇', '柯尔克孜', '土', '达斡尔', '仫佬', '羌', '布朗', '撒拉', '毛南', '仡佬', '锡伯', '阿昌', '普米', '塔吉克', '怒', '乌孜别克', '俄罗斯', '鄂温克', '德昂', '保安', '裕固', '京', '塔塔尔', '独龙', '鄂伦春', '赫哲', '门巴', '珞巴', '基诺');
 // cstVnation3: array[97..99] of string = ('其它', '外国血统', '未识别99');
  cstsex: array[0..2] of string = ('未知', '男', '女');
  DllName = 'readCardInfo.dll';
 // TicNetErrStr: array[0..7] of string = ('TIEC_RECEIVE_INTERNAL_AUTHENTICATE', 'TIEC_SEND_INTERNAL_AUTHENTICATE', 'TIEC_SEND_RANDOM', 'TIEC_RECEIVE_EXTERNAL_AUTHENTICATE', 'TIEC_RECEIVE_LAST_DATA', 'TIEC_CONNECT_SERVER', 'TIEC_SAMV_BUSY', 'TIEC_READ_SEND_FIFTH_FILE');

type
  TcardReadProgress = procedure(nProgress: Integer); stdcall;



    // 二代证信息结构
  TTwoIdInfoStruct = packed record
    arrTwoIdName: array[0..14] of WideChar; // 姓名 UNICODE
    arrTwoIdSex: WideChar; // 性别 UNICODE
    arrTwoIdNation: array[0..1] of WideChar; // 民族 UNICODE
    arrTwoIdBirthday: array[0..7] of WideChar; // 出生日期 UNICODE YYYYMMDD
    arrTwoIdAddress: array[0..34] of WideChar; // 住址 UNICODE
    arrTwoIdNo: array[0..17] of WideChar; // 身份证号码 UNICODE
    arrTwoIdSignedDepartment: array[0..14] of WideChar; // 签发机关 UNICODE
    arrTwoIdValidityPeriodBegin: array[0..7] of WideChar; // 有效期起始日期 UNICODE YYYYMMDD
    arrTwoIdValidityPeriodEnd: array[0..7] of WideChar; // 有效期截止日期 UNICODE YYYYMMDD 有效期为长期时存储“长期”

    arrTwoOtherNO: array[0..9 - 1] of WideChar; // 通行证类号码
    arrTwoSignNum: array[0..2 - 1] of WideChar;		// 签发次数
    arrTwoRemark1: array[0..3 - 1] of WideChar;	// 预留区
    arrTwoType: array[0..1 - 1] of WideChar;		// 证件类型标识
    arrTwoRemark2: array[0..3 - 1] of WideChar;	// 预留区

    arrTwoIdNewAddress: array[0..34] of WideChar; // 最新住址 UNICODE
    arrReserve: array[0..1] of Byte; // 保留字节 字节对齐用
    arrTwoIdPhoto: array[0..1023] of Byte; // 照片信息
    arrTwoIdFingerprint: array[0..1023] of Byte; // 指纹信息
    arrTwoIdPhotoJpeg: array[0..4095] of Byte; // 照片信息 JPEG 格式
    unTwoIdPhotoJpegLength: DWORD; // 照片信息长度 JPEG格式
  end;

  PTwoIdInfoStruct = ^TTwoIdInfoStruct;

  TYZWLReadIDCardInfo = record
    ReadState: Integer;
    sttTwoIdInfo: TTwoIdInfoStruct;
  end;

  pYZWLReadIDCardInfo = ^TYZWLReadIDCardInfo;

type
  TIDReadCard = class(TThread)
  protected
    procedure Execute; override;
  private
    FLastIDCardSN: string;
 //   FStartedRead: Boolean;
    function CloseDevice(var m_nDeviceHandle: Integer): Boolean;
    function OpenDevice(var m_nDeviceHandle: Integer; unRecvTimeOut: Integer): Boolean;
    function NFC_ReadIDCard(): Boolean;
    procedure SleepEx(ATime: DWORD);
//    procedure SetStartedRead(const Value: Boolean);
  public
//    property StartedRead: Boolean read FStartedRead write SetStartedRead;
    class function WltToBmp(szWltData, szBmpData: PByte): Integer;
    constructor Create(AHandle: THandle);
    destructor Destroy; override;
  end;


//{$IFDEF Debug_TK}
//function cardOpenDevice(unRecvTimeOut: Integer; nDeviceNo: Integer = 0): Integer; stdcall; external DllName;
//{$ELSE}
function cardOpenDevice(unRecvTimeOut: Integer; nDeviceNo: Integer): Integer; stdcall; external DllName;
//{$ENDIF}

//function cardOpenDeviceEx(unRecvTimeOut: Integer; nDeviceNo: Integer = 0; nDeviceType: Integer = 0): Integer; stdcall; external DllName;                                                               bool bEncry, bool twoAuth

//function cardReadTwoCard_V1(nDeviceHandle: Integer; cardReadProgress: TcardReadProgress; szFactoryFlag: PAnsiChar; szServerIp: PAnsiChar; nServerPort: Int32; cardinfo: PTwoIdInfoStruct; bTest: Boolean = True; bEncry: Boolean=false; twoAuth: Boolean=true): Boolean; stdcall; external DllName;

function cardReadTwoCard(nDeviceHandle: Integer; cardReadProgress: TcardReadProgress; szFactoryFlag: PAnsiChar; szServerIp: PAnsiChar; nServerPort: Int32; cardinfo: PTwoIdInfoStruct; bTest: Boolean = False): Boolean; stdcall; external DllName;

function cardBeep(nDeviceHandle: Integer): Boolean; stdcall; external DllName;

procedure cardCloseDevice(nDeviceHandle: Integer); stdcall; external DllName;

function cardGetCardDN(nDeviceHandle: Integer; szsn: PAnsiChar; nlen: PInteger): Boolean; stdcall; external DllName;

function cardGetCardSN(nDeviceHandle: Integer; szsn: PAnsiChar; nlen: PInteger): Boolean; stdcall; external DllName;

function cardGetLastError(nDeviceHandle: Integer; nlen: PInteger): PAnsiChar; stdcall; external DllName;

function setCardType(nDeviceHandle: Integer; cardType: Integer): Boolean; stdcall; external DllName;

function getCardNo(nDeviceHandle: Integer; arrcardNo: PAnsiChar; nlen: PInteger): Boolean; stdcall; external DllName;

function cardFindCard(nDeviceHandle: Integer; Bmove: PBoolean): Boolean; stdcall; external DllName;

function Wlt2Bmp(szWltData: PByte; szBmpData: PByte): Integer; stdcall; external 'WltRS.dll';


//       typedef enum cardType
//{
//  unkwonType = -1,
//  ACardType = 0,
//  BCardType = 1
//};

//bool __stdcall setCardType(LotusHandle nDeviceHandle, cardType ctype);
//bool __stdcall getCardNo(LotusHandle nDeviceHandle, char* arrcardNo, int &nlen);

//function saveAnsiCardInfoToServer(szServerIp: PAnsiChar; nServerPort: Int32; cardinfo: TTwoIdInfoStruct; SN: PAnsiChar; DN: PAnsiChar;bTest: Boolean = True): Boolean; stdcall; external DllName;

implementation

uses
  duType, duGlob;

var
  FHandle: THandle;

procedure cardReadProgress(nProgress: Integer); stdcall;
begin
  SendCustomMessage(PostModel, FHandle, TMSG_ID_ReadIDCard, TMSG_wParam_ReaderProgress, nProgress);
end;

class function TIDReadCard.WltToBmp(szWltData, szBmpData: PByte): Integer;
var
  nResult: Integer;
begin
  nResult := 0;
  nResult := Wlt2Bmp(szWltData, szBmpData);
  Result := nResult;
end;

constructor TIDReadCard.Create(AHandle: THandle);
begin
  FreeOnTerminate := False;
  FHandle := AHandle;
  FLastIDCardSN := '';
  inherited Create(False);

end;

destructor TIDReadCard.Destroy;
begin

end;

procedure TIDReadCard.Execute;
begin
  inherited;
  while not Terminated do
  begin
    NFC_ReadIDCard;
   //Suspended := True;
    SleepEx(1000);
  end;
end;

//procedure TIDReadCard.SetStartedRead(const Value: Boolean);
//begin
//  FStartedRead := Value;
//end;

procedure TIDReadCard.SleepEx(ATime: DWORD);
var
  i, ACount: Integer;
begin
  ACount := ATime div 50;
  for i := 0 to ACount - 1 do
    if not Terminated then
    begin
      Sleep(50);
    end
    else
      Break;
end;

function TIDReadCard.OpenDevice(var m_nDeviceHandle: Integer; unRecvTimeOut: Integer): Boolean;
begin
  m_nDeviceHandle := cardOpenDevice(unRecvTimeOut, 0);
  Result := m_nDeviceHandle > 0;

end;

function TIDReadCard.CloseDevice(var m_nDeviceHandle: Integer): Boolean;
begin
  if (m_nDeviceHandle > 0) then
  begin
    cardCloseDevice(m_nDeviceHandle);
    m_nDeviceHandle := -1;
  end;
end;

function TIDReadCard.NFC_ReadIDCard(): Boolean;
const
  cstIDServerIP = 'id.yzfuture.cn';
  cstIDServerPort = 8848;
  cstIDKey = '99ffb2f98a29071107c7a09ad2c6d096';
var
  AReadIDCardInfo: pYZWLReadIDCardInfo;
  AStart: DWORD;
  ADeviceHandle: Integer;
  memTmp: TMemoryStream;
  ANewIDCardSN: array[0..48 - 1] of AnsiChar;
  AIDSNLen: Integer;
  AMove: Boolean;
begin

  if not OpenDevice(ADeviceHandle, 2) then
  begin
    Exit;
  end;
  try
    if not setCardType(ADeviceHandle, 1) then
    begin
      FLastIDCardSN := '';
   // G_Log.WriteLogDebuFile('调用' + 'setCardType:' + cardGetLastError(F_nDeviceHandle, @strIDLen) + '失败!');

      Exit;
    end;

    if not cardFindCard(ADeviceHandle, @AMove) then
    begin
      FLastIDCardSN := '';
  //  G_Log.WriteLogDebuFile('调用' + 'cardFindCard:' + cardGetLastError(F_nDeviceHandle, @strIDLen) + '失败!');

      Exit;
    end;

    AIDSNLen := SizeOf(ANewIDCardSN);

    if not cardGetCardSN(ADeviceHandle, ANewIDCardSN, @AIDSNLen) then
    begin
      FLastIDCardSN := '';
 //   ANewIDCardSN := '';
      // cardGetLastError(F_nDeviceHandle, @strIDLen) + '失败!');
      Exit;
    end;

    if FLastIDCardSN = string(AnsiString(ANewIDCardSN)) then    //如果是同一个身份证 则不多次读取
    begin
      cardBeep(ADeviceHandle);
      Exit;
    end
    else
    begin
      FLastIDCardSN := string(AnsiString(ANewIDCardSN));
    end;

    SendCustomMessage(PostModel, FHandle, TMSG_ID_ReadIDCard, TMSG_wParam_ReaderProgress, 0); //读卡进度条

    GetMem(AReadIDCardInfo, SizeOf(TYZWLReadIDCardInfo));
    FillChar(AReadIDCardInfo^, SizeOf(TYZWLReadIDCardInfo), 0);

    try
      if cardReadTwoCard(ADeviceHandle, @cardReadProgress, cstIDKey, cstIDServerIP, cstIDServerPort, @AReadIDCardInfo.sttTwoIdInfo) then
      begin
        AReadIDCardInfo.ReadState := 1;
        cardBeep(ADeviceHandle);
      end
      else
        FLastIDCardSN := '';
    finally
      SendCustomMessage(SendModel, FHandle, TMSG_ID_ReadIDCard, TMSG_wParam_ReadIDCard, LPARAM(AReadIDCardInfo)); //读卡完成
      FreeMem(AReadIDCardInfo, SizeOf(TYZWLReadIDCardInfo));
    end;

  finally

    SendCustomMessage(PostModel, FHandle, TMSG_ID_ReadIDCard, TMSG_wParam_ReaderProgress, -1); //隐藏读卡进度条

    CloseDevice(ADeviceHandle);
  end;

end;

initialization

finalization
//  if Assigned(G_IDReader) then
//  begin
//    G_IDReader.Terminate;
//    if G_IDReader.Suspended then
//      G_IDReader.Suspended := False;
//    G_IDReader.WaitFor;
//    G_IDReader.Free;
//  end;

end.

