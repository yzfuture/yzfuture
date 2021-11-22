#pragma once
#include "Type.h"

#define FBC_API_PUBLIC __attribute__((visibility ("default")))
#define FBC_API_LOCAL __attribute__((visibility("hidden")))

#ifdef __cplusplus
extern "C" {
#endif

	typedef void(FBC_API_PUBLIC *cardReadProgress)(void* userData, unsigned int nProgress);

	YZWLHandle FBC_API_PUBLIC cardOpenDevice(char* szAppkey, char* szAppSecret, char* szip, 
		int nport, char* userData, int nouttime, int &nerr, int nDeviceNo = 0);
	bool FBC_API_PUBLIC setCardType(YZWLHandle nDeviceHandle);
	bool FBC_API_PUBLIC cardFindCard(YZWLHandle nDeviceHandle, bool &bmove);

	bool FBC_API_PUBLIC cardGetCardDN(YZWLHandle nDeviceHandle, char* szsn, int &nlen);
	bool FBC_API_PUBLIC cardGetCardSN(YZWLHandle nDeviceHandle, char* szsn, int &nlen);

	bool FBC_API_PUBLIC cardGetDeviceNO(YZWLHandle nDeviceHandle, char* szno, int &nlen);
	bool FBC_API_PUBLIC cardGetDeviceSN(YZWLHandle nDeviceHandle, char* szsn, int &nlen);

	bool FBC_API_PUBLIC cardReadTwoCard(YZWLHandle nDeviceHandle, cardReadProgress cardCB, 
		void* userData, TwoIdInfoStructEx &cardinfo);

	bool FBC_API_PUBLIC cardGetSerialNumber(YZWLHandle nDeviceHandle, char* szSn, int &nSn);

	bool FBC_API_PUBLIC cardBeep(YZWLHandle nDeviceHandle);

	int FBC_API_PUBLIC	cardGetLastErrorCode(YZWLHandle nDeviceHandle);
	bool	FBC_API_PUBLIC cardGetErrorInfo(YZWLHandle nDeviceHandle, char* szErr, int &nlen, int nErrorCode);
	bool	FBC_API_PUBLIC cardGetLastError(YZWLHandle nDeviceHandle, char* szErr, int &nlen);

	void FBC_API_PUBLIC cardCloseDevice(YZWLHandle nDeviceHandle);

	// srcimage���ȹ̶�Ϊ1024 
	// outimage��С�������Լ����룬��С��С��40KB
	// outlen����ʱΪoutimageʵ�ʴ�С������ʱΪʵ��ͼƬ��С
	bool FBC_API_PUBLIC decodeCardImage(unsigned char* srcimage, char* outimage, int* outlen);


#ifdef __cplusplus
}
#endif
