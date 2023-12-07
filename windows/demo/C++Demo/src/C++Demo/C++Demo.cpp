// C++Demo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "Type.h"
#include "ReadCardInfoInterface.h"
#include <string>

void __stdcall onCardReadProgress(unsigned int nProgress, YZWLHandle nhandle)
{
	printf("读卡进度为[%u]\r", nProgress);
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
	char	ctype = '0';
	do
	{
		printf("请选择读卡器:(0-标准读卡器 1-离线读卡器)\r\n");
		ctype = getchar();
	} while ((ctype != '0') && (ctype != '1'));
	printf("读卡中，请稍候......\r\n");

	std::string szServerIP = "id.yzfuture.cn";
	int			nServerPort = 443;
	int			nindex = 0;
	CardInfoStruct cardinfo;
	int			nerr;

	if (ctype == '0')
	{
		nindex = 0;
		setDeviceType(0);
		if (!loginCardServerEx("id.yzfuture.cn", 443, nerr))
		{
			printf("登录失败，任意键退出\r\n");
			getchar();
			getchar();
			logoutCardServer();
			cardReadUninit();
			return 0;
		}
	}
	else
	{
		nindex = 1001;
		setDeviceType(1);
	}

	YZWLHandle hlHandle = cardOpenDevice(2, nerr, nindex);
	if (hlHandle >= 0)
	{
		if (setCardType(hlHandle, BCardType))
		{
			bool		bmove(true);
			bool		bfind(cardFindCard(hlHandle, bmove));
			if (bfind)
			{
				bool		bret = cardReadTwoCardEx(hlHandle, &onCardReadProgress, cardinfo);
				if (bret)
				{
					cardBeep(hlHandle);
					convertCardInfoToAnsiEx(cardinfo);
					unsigned char*	lpphoto = nullptr;
					if (cardinfo.etype == eOldForeignerType)
					{
						printf("\r\n========旧版外国人居住证==========\r\n");
						printf("英文名：%s\r\n", cardinfo.info.foreigner.arrEnName);
						printf("中文姓名：%s\r\n", cardinfo.info.foreigner.arrName);
						printf("性别：%s\r\n", cardinfo.info.foreigner.arrSex);
						printf("居留证号码：%s\r\n", cardinfo.info.foreigner.arrNo);
						printf("国籍：%s\r\n", cardinfo.info.foreigner.arrCountry);
						printf("签发日期：%s\r\n", cardinfo.info.foreigner.arrValidityPeriodBegin);
						lpphoto = cardinfo.info.foreigner.arrPhoto;
					}
					else if (cardinfo.etype == eNewForeignerType)
					{
						printf("\r\n========新版外国人居住证==========\r\n");
						printf("外文姓名：%s\r\n", cardinfo.info.newForeigner.arrEnName);
						printf("中文姓名：%s\r\n", cardinfo.info.newForeigner.arrName);
						printf("居留证号：%s\r\n", cardinfo.info.newForeigner.arrNo);
						printf("国籍：%s\r\n", cardinfo.info.newForeigner.arrCountry);
						printf("出生日期：%s\r\n", cardinfo.info.newForeigner.arrBirthday);
						printf("有效期：%s~%s\r\n", cardinfo.info.newForeigner.arrValidityPeriodBegin, cardinfo.info.newForeigner.arrValidityPeriodEnd);
						lpphoto = cardinfo.info.newForeigner.arrPhoto;
					}
					else
					{
						if (cardinfo.etype == eTwoGATType) printf("\r\n========港澳台居住证==========\r\n");
						else printf("\r\n========身份证==========\r\n");
						printf("身份证号：%s\r\n", cardinfo.info.twoId.arrNo);
						printf("姓名：%s\r\n", cardinfo.info.twoId.arrName);
						printf("地址：%s\r\n", cardinfo.info.twoId.arrAddress);
						printf("出生日期：%s\r\n", cardinfo.info.twoId.arrBirthday);
						printf("有效期：%s~%s\r\n", cardinfo.info.twoId.arrValidityPeriodBegin, cardinfo.info.twoId.arrValidityPeriodEnd);
						printf("签发机关：%s\r\n", cardinfo.info.twoId.arrSignedDepartment);
						lpphoto = cardinfo.info.twoId.arrPhoto;
					}
					if (lpphoto)
					{
						char szBmp[1024 * 40] = { 0 };
						int outlen = 1024 * 40;
						if (decodeCardImage(lpphoto, szBmp, outlen))
						{
							FILE* fpwlt(fopen("a.wlt", "wb"));
							if (fpwlt)
							{
								fwrite(lpphoto, 1024, 1, fpwlt);
								fclose(fpwlt);
							}
							FILE* fp(fopen("a.bmp", "wb"));
							if (fp)
							{
								fwrite(szBmp, outlen, 1, fp);
								fclose(fp);
							}
						}
					}
				}
			}
		}
	}
	cardCloseDevice(hlHandle);
	printf("任意键退出\r\n");
	getchar();
	getchar();
	if (ctype == '0')
	{
		logoutCardServer();
	}
	cardReadUninit();
	return 0;
}

