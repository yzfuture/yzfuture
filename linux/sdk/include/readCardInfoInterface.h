#pragma once
#include "Type.h"

#define FBC_API_PUBLIC __attribute__((visibility ("default")))
#define FBC_API_LOCAL __attribute__((visibility("hidden")))

#ifdef __cplusplus
extern "C" {
#endif

	typedef void(FBC_API_PUBLIC *cardReadProgress)(unsigned int nProgress);

	YZWLHandle FBC_API_PUBLIC cardOpenDevice(int nouttime, int nDeviceNo = 0);
	bool FBC_API_PUBLIC setCardType(YZWLHandle nDeviceHandle);
	bool FBC_API_PUBLIC cardFindCard(YZWLHandle nDeviceHandle, bool &bmove);

	bool FBC_API_PUBLIC cardGetCardDN(YZWLHandle nDeviceHandle, char* szsn, int &nlen);
	bool FBC_API_PUBLIC cardGetCardSN(YZWLHandle nDeviceHandle, char* szsn, int &nlen);

	bool FBC_API_PUBLIC cardGetDeviceNO(YZWLHandle nDeviceHandle, char* szno, int &nlen);
	bool FBC_API_PUBLIC cardGetDeviceSN(YZWLHandle nDeviceHandle, char* szsn, int &nlen);

	bool FBC_API_PUBLIC cardReadTwoCard(YZWLHandle nDeviceHandle, cardReadProgress cardCB,
		char* szFactoryFlag, char* szServerIp, int nServerPort,
		TwoIdInfoStructEx &cardinfo, bool bTest);

	bool FBC_API_PUBLIC cardGetSerialNumber(YZWLHandle nDeviceHandle, char* szSn, int &nSn);

	bool FBC_API_PUBLIC cardBeep(YZWLHandle nDeviceHandle);

	int FBC_API_PUBLIC	cardGetLastErrorCode(YZWLHandle nDeviceHandle);
	bool	FBC_API_PUBLIC cardGetErrorInfo(YZWLHandle nDeviceHandle, char* szErr, int &nlen, int nErrorCode);
	bool	FBC_API_PUBLIC cardGetLastError(YZWLHandle nDeviceHandle, char* szErr, int &nlen);

	void FBC_API_PUBLIC cardCloseDevice(YZWLHandle nDeviceHandle);

	void FBC_API_PUBLIC convertCardInfoUncodeToUtf8(TwoIdInfoStructEx &cardinfo);
	void FBC_API_PUBLIC convertCardInfoUtf8ToUncode(TwoIdInfoStructEx &cardinfo);

	// srcimage长度固定为1024 
	// outimage大小由外面自己申请，大小不小于40KB
	// outlen传入时为outimage实际大小，传出时为实际图片大小
	bool FBC_API_PUBLIC decodeCardImage(unsigned char* srcimage, char* outimage, int& outlen);

#ifdef __cplusplus
}
#endif
