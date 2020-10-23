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
	std::string	szFactoryFlag = "99ffb2f98a29071107c7a09ad2c6d096";
	std::string szServerIP = "id.yzfuture.cn";
	int			nServerPort = 8848;
	int			nindex = 0;
	bool		bTest = true;
	TwoIdInfoStructEx cardinfo;
	YZWLHandle hlHandle = cardOpenDevice(2);
	if (hlHandle >= 0)
	{
		if (setCardType(hlHandle, BCardType))
		{
			bool		bmove(true);
			bool		bfind(cardFindCard(hlHandle, bmove));
			if (bfind)
			{
				char		szSN[100] = { 0 };
				int			nSNlen = 99;
				//					bool		bselect(cardSelectCard(hlHandle));
				if (cardGetCardSN(hlHandle, szSN, nSNlen))
				{
					printf("cardGetCardSN[%s]\r\n", szSN);
					bool		bret = cardReadTwoCard(hlHandle, &onCardReadProgress,
						(char*)szFactoryFlag.c_str(), (char*)szServerIP.c_str(),
						nServerPort, cardinfo, bTest);
					if (bret)
					{
						cardBeep(hlHandle);
						convertCardInfoToAnsi(cardinfo);

						printf("身份证号：%s", cardinfo.arrTwoIdNo);
						printf("姓名：%s", cardinfo.arrTwoIdName);
						printf("地址：%s", cardinfo.arrTwoIdAddress);
						printf("出生日期：%s", cardinfo.arrTwoIdBirthday);
						printf("有效期：%s~%s", cardinfo.arrTwoIdValidityPeriodBegin, cardinfo.arrTwoIdValidityPeriodEnd);
						printf("签发机关：%s", cardinfo.arrTwoIdSignedDepartment);

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
	}
	cardCloseDevice(hlHandle);
	getchar();
	return 0;
}

