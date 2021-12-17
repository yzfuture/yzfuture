unit duNFCReader;

interface

uses
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Winapi.Messages, System.Classes,
  syncobjs, SysUtils, Winapi.Windows, jpeg;

const
  cstVnation1: array[1..56] of string = ('��', '�ɹ�', '��', '��', 'ά���', '��', '��', '׳', '����', '����', '��', '��', '��', '��', '����', '����', '������', '��', '��', '����', '��', '�', '��ɽ', '����', 'ˮ', '����', '����', '����', '�¶�����', '��', '���Ӷ�', '����', 'Ǽ', '����', '����', 'ë��', '����', '����', '����', '����', '������', 'ŭ', '���α��', '����˹', '���¿�', '�°�', '����', 'ԣ��', '��', '������', '����', '���״�', '����', '�Ű�', '���', '��ŵ');

  cstsex: array[0..2] of string = ('δ֪', '��', 'Ů');
  DllName = 'readCardInfo.dll';

type
  TcardReadProgress = procedure(nProgress: Integer); stdcall;



    // ����֤��Ϣ�ṹ
  TTwoIdInfoStruct = packed record
    arrTwoIdName: array[0..14] of WideChar; // ���� UNICODE
    arrTwoIdSex: WideChar; // �Ա� UNICODE
    arrTwoIdNation: array[0..1] of WideChar; // ���� UNICODE
    arrTwoIdBirthday: array[0..7] of WideChar; // �������� UNICODE YYYYMMDD
    arrTwoIdAddress: array[0..34] of WideChar; // סַ UNICODE
    arrTwoIdNo: array[0..17] of WideChar; // ���֤���� UNICODE
    arrTwoIdSignedDepartment: array[0..14] of WideChar; // ǩ������ UNICODE
    arrTwoIdValidityPeriodBegin: array[0..7] of WideChar; // ��Ч����ʼ���� UNICODE YYYYMMDD
    arrTwoIdValidityPeriodEnd: array[0..7] of WideChar; // ��Ч�ڽ�ֹ���� UNICODE YYYYMMDD ��Ч��Ϊ����ʱ�洢�����ڡ�

    arrTwoOtherNO: array[0..9 - 1] of WideChar; // ͨ��֤�����
    arrTwoSignNum: array[0..2 - 1] of WideChar;		// ǩ������
    arrTwoRemark1: array[0..3 - 1] of WideChar;	// Ԥ����
    arrTwoType: array[0..1 - 1] of WideChar;		// ֤�����ͱ�ʶ
    arrTwoRemark2: array[0..3 - 1] of WideChar;	// Ԥ����

    arrTwoIdNewAddress: array[0..34] of WideChar; // ����סַ UNICODE
    arrReserve: array[0..1] of Byte; // �����ֽ� �ֽڶ�����
    arrTwoIdPhoto: array[0..1023] of Byte; // ��Ƭ��Ϣ
    arrTwoIdFingerprint: array[0..1023] of Byte; // ָ����Ϣ
    arrTwoIdPhotoJpeg: array[0..4095] of Byte; // ��Ƭ��Ϣ JPEG ��ʽ
    unTwoIdPhotoJpegLength: DWORD; // ��Ƭ��Ϣ���� JPEG��ʽ
  end;

  pTwoIdInfoStruct = ^TTwoIdInfoStruct;




procedure cardReadInit(); stdcall; external DllName;// ��ʼ���������ʼ�������ʱ�����

procedure cardReadUninit(); stdcall; external DllName;// ����ʼ��������������������ʱ�����// ��ʼ���������ʼ�������ʱ�����

function loginCardServer(szServerIp: PAnsiChar; nServerPort: Int32;szAppkey: PAnsiChar; szAppSecret: PAnsiChar;userData: PAnsiChar;nerr: PInteger): Boolean; stdcall; external DllName; // ��¼������

procedure logoutCardServer(); stdcall; external DllName; // �ǳ�

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

