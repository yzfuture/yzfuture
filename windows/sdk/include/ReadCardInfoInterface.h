#pragma once
#include "Type.h"

typedef void(__stdcall *cardReadProgress)(unsigned int nProgress, YZWLHandle nhandle);

typedef enum _eDeviceType
{
	yzwlType = 0,
	sdtapiType = 1
}eDeviceType;

void __stdcall cardReadInit(); // 初始化操作，最开始程序构造的时候调用

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

// 解码身份证头像
// srcimage长度固定为1024 
// outimage大小由外面自己申请，大小不小于40KB，生成身份证头像bmp格式
// outlen传入时为outimage实际大小，传出时为实际图片大小
bool __stdcall decodeCardImage(unsigned char* srcimage, char* outimage, int& outlen);

// 合成身份证图片正反面
// outimage大小由外面自己申请，大小不小于200KB，生成身份证图片正反面bmp格式
// outlen传入时为outimage实际大小，传出时为实际图片大小
bool __stdcall psInfoToImage(CardInfoStruct cardinfo, char* outimage, int& outlen);

void __stdcall logoutCardServer();

void __stdcall cardReadUninit(); // 反初始化操作，最后程序析构的时候调用
