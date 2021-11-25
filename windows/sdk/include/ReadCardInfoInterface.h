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

YZWLHandle __stdcall cardOpenDevice(char* szAppkey, char* szAppSecret, char* szip, int nport, char* userData, int nouttime, int &nerr, int nDeviceNo = 0);

bool __stdcall cardReadTwoCard(YZWLHandle nDeviceHandle, cardReadProgress cardCB, TwoIdInfoStructEx &cardinfo);

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
void __stdcall convertCardInfoToUncode(TwoIdInfoStructEx &cardinfo);

// srcimage���ȹ̶�Ϊ1024 
// outimage��С�������Լ����룬��С��С��40KB
// outlen����ʱΪoutimageʵ�ʴ�С������ʱΪʵ��ͼƬ��С
bool __stdcall decodeCardImage(unsigned char* srcimage, char* outimage, int& outlen);


void __stdcall cardReadUninit(); // ����ʼ��������������������ʱ�����
