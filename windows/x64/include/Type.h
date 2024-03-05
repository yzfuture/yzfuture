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


#pragma pack(push)
#pragma pack(1)
//二代证信息结构
typedef struct _TwoIdInfoStruct 
{
	char arrName[30];					//姓名 UNICODE
	char arrSex[2];					//性别 UNICODE
	char arrNation[4];					//民族 UNICODE
	char arrBirthday[16];				//出生日期 UNICODE YYYYMMDD
	char arrAddress[70];				//住址 UNICODE
	char arrNo[36];					//身份证号码 UNICODE
	char arrSignedDepartment[30];		//签发机关 UNICODE
	char arrValidityPeriodBegin[16];	//有效期起始日期 UNICODE YYYYMMDD
	char arrValidityPeriodEnd[16];		//有效期截止日期 UNICODE YYYYMMDD 有效期为长期时存储“长期”

	char arrOtherNO[18]; // 通行证类号码
	char arrSignNum[4];		// 签发次数
	char arrRemark1[6];	// 预留区
	char arrType[2];		// 证件类型标识
	char arrRemark2[6];	// 预留区

	unsigned char arrPhoto[1024];		//照片信息
	unsigned char arrFingerprint[1024];//指纹信息
}TwoIdInfoStruct;

typedef struct _ForeignerInfoOld // 外国人永久居住证
{
	char arrEnName[120];					//英文名
	char arrSex[2];					//性别 UNICODE
	char arrNo[30];					//15个字符的居留证号码 UNICODE
	char arrCountry[6];				//国籍 UNICODE GB/T2659-2000
	char arrName[30];				//中文姓名 UNICODE 如果没有中文姓名，则全为0x0020
	char arrValidityPeriodBegin[16];//签发日期 UNICODE YYYYMMDD
	char arrValidityPeriodEnd[16];	//终止日期 UNICODE YYYYMMDD
	char arrBirthday[16];			//出生日期 UNICODE YYYYMMDD

	char arrVersion[4]; // 版本号
	char arrSignedDepartment[8];		//签发机关代码 UNICODE 证件芯片内不存储签发机关
	char arrType[2];		// 证件类型标识
	char arrRemark2[6];	// 预留区

	unsigned char arrPhoto[1024];		//照片信息
	unsigned char arrFingerprint[1024];//指纹信息
}ForeignerInfoOld;

typedef struct _ForeignerInfoNew  // 外国人永久居住证新版
{
	char arrName[30];					//姓名 UNICODE
	char arrSex[2];					//性别 UNICODE
	char arrNation[4];					//民族 UNICODE
	char arrBirthday[16];				//出生日期 UNICODE YYYYMMDD
	char arrEnName[70];				//外文姓名 UNICODE
	char arrNo[36];					//身份证号码 UNICODE
	char arrSignedDepartment[30];		//签发机关 UNICODE
	char arrValidityPeriodBegin[16];	//有效期起始日期 UNICODE YYYYMMDD
	char arrValidityPeriodEnd[16];		//有效期截止日期 UNICODE YYYYMMDD 有效期为长期时存储“长期”

	char arrOtherNO[18]; // 通行证类号码
	char arrSignNum[4];		// 签发次数
	char arrCountry[6];		//国籍 UNICODE GB/T2659-2000
	char arrType[2];		// 证件类型标识
	char arrRemark2[6];	// 预留区

	unsigned char arrPhoto[1024];		//照片信息
	unsigned char arrFingerprint[1024];//指纹信息
}ForeignerInfoNew;

typedef enum _eCardFormatType
{
	eTwoIDType = ' ', // 身份证
	eTwoGATType = 'J', // 港澳台居民居住证
	eOldForeignerType = 'I', // 外国人永久居留身份证
	eNewForeignerType = 'Y', // 外国人永久居留身份证(新版)
}eCardFormatType;

typedef struct _CardInfoStruct
{
	char	etype; // eCardFormatType
	union
	{
		TwoIdInfoStruct	 twoId; // 身份证/港澳台居民居住证
		ForeignerInfoOld foreigner; // 旧版外国人永久居住证
		ForeignerInfoNew newForeigner; // 新版外国人永久居住证
	}info;
}CardInfoStruct;

#pragma pack(pop)