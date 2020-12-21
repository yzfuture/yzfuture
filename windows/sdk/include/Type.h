#pragma once

#include <stdint.h>

//#define		FOR_WINDOWS10					//WINDOWS 10 用long long
#ifdef FOR_WINDOWS10
typedef long long YZWLHandle;				//针对句柄单独搞个类型 便于改动
#else
typedef long YZWLHandle;				//针对句柄单独搞个类型 便于改动
#endif

#define		COS_RESULT_BUFFER_LENGTH		256
#define		COS_SEND_BUFFER_LENGTH			256


typedef bool(__stdcall *userApduCommand)(char* inbuf, int ninbuf, unsigned char* outbuf, int &noutbuf, void* userdata);

typedef enum cardType
{
	unkwonType = -1,
	ACardType = 0,
	BCardType = 1
};


#if 1 //作为VC demo调用头文件 把0改为1
typedef int		 					BOOL;
typedef unsigned char		 		byte;

#define		RT_NOT_HALT				0x26
#define		RT_ALL					0x52

#define		AM_A					0x60
#define		AM_B					0x61
#endif

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

