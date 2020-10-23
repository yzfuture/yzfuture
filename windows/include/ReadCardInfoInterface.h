#pragma once
#include "Type.h"

typedef void(__stdcall *cardReadProgress)(unsigned int nProgress, YZWLHandle nhandle);

YZWLHandle __stdcall cardOpenDevice(int nouttime, int nDeviceNo = 0);
YZWLHandle __stdcall cardOpenDeviceEx(int nouttime, int nDeviceNo = 0, readCardType etype = readCardType::crulType);
YZWLHandle __stdcall cardOpenDevice_V1(int nouttime, userApduCommand apducb, void* userdata, int nDeviceNo = 0);
YZWLHandle __stdcall cardOpenDevice_Socket(int nouttime, int nsocket);

bool __stdcall cardReadTwoCard(YZWLHandle nDeviceHandle, cardReadProgress cardCB, 
	char* szFactoryFlag, char* szServerIp, int nServerPort,
	TwoIdInfoStructEx &cardinfo, bool bTest);

const char* __stdcall cardGetSerialNumber(YZWLHandle nDeviceHandle);

bool __stdcall cardGetCardDN(YZWLHandle nDeviceHandle, char* szsn, int &nlen);
bool __stdcall cardGetCardSN(YZWLHandle nDeviceHandle, char* szsn, int &nlen);

bool __stdcall cardGetDeviceNO(YZWLHandle nDeviceHandle, char* szno, int &nlen);
bool __stdcall cardGetDeviceSN(YZWLHandle nDeviceHandle, char* szsn, int &nlen);

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

// srcimage长度固定为1024 
// outimage大小由外面自己申请，大小不小于40KB
// outlen传入时为outimage实际大小，传出时为实际图片大小
bool __stdcall decodeCardImage(unsigned char* srcimage, char* outimage, int& outlen);

bool __stdcall readEPassportInfo(YZWLHandle nDeviceHandle, char* szFactoryFlag, 
	char* szServerIp, int nServerPort, char* szNO, char* szBirth,
	char* szEndtime, EPassportInfoStruct &cardinfo, bool bTest);

//注册相关
int	__stdcall	regeditGetVerificationCode(char* szserverip, unsigned int nserverport,
	char* szphoto, char* szError, int &nError);
int	__stdcall	regeditGetLogonAppKey(char* szserverip, unsigned int nserverport, char* szphoto,
	char* szverCode, char* szuserName, char* szret, int &nret, char* szError, int &nError);
int	__stdcall	regeditFindLogonAppKey(char* szserverip, unsigned int nserverport, char* szphoto,
	char* szverCode, char* szret, int &nret, char* szError, int &nError);
int	__stdcall	regeditGetAppKeyUseNum(char* szserverip, unsigned int nserverport,
	char* szAppkey, char* szError, int &nError);
