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
	/*
	* cardReadInit
	* loginCardServer
	* logoutCardServer
	* cardReadUninit
	* �����ĸ��ӿھ��Լ������Լ��ĳ����߼������˴�ֻ��չʾ�÷���Ϊʾ����
	*/
	cardReadInit();
	char	ctype = '0';
	do
	{
		printf("��ѡ�������:(0-��׼������ 1-���߶�����)\r\n");
		ctype = getchar();
	} while ((ctype != '0') && (ctype != '1'));
	printf("�����У����Ժ�......\r\n");

	std::string szServerIP = "id.yzfuture.cn";
	int			nServerPort = 443;
	int			nindex = 0;
	TwoIdInfoStructEx cardinfo;
	int			nerr;

	if (ctype == '0')
	{
		nindex = 0;
		setDeviceType(0);
		if (!loginCardServerEx("id.yzfuture.cn", 443, nerr))
		{
			printf("��¼ʧ�ܣ�������˳�\r\n");
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
				bool		bret = cardReadTwoCard(hlHandle, &onCardReadProgress, cardinfo);
				if (bret)
				{
					cardBeep(hlHandle);
					convertCardInfoToAnsi(cardinfo);

					printf("���֤�ţ�%s\r\n", cardinfo.arrTwoIdNo);
					printf("������%s\r\n", cardinfo.arrTwoIdName);
					printf("��ַ��%s\r\n", cardinfo.arrTwoIdAddress);
					printf("�������ڣ�%s\r\n", cardinfo.arrTwoIdBirthday);
					printf("��Ч�ڣ�%s~%s\r\n", cardinfo.arrTwoIdValidityPeriodBegin, cardinfo.arrTwoIdValidityPeriodEnd);
					printf("ǩ�����أ�%s\r\n", cardinfo.arrTwoIdSignedDepartment);

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
	printf("������˳�\r\n");
	getchar();
	getchar();
	if (ctype == '0')
	{
		logoutCardServer();
	}
	cardReadUninit();
	return 0;
}

