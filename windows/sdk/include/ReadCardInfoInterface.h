#pragma once
#include "Type.h"

typedef void(__stdcall *cardReadProgress)(unsigned int nProgress, YZWLHandle nhandle);

typedef enum _eDeviceType
{
	yzwlType = 0,
	sdtapiType = 1
}eDeviceType;

void __stdcall cardReadInit(); // ��ʼ���������ʼ�������ʱ�����

void __stdcall setDeviceType(int nDeviceType);

bool __stdcall loginCardServerEx(char* szip, int nport, int &nerr);
bool __stdcall loginCardServer(char* szip, int nport, char* szAppkey, char* szAppSecret, char* szAppUserId, int &nerr);
YZWLHandle __stdcall cardOpenDevice(int nouttime, int &nerr, int nDeviceNo = 0);

bool __stdcall cardReadTwoCard(YZWLHandle nDeviceHandle, cardReadProgress cardCB, TwoIdInfoStructEx &cardinfo);
bool __stdcall cardReadTwoCardEx(YZWLHandle nDeviceHandle, cardReadProgress cardCB, CardInfoStruct& cardinfo);

const char* __stdcall cardGetSerialNumber(YZWLHandle nDeviceHandle);

bool __stdcall cardGetCardDN(YZWLHandle nDeviceHandle, char* szsn, int &nlen);
bool __stdcall cardGetCardSN(YZWLHandle nDeviceHandle, char* szsn, int &nlen);

bool __stdcall cardBeep(YZWLHandle nDeviceHandle);

bool __stdcall setCardType(YZWLHandle nDeviceHandle, cardType ctype);
bool __stdcall getCardNo(YZWLHandle nDeviceHandle, char* arrcardNo, int &nlen);
bool __stdcall cardFindCard(YZWLHandle nDeviceHandle, bool &bmove);
bool __stdcall cardSelectCard(YZWLHandle nDeviceHandle);

void __stdcall cardCloseDevice(YZWLHandle nDeviceHandle);

int __stdcall	cardGetLastErrorCode(YZWLHandle nDeviceHandle);
const char*	__stdcall cardGetErrorInfo(YZWLHandle nDeviceHandle, int &nlen, int nErrorCode);
const char*	__stdcall cardGetLastError(YZWLHandle nDeviceHandle, int &nlen);

void __stdcall convertCardInfoToAnsi(TwoIdInfoStructEx &cardinfo);
void __stdcall convertCardInfoToAnsiEx(CardInfoStruct &cardinfo);

// �������֤ͷ��
// srcimage���ȹ̶�Ϊ1024 
// outimage��С�������Լ����룬��С��С��40KB���������֤ͷ��bmp��ʽ
// outlen����ʱΪoutimageʵ�ʴ�С������ʱΪʵ��ͼƬ��С
bool __stdcall decodeCardImage(unsigned char* srcimage, char* outimage, int& outlen);

// �ϳ����֤ͼƬ������
// outimage��С�������Լ����룬��С��С��200KB���������֤ͼƬ������bmp��ʽ
// outlen����ʱΪoutimageʵ�ʴ�С������ʱΪʵ��ͼƬ��С
bool __stdcall psInfoToImage(CardInfoStruct cardinfo, char* outimage, int& outlen);

void __stdcall logoutCardServer();

void __stdcall cardReadUninit(); // ����ʼ��������������������ʱ�����
