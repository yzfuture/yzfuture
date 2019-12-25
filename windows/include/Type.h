#pragma once

#include <stdint.h>

//#define		FOR_WINDOWS10					//WINDOWS 10 用long long
#ifdef FOR_WINDOWS10
typedef long long YZWLHandle;				//针对句柄单独搞个类型 便于改动
#else
typedef long YZWLHandle;				//针对句柄单独搞个类型 便于改动
#endif

#if 1 //作为VC demo调用头文件 把0改为1
typedef int		 					BOOL;
typedef unsigned char		 		byte;

#define		RT_NOT_HALT				0x26
#define		RT_ALL					0x52

#define		AM_A					0x60
#define		AM_B					0x61
#endif

typedef enum ErrorType
{
	//YZWL_DEVICEMIXERROR = -19999,

	// 网络出错代码
	// 带返回值
	YZWL_FIRSTAPPLYFAILED = -25000, // 首次鉴权出错
	YZWL_DEVICEAPPLYFAILED, // 设备鉴权出错
	YZWL_GETSERVERFAILED, // 获取服务器信息时失败
	YZWL_GETPASSPORTFAILED, // 获取护照信息时失败
	YZWL_GETCARDTYPEFAILED, // 获取卡片类型失败

	// 不带返回值
	YZWL_CURLFAILED = -29999, // 网络出错的开始码
	YZWL_PARSEUSERINFOFAILED, // 解析用户信息失败
	YZWL_PARSESERVERINFOFAILED, // 解析身份证服务器信息失败
	YZWL_PARSECARDINFOFAILED, // 解析身份证信息失败
	YZWL_PARSECARDINFOEXCEPTION, // 解析身份证信息异常
	YZWL_PARSEEPASSPORTFAILED, // 解析护照服务器信息异常
	YZWL_PARSEPASSPORTFAILED, // 解析护照信息失败
	YZWL_PARSEPASSPORTEXCEPTION, // 解析护照信息异常
	YZWL_SAVEINFOTOSERVERFAILED, // 保存卡片信息失败

	YZWL_RETCMDERROR, // 返回包命令ID错误
	YZWL_RETERRORDATAERROR, // 返回错误ID失败
	YZWL_RETERPARSEERROR, // 返回包解析数据出错
	YZWL_RETERFORMATERROR, // 返回包格式出错
	// 网络出错代码,结束

	YZWL_FACTORYEMPTY = -39999, // 厂商信息为空
	YZWL_CREATEDEVICEFAILED, // 创建设备失败
	YZWL_READDEVICENOOPEN, // 读卡器没有打开
	YZWL_FINDREQUESRFAILED, // 寻卡选卡失败
	YZWL_GETSNDNFAILED, // 获取SN或DN失败
	YZWL_GETDEVICEINFOFAILED, // 获取读卡器信息失败
	YZWL_GETTWOCARDINFOFAILED, // 获取身份证信息失败
	YZWL_DECODETWOCARDFAILED, // 解码身份证信息失败
	YZWL_SERVEDRINFOEXCEPTION, // 身份证解码服务器地址异常

	YZWL_GETDEVICENOFAILED, // 获取读卡器序列号失败
	YZWL_GETDEVICESNFAILED, // 获取读卡器芯片序列号失败

	YZWL_INPUTBUFSPACENENOUGH, // 输入空间不足
	YZWL_OUTPUTBUFSPACENENOUGH, // 输出空间不足
	YZWL_LOADLABLIRYFAILED, // 加载动态库失败
	YZWL_IMPORTFUNFAILED, // 导入动态函数失败
	YZWL_IMAGEDECODEFAILED, // 图片解码失败

	YZWL_CALLBACKFUNEMPTY, // 回调函数为空
	YZWL_CONNECTSERVERFAILED, // 连接服务器失败
	YZWL_SENDDATAFAILED, //  数据发送失败
	YZWL_TALKDATAFAILED, // 与服务器交互失败
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

typedef struct _EPassportInfoStruct{
	// 护照信息

	// 证件类型(缩写)
	char szPaperType[100];
	// 证件类型（全称）
	char szTypeFullName[100];
	// 签发国家或签发机构
	char szSignedDepartment[100];
	// 英文名
	char szENName[100];
	// 中文名
	char szCNName[100];
	// 证件号码
	char szIdNo[100];
	// 护照ID，算法暂时不清楚
	char szDocumentID[100];
	// 国籍
	char szCountry[100];
	// 出生日期 UNICODE YYMMDD
	char szBirthday[6];
	// 性别 UNICODE
	char szSex[1];
	// 有效期截止日期 UNICODE YYMMDD
	char szValidityPeriodEnd[6];
	// 照片信息
	char faceImage[1024 * 40];
}EPassportInfoStruct;
