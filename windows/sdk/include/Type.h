#pragma once

#include <stdint.h>
				
#ifdef FOR_WINDOWS10
typedef long long YZWLHandle;				
#else
typedef long YZWLHandle;				
#endif

typedef enum cardType
{
	unkwonType = -1,
	ACardType = 0,
	BCardType = 1
};

typedef struct TwoIdInfoStructEx
{
	char arrTwoIdName[30];					//姓名 UNICODE
	char arrTwoIdSex[2];					//性别 UNICODE
	char arrTwoIdNation[4];					//民族 UNICODE
	char arrTwoIdBirthday[16];				//出生日期 UNICODE YYYYMMDD
	char arrTwoIdAddress[70];				//住址 UNICODE
	char arrTwoIdNo[36];					//身份证号码 UNICODE
	char arrTwoIdSignedDepartment[30];		//签发机关 UNICODE
	char arrTwoIdValidityPeriodBegin[16];	//有效期起始日期 UNICODE YYYYMMDD
	char arrTwoIdValidityPeriodEnd[16];		//有效期截止日期 UNICODE YYYYMMDD 有效期为长期时存储“长期”

	char arrTwoOtherNO[18]; // 通行证类号码
	char arrTwoSignNum[4];		// 签发次数
	char arrTwoRemark1[6];	// 预留区
	char arrTwoType[2];		// 证件类型标识
	char arrTwoRemark2[6];	// 预留区

	char arrTwoIdNewAddress[70];			//最新住址 UNICODE
	char arrReserve[2];						//保留字节 字节对齐用
	unsigned char arrTwoIdPhoto[1024];		//照片信息
	unsigned char arrTwoIdFingerprint[1024];//指纹信息
	unsigned char arrTwoIdPhotoJpeg[4096];	//照片信息 JPEG 格式
	unsigned int  unTwoIdPhotoJpegLength;	//照片信息长度 JPEG格式
};
