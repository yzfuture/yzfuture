// C++Demo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include "Type.h"
#include "ReadCardInfoInterface.h"
#include <string>

void __stdcall onCardReadProgress(unsigned int nProgress, YZWLHandle nhandle)
{
	printf("��������Ϊ[%u]\r", nProgress);
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

						printf("���֤�ţ�%s", cardinfo.arrTwoIdNo);
						printf("������%s", cardinfo.arrTwoIdName);
						printf("��ַ��%s", cardinfo.arrTwoIdAddress);
						printf("�������ڣ�%s", cardinfo.arrTwoIdBirthday);
						printf("��Ч�ڣ�%s~%s", cardinfo.arrTwoIdValidityPeriodBegin, cardinfo.arrTwoIdValidityPeriodEnd);
						printf("ǩ�����أ�%s", cardinfo.arrTwoIdSignedDepartment);

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
						else
						{
							printf("��ͼƬʧ��\r\n");
						}
					}
					else
					{
						printf("�����֤ʧ��\r\n");
					}
				}
				else
				{
					printf("��ȡ��ƬSNʧ��\r\n");
				}
			}
			else
			{
				printf("Ѱ��ʧ��\r\n");
			}
		}
		else
		{
			printf("���ÿ�����ʧ��\r\n");
		}
	}
	else
	{
		printf("�򿪶�����ʧ��\r\n");
	}
	cardCloseDevice(hlHandle);
	getchar();
	return 0;
}

