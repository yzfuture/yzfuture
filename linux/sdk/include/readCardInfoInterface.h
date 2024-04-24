#pragma once
#include "Type.h"

#define FBC_API_PUBLIC __attribute__((visibility ("default")))
#define FBC_API_LOCAL __attribute__((visibility("hidden")))

#ifdef __cplusplus
extern "C" {
#endif

	typedef void(FBC_API_PUBLIC *cardReadProgress)(void* userData, unsigned int nProgress);

	void FBC_API_PUBLIC cardReadInit(); // 初始化操作，最开始程序构造的时候调用
	void FBC_API_PUBLIC setDeviceType(int nDeviceType);

	bool FBC_API_PUBLIC loginCardServerEx(char* szip, int nport, int &nerr);
	YZWLHandle FBC_API_PUBLIC cardOpenDevice(int nouttime, int &nerr, int nDeviceNo = 0);

	bool FBC_API_PUBLIC setCardType(YZWLHandle nDeviceHandle, cardType ctype=BCardType);
	bool FBC_API_PUBLIC cardFindCard(YZWLHandle nDeviceHandle, bool &bmove);

	bool FBC_API_PUBLIC cardGetCardDN(YZWLHandle nDeviceHandle, char* szsn, int &nlen);
	bool FBC_API_PUBLIC cardGetCardSN(YZWLHandle nDeviceHandle, char* szsn, int &nlen);

	bool FBC_API_PUBLIC getACardNo(YZWLHandle nDeviceHandle, char* arrcardNo, int &nlen);
	bool FBC_API_PUBLIC authACardAPwd(YZWLHandle ldevHandle, int nsector, char* szpwd);
	bool FBC_API_PUBLIC authACardBPwd(YZWLHandle ldevHandle, int nsector, char* szpwd);
	bool FBC_API_PUBLIC readACardData(YZWLHandle ldevHandle, int nblock/*nblock = nsector*4+index */, char* szread);
	bool FBC_API_PUBLIC writeACardData(YZWLHandle ldevHandle, int nblock/*nblock = nsector*4+index */, char* szwrite);

	bool FBC_API_PUBLIC cardGetDeviceNO(YZWLHandle nDeviceHandle, char* szno, int &nlen);
	bool FBC_API_PUBLIC cardGetDeviceSN(YZWLHandle nDeviceHandle, char* szsn, int &nlen);

	bool FBC_API_PUBLIC cardReadTwoCard(YZWLHandle nDeviceHandle, cardReadProgress cardCB, 
		void* userData, TwoIdInfoStructEx &cardinfo);
	bool FBC_API_PUBLIC cardReadTwoCardEx(YZWLHandle nDeviceHandle, cardReadProgress cardCB,
		void* userData, CardInfoStruct &cardinfo);

	bool FBC_API_PUBLIC cardGetSerialNumber(YZWLHandle nDeviceHandle, char* szSn, int &nSn);

	bool FBC_API_PUBLIC cardBeep(YZWLHandle nDeviceHandle);

	int FBC_API_PUBLIC	cardGetLastErrorCode(YZWLHandle nDeviceHandle);
	bool	FBC_API_PUBLIC cardGetErrorInfo(YZWLHandle nDeviceHandle, char* szErr, int &nlen, int nErrorCode);
	bool	FBC_API_PUBLIC cardGetLastError(YZWLHandle nDeviceHandle, char* szErr, int &nlen);

	void FBC_API_PUBLIC cardCloseDevice(YZWLHandle nDeviceHandle);

	int	FBC_API_PUBLIC	convertUncodeToUtf8(char* Src, int nsrc, char* strOut);
	void 	FBC_API_PUBLIC convertCardInfoUncodeToUtf8(TwoIdInfoStructEx &cardinfo);
	void 	FBC_API_PUBLIC convertCardInfoUtf8ToUncode(TwoIdInfoStructEx &cardinfo);
	void 	FBC_API_PUBLIC convertCardInfoUncodeToUtf8Ex(CardInfoStruct &cardinfo);

	// srcimage长度固定为1024 
	// outimage大小由外面自己申请，大小不小于40KB
	// outlen传入时为outimage实际大小，传出时为实际图片大小
	bool FBC_API_PUBLIC decodeCardImage(unsigned char* srcimage, char* outimage, int* outlen);

	void 	FBC_API_PUBLIC logoutCardServer();
	void 	FBC_API_PUBLIC cardReadUninit(); // 反初始化操作，最后程序析构的时候调用


#ifdef __cplusplus
}
#endif
