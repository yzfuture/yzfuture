// C++Demo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "Type.h"
#include "ReadCardInfoInterface.h"
#include <string>
#include <iostream>
using namespace std;

std::string hex_to_str(char* src, int nlen)
{
	std::string szdst;
	szdst.resize(nlen * 2);
	char*		sztmp = (char*)szdst.c_str();
	for (int i = 0; i < nlen; i++)
	{
		unsigned char	tmp = src[i];
		sprintf(sztmp, "%s%02x", sztmp, tmp);
	}
	return szdst;
}

void __stdcall onCardReadProgress(unsigned int nProgress, YZWLHandle nhandle)
{
	printf("读卡进度为[%u]\r", nProgress);
}

void sfzInfo(YZWLHandle hlHandle)
{
	CardInfoStruct cardinfo;
	bool		bret = readSFZInfoEx(hlHandle, &onCardReadProgress, &cardinfo);
	if (!bret)
	{
		printf("解码失败[%d]\r\n", cardGetLastErrorCode(hlHandle));
	}
	else
	{
		printf("解码完成\r\n");
		cardBeep(hlHandle);
		char	sfzbmp[200 * 1024] = { 0 };
		int		nsfz = sizeof(sfzbmp);
		if (cardInfoToImage(cardinfo, sfzbmp, nsfz))
		{
			FILE* fp(fopen("sfz.bmp", "wb"));
			if (fp)
			{
				fwrite(sfzbmp, nsfz, 1, fp);
				fclose(fp);
			}
			printf("\r\n身份证合成完成,保存路径 [.\\sfz.bmp]\r\n");
		}
		convertCardInfoToAnsiEx(cardinfo);
		unsigned char*	lpphoto = nullptr;
		if (cardinfo.etype == (char)eOldForeignerType)
		{
			printf("\r\n========旧版外国人居住证==========\r\n");
			ForeignerInfoOld	&oldInfo = cardinfo.info.foreigner;
			printf("EnName:%s\r\n", oldInfo.arrEnName);
			printf("Name:%s\r\n", oldInfo.arrName);
			printf("Sex:%s\r\n", oldInfo.arrSex);
			printf("NO:%s\r\n", oldInfo.arrNo);
			printf("Country:%s\r\n", oldInfo.arrCountry);
			printf("Birthday:%s\r\n", oldInfo.arrBirthday);
			printf("BeginData:%s\r\n", oldInfo.arrValidityPeriodBegin);
			printf("EndDate:%s\r\n", oldInfo.arrValidityPeriodEnd);
			lpphoto = cardinfo.info.foreigner.arrPhoto;
		}
		else if (cardinfo.etype == (char)eNewForeignerType)
		{
			printf("\r\n========新版外国人居住证==========\r\n");
			ForeignerInfoNew &newForeigner = cardinfo.info.newForeigner;
			printf("EnName:%s\r\n", newForeigner.arrEnName);
			printf("Name:%s\r\n", newForeigner.arrName);
			printf("NO:%s\r\n", newForeigner.arrNo);
			printf("Country:%s\r\n", newForeigner.arrCountry);
			printf("Birthday:%s\r\n", newForeigner.arrBirthday);
			printf("BeginData:%s\r\n", newForeigner.arrValidityPeriodBegin);
			printf("EndDate:%s\r\n", newForeigner.arrValidityPeriodEnd);
			lpphoto = cardinfo.info.newForeigner.arrPhoto;
		}
		else
		{
			if (cardinfo.etype == eTwoGATType) printf("\r\n========港澳台居住证==========\r\n");
			else printf("\r\n========身份证==========\r\n");
			TwoIdInfoStruct &twoId = cardinfo.info.twoId;
			printf("NO:%s\r\n", twoId.arrNo);
			printf("Name:%s\r\n", twoId.arrName);
			printf("Address:%s\r\n", twoId.arrAddress);
			printf("Birthday:%s\r\n", twoId.arrBirthday);
			printf("Date:%s~%s\r\n", twoId.arrValidityPeriodBegin, twoId.arrValidityPeriodEnd);
			printf("SignedDepartment1:%s\r\n", twoId.arrSignedDepartment);
			lpphoto = cardinfo.info.twoId.arrPhoto;
		}
		if (lpphoto)
		{
			int outlen = sizeof(sfzbmp);
			if (decodeCardImage(lpphoto, sfzbmp, outlen))
			{
				FILE* fpwlt(fopen("zp.wlt", "wb"));
				if (fpwlt)
				{
					fwrite(lpphoto, 1024, 1, fpwlt);
					fclose(fpwlt);
				}
				FILE* fp(fopen("zp.bmp", "wb"));
				if (fp)
				{
					fwrite(sfzbmp, outlen, 1, fp);
					fclose(fp);
				}
				printf("\r\n身份证头像保存路径 [.\\zp.bmp]\r\n");
			}
		}
		printf("\r\n==========================\r\n");
	}
}

void epassport(YZWLHandle hlHandle)
{
	epassportInfo	epassinfo;
	string			no;
	string			birth;
	string			validity;
	getchar();
	memset(&epassinfo, 0, sizeof(epassportInfo));
	printf("请输入护照号码，以回车键结束：");
	getline(cin, no);
	printf("请输入护照六位出生日期(格式：YYMMDD)，以回车键结束：");
	getline(cin, birth);
	printf("请输入护照六位到期日期(格式：YYMMDD)，以回车键结束：");
	getline(cin, validity);
	if (readEPassportInfo(hlHandle, &onCardReadProgress, (char*)no.c_str(), (char*)birth.c_str(), (char*)validity.c_str(), &epassinfo))
	{
		printf("证件类型(缩写):%s\r\n", std::string(epassinfo.arrPaperType, 2).c_str());
		printf("证件类型(全称):%s\r\n", epassinfo.arrTypeFullName);
		printf("签发国家或签发机构:%s\r\n", epassinfo.arrSignedDepartment);
		printf("英文名:%s\r\n", epassinfo.arrENName);
		printf("其它名:%s\r\n", epassinfo.arrOtherName);
		printf("护照号:%s\r\n", epassinfo.arrNo);
		printf("有效期:%s\r\n", std::string(epassinfo.arrValidityPeriodEnd, 6).c_str());
		printf("生日:%s\r\n", std::string(epassinfo.arrBirthday, 6).c_str());
		printf("国籍编码:%s\r\n", std::string(epassinfo.arrCountry, 3).c_str());
		printf("性别:%s\r\n", epassinfo.cSex == (char)'M' ? "男" : "女");
		FILE*	fpbmp(fopen("epassport.jpg", "wb"));
		if (fpbmp)
		{
			fwrite(epassinfo.arrFaceJpg, 1, epassinfo.nFaceLen, fpbmp);
			fclose(fpbmp);
			printf("\r\n护照头像保存路径 [.\\epassport.jpg]\r\n");
		}
		cardBeep(hlHandle);
	}
}

void cashInfo(YZWLHandle hlHandle)
{
	char	sn[64] = { 0 };
	char	no[64] = { 0 };
	char	validity[64] = { 0 };
	int		nsn = sizeof(sn);
	int		nno = sizeof(no);
	int		nvalidity = sizeof(validity);
	if (readCashInfo(hlHandle, sn, &nsn, no, &nno, validity, &nvalidity))
	{
		std::string szNo = hex_to_str(sn, nsn);
		printf("银行卡SN:%s\r\n", szNo.c_str());
		printf("银行卡卡号:%s\r\n", no);
		printf("银行卡有效期:[%.4s]\r\n", validity);
		cardBeep(hlHandle);
	}
}

void readIC(YZWLHandle hlHandle)
{
	int		nsector = 0;
	char	szapwd[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
	char	szbpwd[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
	char	szread[16];
	char	szwrite[16];
	for (int i = 0; i < 4; i++)
	{
		memset(szread, 0, sizeof(szread));
		if (cardICRead(hlHandle, szapwd, szbpwd, nsector, i, szread))
		{
			if (i == 0) memcpy(szwrite, szread, 16);
			if (i == 1)
			{
				if (cardICWrite(hlHandle, szapwd, szbpwd, nsector, i, szwrite))
				{
					printf("写IC卡%d扇区%d块成功\r\n", nsector, i);
				}
				else
				{
					int nlen = 0;
					printf("写IC卡%d扇区%d块失败，原因[%s]\r\n", nsector, i, cardGetErrorInfo(hlHandle, nlen, cardGetLastErrorCode(hlHandle)));
				}
			}
			printf("读IC卡%d扇区%d块成功\r\n", nsector, i);
		}
		else
		{
			printf("读IC卡%d扇区%d块失败\r\n", nsector, i);
		}
	}
}

int readCardInfo()
{
	std::string szServerIP = "id.yzfuture.cn";
	int			nServerPort = 443;
	int			nindex = 0;

	char	ctype = '0';
	do
	{
		printf("\r\n请选择读卡器:(0-标准读卡器 1-离线读卡器)\r\n");
		ctype = getchar();
	} while ((ctype != '0') && (ctype != '1'));
	if (ctype == '0')
	{
		nindex = 0;
	}
	else
	{
		nindex = 1001;
	}

	printf("开始读卡");
	int		nlen = 0;
	int		nerr;
	if (loginCardServerEx((char*)szServerIP.c_str(), nServerPort, nerr))
	{
		printf("登录成功\r\n");
		YZWLHandle hlHandle = cardOpenDevice(2, nerr, nindex);
		if (hlHandle != -1)
		{
			printf("打开设备成功\r\n");
			if (nindex == 0)
			{

				char	szdevSn[56] = { 0 };
				int		ndevSn = sizeof(szdevSn);
				eCardModel emodel = getCardModel(hlHandle, szdevSn, &ndevSn);
				if (Unknown_Model != emodel) printf("卡片SN[%s]\r\n", szdevSn);
				switch (emodel)
				{
				case SFZ_Model:
					printf("这是身份证\r\n");
					sfzInfo(hlHandle);
					break;
				case EPassport_Model:
					printf("这是护照\r\n");
					epassport(hlHandle);
					break;
				case Cash_Model:
					printf("这是银行卡\r\n");
					cashInfo(hlHandle);
					break;
				case M1_S70_Model:
					printf("这是S70卡\r\n");
					readIC(hlHandle);
					cardBeep(hlHandle);
					break;
				case M1_S50_Model:
					printf("这是S50卡\r\n");
					readIC(hlHandle);
					cardBeep(hlHandle);
					break;
				case M1_CPU_Model:
					printf("这是其它CPU卡\r\n");
					cardBeep(hlHandle);
					break;
				case M1_Ultralight_Model:
					printf("这是Ultralight卡\r\n");
					cardBeep(hlHandle);
					break;
				case M1_DESFire_Model:
					printf("这是DESFire卡\r\n");
					cardBeep(hlHandle);
					break;
				case Unknown_Model:
				default:
					break;
				}
			}
			else
			{
				printf("离线读卡器只支持读身份证\r\n");
				sfzInfo(hlHandle);
			}
		}
		else
		{
			printf("设备打开失败[%s]\r\n", cardGetErrorInfo(hlHandle, nlen, cardGetLastErrorCode(hlHandle)));
		}
		cardCloseDevice(hlHandle);
	}
	else
	{
		printf("登录失败\r\n");
	}
	logoutCardServer();
	printf("==========测====试====完====成==========\r\n");

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	/*
	* cardReadInit
	* loginCardServer
	* logoutCardServer
	* cardReadUninit
	* 以上四个接口就自己按照自己的程序逻辑处理，此处只是展示用法做为示例用
	*/
	cardReadInit();
	printf("\r\n请把身份证放到读卡器上，然后回车进行证件读取，q或Q退出.\r\n");

	char	cexit = '0';
	do
	{
		readCardInfo();
		cexit = getchar();
		if ((cexit == 'q') || (cexit == 'Q')) break;
		while ((cexit = getchar()) != '\n' && cexit != EOF)
		{
			continue;
		}
	} while (true);
	getchar();
	cardReadUninit();
	return 0;
}



