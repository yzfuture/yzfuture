#pragma once
#include "Type.h"

typedef enum _eCardModel
{
	Unknown_Model,
	SFZ_Model,
	EPassport_Model,
	Cash_Model,
	M1_S70_Model,
	M1_S50_Model,
	M1_CPU_Model,
	M1_Ultralight_Model,
	M1_DESFire_Model
}eCardModel;
// 身份证、护照 回调 20步
typedef void(__stdcall *cardReadProgress)(unsigned int nProgress, YZWLHandle nhandle);

// 初始化操作，最开始程序构造的时候调用
void __stdcall cardReadInit();
// 登录服务器
bool __stdcall loginCardServerEx(char* szip, int nport, int &nerr);
// 打开设备
YZWLHandle __stdcall cardOpenDevice(int nouttime, int &nerr, int nDeviceNo = 0);

// 获取卡片类型和SN号：身份证不返回SN
eCardModel __stdcall getCardModel(YZWLHandle nDeviceHandle, char* szsn, int* nsn);

// 二代证
// 返回身份证/港澳居住证/外国人居住证信息
bool __stdcall readSFZInfo(YZWLHandle nDeviceHandle, cardReadProgress cardCB, char* szbase, int* nlen, char* ctype, bool boldSdt = false);
bool __stdcall readSFZInfoEx(YZWLHandle nDeviceHandle, cardReadProgress cardCB, CardInfoStruct* cardinfo);
// 二代证信息转换成ansi编码
void __stdcall convertCardInfoToAnsiEx(CardInfoStruct &cardinfo);
// 二代证DN码
bool __stdcall cardGetCardDN(YZWLHandle nDeviceHandle, char* szsn, int &nlen);
// 解码二代证头像
// srcimage长度固定为1024 
// outimage大小由外面自己申请，大小不小于40KB，生成身份证头像bmp格式
// outlen传入时为outimage实际大小，传出时为实际图片大小
bool __stdcall decodeCardImage(unsigned char* srcimage, char* outimage, int& outlen);
// 二代证图片合成
// outimage大小由外面自己申请，大小不小于200KB
// outlen传入时为outimage实际大小，传出时为实际图片大小
// ntype: 0-正面照  1-反面照  2-横向双面  3-纵向双面
// nformat:0--bmp,1--jpg,2--png
// szwatermar:水印，为空时内容为默认值 
bool __stdcall cardInfoToImage(CardInfoStruct cardinfo, char* outimage, int& outlen, int ntype = 3, int nformat = 1, char* szwatermark="");
bool __stdcall twoIdToImage(TwoIdInfoStruct twoId, char* outimage, int& outlen, int ntype = 3, int nformat = 1, char* szwatermark = "");
bool __stdcall oldForeignerToImage(ForeignerInfoOld foreigner, char* outimage, int& outlen, int ntype = 3, int nformat = 1, char* szwatermark = "");
bool __stdcall newForeignerToImage(ForeignerInfoNew foreigner, char* outimage, int& outlen, int ntype = 3, int nformat = 1, char* szwatermark = "");

// 护照：返回护照信息
bool __stdcall readEPassportInfo(YZWLHandle nDeviceHandle, cardReadProgress cardCB, char* no, char* birth, char* validity, epassportInfo* cardinfo);

// 银行卡：返回卡号和有效期
bool __stdcall readCashInfo(YZWLHandle nDeviceHandle, char* sn, int* nsn, char* no, int* nno, char* validity, int* nvalidity);

// IC卡：每次读写16个字节
bool __stdcall cardICRead(YZWLHandle nDeviceHandle, char szpwdA[6], char szpwdB[6],  int nsector, int nblock, char szread[16]);
bool __stdcall cardICWrite(YZWLHandle nDeviceHandle, char szpwdA[6], char szpwdB[6],  int nsector, int nblock, char szwrite[16]);

// 读卡器蜂鸣一次
bool __stdcall cardBeep(YZWLHandle nDeviceHandle);

// 读取流水号
const char* __stdcall cardGetSerialNumber(YZWLHandle nDeviceHandle);

// 获取失败码
int __stdcall	cardGetLastErrorCode(YZWLHandle nDeviceHandle);
// 通过失败码获取详细信息
const char*	__stdcall cardGetErrorInfo(YZWLHandle nDeviceHandle, int &nlen, int nErrorCode);

// 关闭读卡器
void __stdcall cardCloseDevice(YZWLHandle nDeviceHandle);
// 登出服务器
void __stdcall logoutCardServer();
// 反初始化
void __stdcall cardReadUninit(); // 反初始化操作，最后程序析构的时候调用

// 正常情况下调用loginCardServerEx即可
bool __stdcall loginCardServer(char* szip, int nport, char* szAppkey, char* szAppSecret, char* szAppUserId, int &nerr);


// 以下接口为过时接口，后期会逐渐停止支持
// Deprecated 新版本会根据cardOpenDevice的nDeviceNo判断
void __stdcall setDeviceType(int nDeviceType);
// Deprecated 过时接口
bool __stdcall cardReadTwoCard(YZWLHandle nDeviceHandle, cardReadProgress cardCB, TwoIdInfoStructEx &cardinfo);
// Deprecated readSFZInfo替换
bool __stdcall cardReadTwoCardEx(YZWLHandle nDeviceHandle, cardReadProgress cardCB, CardInfoStruct& cardinfo);
// Deprecated readSFZInfoEx替换
bool __stdcall cardReadTwoCardBase(YZWLHandle nDeviceHandle, cardReadProgress cardCB, char* szbase, int& nlen, char	&ctype, bool boldSdt=false);// szbase大小不小于256+1024+1024，存储结构与具体证件对应结构体相同  nlen传入值不得小于256+1024+1024，传出值为实际大小
// Deprecated getCardModel替换
bool __stdcall cardGetCardSN(YZWLHandle nDeviceHandle, char* szsn, int &nlen);
// Deprecated 不需要设置
bool __stdcall setCardType(YZWLHandle nDeviceHandle, cardType ctype);
// Deprecated getCardModel替换
bool __stdcall getCardNo(YZWLHandle nDeviceHandle, char* arrcardNo, int &nlen);
// Deprecated 新版本不需要调用
bool __stdcall cardFindCard(YZWLHandle nDeviceHandle, bool &bmove);
// Deprecated 新版本不需要调用
bool __stdcall cardSelectCard(YZWLHandle nDeviceHandle);
// Deprecated 功能与cardGetErrorInfo重复
const char*	__stdcall cardGetLastError(YZWLHandle nDeviceHandle, int &nlen);
// Deprecated  过时接口
void __stdcall convertCardInfoToAnsi(TwoIdInfoStructEx &cardinfo);

// Deprecated 新接口中此枚举不需要
typedef enum _eDeviceType
{
	yzwlType = 0,
	sdtapiType = 1
}eDeviceType;
