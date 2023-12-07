#pragma once
#include "Type.h"

#define FBC_API_PUBLIC __attribute__((visibility ("default")))
#define FBC_API_LOCAL __attribute__((visibility("hidden")))

#ifdef __cplusplus
extern "C" {
#endif

	typedef void(FBC_API_PUBLIC *cardReadProgress)(void* userData, unsigned int nProgress);

	YZWLHandle FBC_API_PUBLIC cardOpenDeviceEx(char* szip, int nport, int nouttime, int &nerr, int nDeviceNo = 0);
	bool FBC_API_PUBLIC setCardType(YZWLHandle nDeviceHandle);
	bool FBC_API_PUBLIC cardFindCard(YZWLHandle nDeviceHandle, bool &bmove);

	bool FBC_API_PUBLIC cardGetCardDN(YZWLHandle nDeviceHandle, char* szsn, int &nlen);
	bool FBC_API_PUBLIC cardGetCardSN(YZWLHandle nDeviceHandle, char* szsn, int &nlen);
	bool FBC_API_PUBLIC getACardNo(YZWLHandle nDeviceHandle, char* arrcardNo, int &nlen);

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

	// srcimage长度固定为1024 
	// outimage大小由外面自己申请，大小不小于40KB
	// outlen传入时为outimage实际大小，传出时为实际图片大小
	bool FBC_API_PUBLIC decodeCardImage(unsigned char* srcimage, char* outimage, int* outlen);


#ifdef __cplusplus
}
#endif
