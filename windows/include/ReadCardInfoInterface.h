#pragma once
#include "Type.h"

typedef void(__stdcall *cardReadProgress)(unsigned int nProgress, YZWLHandle nhandle);

YZWLHandle __stdcall cardOpenDevice(int nouttime, int nDeviceNo = 0);
bool __stdcall setCardType(YZWLHandle nDeviceHandle, cardType ctype);
bool __stdcall cardFindCard(YZWLHandle nDeviceHandle, bool &bmove);

bool __stdcall cardGetCardDN(YZWLHandle nDeviceHandle, char* szsn, int &nlen);
bool __stdcall cardGetCardSN(YZWLHandle nDeviceHandle, char* szsn, int &nlen);

bool __stdcall cardGetDeviceNO(YZWLHandle nDeviceHandle, char* szno, int &nlen);
bool __stdcall cardGetDeviceSN(YZWLHandle nDeviceHandle, char* szsn, int &nlen);

bool __stdcall cardReadTwoCard(YZWLHandle nDeviceHandle, cardReadProgress cardCB, 
	char* szFactoryFlag, char* szServerIp, int nServerPort,
	TwoIdInfoStructEx &cardinfo, bool bTest);

const char* __stdcall cardGetSerialNumber(YZWLHandle nDeviceHandle);

bool __stdcall cardBeep(YZWLHandle nDeviceHandle);

void __stdcall cardCloseDevice(YZWLHandle nDeviceHandle);

int __stdcall	cardGetLastErrorCode(YZWLHandle nDeviceHandle);
const char*	__stdcall cardGetErrorInfo(YZWLHandle nDeviceHandle, int &nlen, int nErrorCode);
const char*	__stdcall cardGetLastError(YZWLHandle nDeviceHandle, int &nlen);

// srcimage长度固定为1024 
// outimage大小由外面自己申请，大小不小于40KB
// outlen传入时为outimage实际大小，传出时为实际图片大小
bool __stdcall decodeCardImage(unsigned char* srcimage, char* outimage, int& outlen);

//注册相关
int	__stdcall	regeditGetVerificationCode(char* szserverip, unsigned int nserverport,
	char* szphoto, char* szError, int &nError);
int	__stdcall	regeditGetLogonAppKey(char* szserverip, unsigned int nserverport, char* szphoto,
	char* szverCode, char* szuserName, char* szret, int &nret, char* szError, int &nError);
int	__stdcall	regeditGetAppKeyUseNum(char* szserverip, unsigned int nserverport,
	char* szAppkey, char* szError, int &nError);
