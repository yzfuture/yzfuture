unit duNFCReader;

interface

uses
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Winapi.Messages, System.Classes,
  syncobjs, SysUtils, Winapi.Windows, jpeg;

const
  cstVnation1: array[1..56] of string = ('汉', '蒙古', '回', '藏', '维吾尔', '苗', '彝', '壮', '布依', '朝鲜', '满', '侗', '瑶', '白', '土家', '哈尼', '哈萨克', '傣', '黎', '傈僳', '佤', '畲', '高山', '拉祜', '水', '东乡', '纳西', '景颇', '柯尔克孜', '土', '达斡尔', '仫佬', '羌', '布朗', '撒拉', '毛南', '仡佬', '锡伯', '阿昌', '普米', '塔吉克', '怒', '乌孜别克', '俄罗斯', '鄂温克', '德昂', '保安', '裕固', '京', '塔塔尔', '独龙', '鄂伦春', '赫哲', '门巴', '珞巴', '基诺');

  cstsex: array[0..2] of string = ('未知', '男', '女');
  DllName = 'readCardInfo.dll';

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

  pTwoIdInfoStruct = ^TTwoIdInfoStruct;




procedure cardReadInit(); stdcall; external DllName;// 初始化操作，最开始程序构造的时候调用

procedure cardReadUninit(); stdcall; external DllName;// 反初始化操作，最后程序析构的时候调用// 初始化操作，最开始程序构造的时候调用

function loginCardServer(szServerIp: PAnsiChar; nServerPort: Int32;szAppkey: PAnsiChar; szAppSecret: PAnsiChar;userData: PAnsiChar;nerr: PInteger): Boolean; stdcall; external DllName; // 登录服务器

procedure logoutCardServer(); stdcall; external DllName; // 登出

function cardOpenDevice(unRecvTimeOut: Integer;nerr: PInteger; nDeviceNo: Integer): Integer; stdcall; external DllName;

function cardReadTwoCard(nDeviceHandle: Integer; cardReadProgress: TcardReadProgress; cardinfo: PTwoIdInfoStruct): Boolean; stdcall; external DllName;

procedure setDeviceType( nDeviceType:Int32)stdcall; external DllName;


function cardBeep(nDeviceHandle: Integer): Boolean; stdcall; external DllName;

procedure cardCloseDevice(nDeviceHandle: Integer); stdcall; external DllName;

function cardGetCardDN(nDeviceHandle: Integer; szsn: PAnsiChar; nlen: PInteger): Boolean; stdcall; external DllName;

function cardGetCardSN(nDeviceHandle: Integer; szsn: PAnsiChar; nlen: PInteger): Boolean; stdcall; external DllName;

function cardGetLastError(nDeviceHandle: Integer; nlen: PInteger): PAnsiChar; stdcall; external DllName;

function setCardType(nDeviceHandle: Integer; cardType: Integer): Boolean; stdcall; external DllName;

function getCardNo(nDeviceHandle: Integer; arrcardNo: PAnsiChar; nlen: PInteger): Boolean; stdcall; external DllName;

function cardFindCard(nDeviceHandle: Integer; Bmove: PBoolean): Boolean; stdcall; external DllName;

function Wlt2Bmp(szWltData: PByte; szBmpData: PByte): Integer; stdcall; external 'WltRS.dll';



implementation




initialization

finalization


end.

