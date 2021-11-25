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
	cardReadInit();
	char	ctype = '0';
	do
	{
		printf("请选择读卡器:(0-标准读卡器 1-离线读卡器)\r\n");
		ctype = getchar();
	} while ((ctype != '0') && (ctype != '1'));
	printf("读卡中，请稍候......\r\n");

 	std::string	szAppKey = "请参照《NFC服务注册流程 V2.pdf》申请";
 	std::string szAppSecret = "请参照《NFC服务注册流程 V2.pdf》申请";
 	std::string szUserData = "请参照《NFC服务注册流程 V2.pdf》申请";
	std::string szServerIP = "id.yzfuture.cn";
	int			nServerPort = 443;
	int			nindex = 0;
	TwoIdInfoStructEx cardinfo;

	if (ctype=='0')
	{
		nindex = 0;
		setDeviceType(0);
	}
	else
	{
		nindex = 1001;
		setDeviceType(1);
	}

	int 	nerr = 0;
	YZWLHandle hlHandle = cardOpenDevice((char*)szAppKey.c_str(), (char*)szAppSecret.c_str(),
		(char*)szServerIP.c_str(), nServerPort, (char*)szUserData.c_str(), 2, nerr, nindex);
	if (hlHandle >= 0)
	{
		if (setCardType(hlHandle, BCardType))
		{
			bool		bmove(true);
			bool		bfind(cardFindCard(hlHandle, bmove));
			if (bfind)
			{
				bool		bret = cardReadTwoCard(hlHandle, &onCardReadProgress, cardinfo);
				if (bret)
				{
					cardBeep(hlHandle);
					convertCardInfoToAnsi(cardinfo);

					printf("身份证号：%s\r\n", cardinfo.arrTwoIdNo);
					printf("姓名：%s\r\n", cardinfo.arrTwoIdName);
					printf("地址：%s\r\n", cardinfo.arrTwoIdAddress);
					printf("出生日期：%s\r\n", cardinfo.arrTwoIdBirthday);
					printf("有效期：%s~%s\r\n", cardinfo.arrTwoIdValidityPeriodBegin, cardinfo.arrTwoIdValidityPeriodEnd);
					printf("签发机关：%s\r\n", cardinfo.arrTwoIdSignedDepartment);

					char szBmp[1024 * 40] = { 0 };
					int outlen = 1024 * 40;
					if (decodeCardImage(cardinfo.arrTwoIdPhoto, szBmp, outlen))
					{
						FILE* fpwlt(fopen("a.wlt", "wb"));
						if (fpwlt)
						{
							fwrite(cardinfo.arrTwoIdPhoto, 1024, 1, fpwlt);
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
	cardCloseDevice(hlHandle);
	printf("任意键退出\r\n");
	getchar();
	getchar();
	cardReadUninit();
	return 0;
}

