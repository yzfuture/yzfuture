
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <map>
#include <chrono>
#include <string>
using namespace std;

#include "Type.h"
#include "readCardInfoInterface.h"

void onCardReadProgress(void* userData, unsigned int nProgress)
{
	printf("read progress[%u]\r\n", nProgress);
}

int readCardInfo()
{
	std::string szServerIP = "id.yzfuture.cn";
	int			nServerPort = 443;
	int			nindex = 0;

	bool		bsuccess(false);
	std::string	szerror = "";
	int			nerr;

	YZWLHandle hlHandle = cardOpenDeviceEx(
		(char*)szServerIP.c_str(),
		nServerPort,
		2,
		nerr, 
		nindex);
	if (hlHandle != -1)
	{
		cardBeep(hlHandle);
		
		char	arrcardNo[32] = { 0 };
		int		ncardNo = sizeof(arrcardNo);
		if (getACardNo(hlHandle, arrcardNo, ncardNo))
		{
			printf("getACardNo[%s]\r\n", arrcardNo);
		}
		else
		{
			printf("The card is no a type\r\n");
		}
		
		if (setCardType(hlHandle))
		{						
			bool		bmove(true);
			bool		bfind(cardFindCard(hlHandle, bmove));
			if (bfind)
			{
				char		szSN[100] = { 0 };
				int			nSNlen = 99;
				if (cardGetCardSN(hlHandle, szSN, nSNlen))
				{
					printf("cardGetCardSN[%s]\r\n", szSN);
					
					TwoIdInfoStructEx cardinfo;
					bool		bret = cardReadTwoCard(hlHandle, &onCardReadProgress, NULL, cardinfo);
					int		nSerialNumber(0);
					char	szSerialNumber[1024] = { 0 };
					cardGetSerialNumber(hlHandle, szSerialNumber, nSerialNumber);
					printf("Serial Number [%s]\r\n", szSerialNumber);
					if (!bret)
					{
						int		nlen(0);
						char	szError[1024] = { 0 };
						cardGetErrorInfo(hlHandle, szError, nlen, cardGetLastErrorCode(hlHandle));
					}
					else
					{
						bsuccess = true;
						printf("Decode success, converting Unicode to ANSI to get Chinese  +++++++++++++\r\n");
						
						char szBmp[1024 * 40] = { 0 };
						int outlen = 1024 * 40;
						if (decodeCardImage((unsigned char*)cardinfo.arrTwoIdPhoto, (char*)szBmp, &outlen))
						{
							FILE* fp(fopen("a.bmp", "wb"));
							if (fp)
							{
								fwrite(szBmp, outlen, 1, fp);
								fclose(fp);
							}
						}
					}					
					
					/*
					
					TwoCardInfoStruct cardinfo;
					bool		bret = cardReadTwoCardEx(hlHandle, &onCardReadProgress, NULL, cardinfo);
					int		nSerialNumber(0);
					char	szSerialNumber[1024] = { 0 };
					cardGetSerialNumber(hlHandle, szSerialNumber, nSerialNumber);
					printf("Serial Number [%s]\r\n", szSerialNumber);
					if (!bret)
					{
						int		nlen(0);
						char	szError[1024] = { 0 };
						cardGetErrorInfo(hlHandle, szError, nlen, cardGetLastErrorCode(hlHandle));
					}
					else
					{
						bsuccess = true;
						if (cardinfo.etype == eTwoIDType || cardinfo.etype == eTwoGATType)
						{
							// eTwoIDType 身份证
							// eTwoGATType 港澳台居民居住证
							printf("user NO:%s\r\n", WcharToChar((char*)cardinfo.twoInfo.arrTwoIdNo, sizeof(cardinfo.twoInfo.arrTwoIdNo)).c_str());
							printf("user Name:%s\r\n", WcharToChar((char*)cardinfo.twoInfo.arrTwoIdName, sizeof(cardinfo.twoInfo.arrTwoIdName)).c_str());
							printf("user Address:%s\r\n", WcharToChar((char*)cardinfo.twoInfo.arrTwoIdAddress, sizeof(cardinfo.twoInfo.arrTwoIdAddress)).c_str());
							printf("user Birthday:%s\r\n", WcharToChar((char*)cardinfo.twoInfo.arrTwoIdBirthday, sizeof(cardinfo.twoInfo.arrTwoIdBirthday)).c_str());
							printf("user Period:%s~%s\r\n", WcharToChar((char*)cardinfo.twoInfo.arrTwoIdValidityPeriodBegin, sizeof(cardinfo.twoInfo.arrTwoIdValidityPeriodBegin)).c_str(), WcharToChar((char*)cardinfo.twoInfo.arrTwoIdValidityPeriodEnd, sizeof(cardinfo.twoInfo.arrTwoIdValidityPeriodEnd)).c_str());
							printf("user Department:%s\r\n", WcharToChar((char*)cardinfo.twoInfo.arrTwoIdSignedDepartment, sizeof(cardinfo.twoInfo.arrTwoIdSignedDepartment)).c_str());
                        
							char szBmp[1024 * 40] = { 0 };
							int outlen = 1024 * 40;
							if (decodeCardImage((unsigned char*)cardinfo.twoInfo.arrTwoIdPhoto, (char*)szBmp, &outlen))
							{
								FILE* fp(fopen("a.bmp", "wb"));
								if (fp)
								{
									fwrite(szBmp, outlen, 1, fp);
									fclose(fp);
								}
							}							
						}
						else if (cardinfo.etype == eTwoForeignerType)
						{
							// 外国人永久居住证
							printf("En name:%s\r\n", cardinfo.foreignerInfo.arrEnName);
						}
					}*/
				}
			}
		}
		else
		{
			int		nlen(0);
			char	szError[1024] = { 0 };
			cardGetErrorInfo(hlHandle, szError, nlen, cardGetLastErrorCode(hlHandle));
		}
	}
	else
	{
		int		nlen(0);
		char	szError[1024] = { 0 };
		cardGetErrorInfo(hlHandle, szError, nlen, -19997);
	}
	if (hlHandle >= 0)
	{
		cardCloseDevice(hlHandle);
		hlHandle = -1;
	}
	return 0;
}

int main(int argc, char *argv[])
{
	char	cexit = '0';
	do
	{
		readCardInfo();
		//
		printf("\r\nenter any key continet, 'q' or 'Q' exit.\r\n");
		cexit = getchar();
		if ((cexit == 'q') || (cexit == 'Q')) break;
	} while (true);
	getchar();
	return 0;
}
