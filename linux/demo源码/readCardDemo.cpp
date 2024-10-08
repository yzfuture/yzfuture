
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

int readCardInfo(int nindex)
{
	std::string szServerIP = "id.yzfuture.cn";
	int			nServerPort = 443;

	bool		bsuccess(false);
	std::string	szerror = "";
	int			nerr;

	if (nindex == 0)
	{
		setDeviceType(0);
	}
	else
	{
		if (nindex < 1001)
		{
			nindex = 1001;
		}
		setDeviceType(1);
	}

	if (loginCardServerEx((char*)szServerIP.c_str(), nServerPort, nerr))
	{
		YZWLHandle hlHandle = cardOpenDevice(2, nerr, nindex);
		if (hlHandle >= 0)
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

						CardInfoStruct cardinfo;
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
							printf("\033[34mDecode success, converting Unicode to ANSI to get Chinese  +++++++++++++\r\n");
							convertCardInfoUncodeToUtf8Ex(cardinfo);

							unsigned char*	lpphoto = nullptr;
							if (cardinfo.etype == (char)eOldForeignerType)
							{
								printf("\r\n========Foreigner's Residence Permit(old)==========\r\n");
								ForeignerInfoOld	oldInfo = cardinfo.info.foreigner;
								memset(&oldInfo, 0, sizeof(ForeignerInfoOld));
								// convertUncodeToUtf8(cardinfo.info.foreigner.arrEnName, sizeof(cardinfo.info.foreigner.arrEnName), oldInfo.arrEnName);
								printf("EnName:%s\r\n", oldInfo.arrEnName);
								// convertUncodeToUtf8(cardinfo.info.foreigner.arrName, sizeof(cardinfo.info.foreigner.arrName), oldInfo.arrName);
								printf("Name:%s\r\n", oldInfo.arrName);
								// convertUncodeToUtf8(cardinfo.info.foreigner.arrSex, sizeof(cardinfo.info.foreigner.arrSex), oldInfo.arrSex);
								printf("Sex:%s\r\n", oldInfo.arrSex);
								// convertUncodeToUtf8(cardinfo.info.foreigner.arrNo, sizeof(cardinfo.info.foreigner.arrNo), oldInfo.arrNo);
								printf("NO:%s\r\n", oldInfo.arrNo);
								// convertUncodeToUtf8(cardinfo.info.foreigner.arrCountry, sizeof(cardinfo.info.foreigner.arrCountry), oldInfo.arrCountry);
								printf("Country:%s\r\n", oldInfo.arrCountry);
								// convertUncodeToUtf8(cardinfo.info.foreigner.arrBirthday, sizeof(cardinfo.info.foreigner.arrBirthday), oldInfo.arrBirthday);
								printf("Birthday:%s\r\n", oldInfo.arrBirthday);
								// convertUncodeToUtf8(cardinfo.info.foreigner.arrValidityPeriodBegin, sizeof(cardinfo.info.foreigner.arrValidityPeriodBegin), oldInfo.arrValidityPeriodBegin);
								printf("BeginData:%s\r\n", oldInfo.arrValidityPeriodBegin);
								// convertUncodeToUtf8(cardinfo.info.foreigner.arrValidityPeriodEnd, sizeof(cardinfo.info.foreigner.arrValidityPeriodEnd), oldInfo.arrValidityPeriodEnd);
								printf("EndDate:%s\r\n", oldInfo.arrValidityPeriodEnd);
								lpphoto = cardinfo.info.foreigner.arrPhoto;
							}
							else if (cardinfo.etype == (char)eNewForeignerType)
							{
								printf("\r\n========Foreigner's Residence Permit(new)==========\r\n");
								ForeignerInfoNew newForeigner = cardinfo.info.newForeigner;
								// convertUncodeToUtf8(cardinfo.info.newForeigner.arrEnName, sizeof(cardinfo.info.newForeigner.arrEnName), newForeigner.arrEnName);
								printf("EnName:%s\r\n", newForeigner.arrEnName);
								// convertUncodeToUtf8(cardinfo.info.newForeigner.arrName, sizeof(cardinfo.info.newForeigner.arrName), newForeigner.arrName);
								printf("Name:%s\r\n", newForeigner.arrName);
								// convertUncodeToUtf8(cardinfo.info.newForeigner.arrNo, sizeof(cardinfo.info.newForeigner.arrNo), newForeigner.arrNo);
								printf("NO:%s\r\n", newForeigner.arrNo);
								// convertUncodeToUtf8(cardinfo.info.newForeigner.arrCountry, sizeof(cardinfo.info.newForeigner.arrCountry), newForeigner.arrCountry);
								printf("Country:%s\r\n", newForeigner.arrCountry);
								// convertUncodeToUtf8(cardinfo.info.newForeigner.arrBirthday, sizeof(cardinfo.info.newForeigner.arrBirthday), newForeigner.arrBirthday);
								printf("Birthday:%s\r\n", newForeigner.arrBirthday);
								// convertUncodeToUtf8(cardinfo.info.newForeigner.arrValidityPeriodBegin, sizeof(cardinfo.info.newForeigner.arrValidityPeriodBegin), newForeigner.arrValidityPeriodBegin);
								printf("BeginData:%s\r\n", newForeigner.arrValidityPeriodBegin);
								// convertUncodeToUtf8(cardinfo.info.newForeigner.arrValidityPeriodEnd, sizeof(cardinfo.info.newForeigner.arrValidityPeriodEnd), newForeigner.arrValidityPeriodEnd);
								printf("EndDate:%s\r\n", newForeigner.arrValidityPeriodEnd);
								lpphoto = cardinfo.info.newForeigner.arrPhoto;
							}
							else
							{
								if (cardinfo.etype == (char)eTwoGATType) printf("\r\n========Hong Kong, Macao, and Taiwan Residence Permit==========\r\n");
								else printf("\r\n========TwoID==========\r\n");
								TwoIdInfoStruct twoId = cardinfo.info.twoId;
								// convertUncodeToUtf8(cardinfo.info.twoId.arrNo, sizeof(cardinfo.info.twoId.arrNo), twoId.arrNo);
								printf("NO:%s\r\n", twoId.arrNo);
								// convertUncodeToUtf8(cardinfo.info.twoId.arrName, sizeof(cardinfo.info.twoId.arrName), twoId.arrName);
								printf("Name:%s\r\n", twoId.arrName);
								// convertUncodeToUtf8(cardinfo.info.twoId.arrAddress, sizeof(cardinfo.info.twoId.arrAddress), twoId.arrAddress);
								printf("Address:%s\r\n", twoId.arrAddress);
								// convertUncodeToUtf8(cardinfo.info.twoId.arrBirthday, sizeof(cardinfo.info.twoId.arrBirthday), twoId.arrBirthday);
								printf("Birthday:%s\r\n", twoId.arrBirthday);
								// convertUncodeToUtf8(cardinfo.info.twoId.arrValidityPeriodBegin, sizeof(cardinfo.info.twoId.arrValidityPeriodBegin), twoId.arrValidityPeriodBegin);
								// convertUncodeToUtf8(cardinfo.info.twoId.arrValidityPeriodEnd, sizeof(cardinfo.info.twoId.arrValidityPeriodEnd), twoId.arrValidityPeriodEnd);
								printf("Date:%s~%s\r\n", twoId.arrValidityPeriodBegin, twoId.arrValidityPeriodEnd);
								// convertUncodeToUtf8(cardinfo.info.twoId.arrSignedDepartment, sizeof(cardinfo.info.twoId.arrSignedDepartment), twoId.arrSignedDepartment);
								printf("SignedDepartment1:%s\r\n", twoId.arrSignedDepartment);
								lpphoto = cardinfo.info.twoId.arrPhoto;
							}
							if (lpphoto)
							{
								char szBmp[1024 * 40] = { 0 };
								int outlen = 1024 * 40;
								if (decodeCardImage(lpphoto, szBmp, &outlen))
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
							printf("\033[0m");
						}
					}
				}
			}
			else
			{
				int		nlen(0);
				char	szError[1024] = { 0 };
				cardGetErrorInfo(hlHandle, szError, nlen, cardGetLastErrorCode(hlHandle));
				printf("set card type failed\r\n");
			}
		}
		else
		{
			int		nlen(0);
			char	szError[1024] = { 0 };
			cardGetErrorInfo(hlHandle, szError, nlen, -19997);
			printf("open device failed\r\n");
		}
		if (hlHandle >= 0)
		{
			cardCloseDevice(hlHandle);
			hlHandle = -1;
		}
	}

	return 0;
}

int main(int argc, char *argv[])
{
	cardReadInit();
	char	cexit = '0';
	do
	{
		printf("\r\nPlease enter the device index number:\r\n 0	- YZWL Standard card reader\r\n 1	- SDT Offline card reader\r\n q/Q	- exit\r\n");
		cexit = getchar();
		if (cexit == '0' || cexit == '1')
		{
			if (cexit == '1') readCardInfo(1001);
			else readCardInfo(0);
		}
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
