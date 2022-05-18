
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <map>
#include <chrono>
#include <string>
using namespace std;

#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/ini_parser.hpp>  

#include "Type.h"
#include "readCardInfoInterface.h"
#include "iconv.h"

int code_convert(char *from_charset, char *to_charset, char *inbuf, size_t inlen, char *outbuf, size_t &outlen)
{
	iconv_t cd;
	int rc;
	char **pin = &inbuf;
	char **pout = &outbuf;
	//    char *from_charset = "UTF-16le";
	//    char *to_charset = "UTF-8";

	cd = iconv_open(to_charset, from_charset);
	if (cd == 0) return -1;
	memset(outbuf, 0, outlen);
	if (iconv(cd, pin, &inlen, pout, &outlen) == -1) return -1;
	iconv_close(cd);
	return 0;
}

std::string WcharToChar(char* inbuf, int inlen)
{
	size_t          outLen = inlen * 2;
	std::string     szRetBuf;
	szRetBuf.resize(outLen);
	char*           outbuf = (char*)szRetBuf.c_str();
	memset(outbuf, 0, outLen);
	if (outbuf)
	{
		code_convert("UTF-16le", "UTF-8", inbuf, inlen, outbuf, outLen);
	}
	return szRetBuf;
}


void onCardReadProgress(void* userData, unsigned int nProgress)
{
	printf("read progress[%u]\r\n", nProgress);
}

int readCardInfo()
{
	std::string szServerIP = "id.yzfuture.cn";
	int			nServerPort = 443;
	int			nindex = 0;

	char *buffer;
	buffer = getcwd(NULL, 0);
	std::string szlog = buffer;
	std::string	szConfig = szlog + "/Config.ini";
	using namespace boost::property_tree;
	try
	{
		ptree pt;
		read_ini(szConfig, pt);
		szServerIP = pt.get<std::string>("Config.DecodeIP");
		nServerPort = pt.get<int>("Config.DecodePort");
		nindex = pt.get<int>("Config.DeviceIndex");
	}
	catch (...)
	{
	}


	auto itstart = std::chrono::steady_clock::now();

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
		if (setCardType(hlHandle))
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
						printf("user NO:%s\r\n", WcharToChar((char*)cardinfo.arrTwoIdNo, sizeof(cardinfo.arrTwoIdNo)).c_str());
						printf("user Name:%s\r\n", WcharToChar((char*)cardinfo.arrTwoIdName, sizeof(cardinfo.arrTwoIdName)).c_str());
						printf("user Address:%s\r\n", WcharToChar((char*)cardinfo.arrTwoIdAddress, sizeof(cardinfo.arrTwoIdAddress)).c_str());
						printf("user Birthday:%s\r\n", WcharToChar((char*)cardinfo.arrTwoIdBirthday, sizeof(cardinfo.arrTwoIdBirthday)).c_str());
						printf("user Period:%s~%s\r\n", WcharToChar((char*)cardinfo.arrTwoIdValidityPeriodBegin, sizeof(cardinfo.arrTwoIdValidityPeriodBegin)).c_str(), WcharToChar((char*)cardinfo.arrTwoIdValidityPeriodEnd, sizeof(cardinfo.arrTwoIdValidityPeriodEnd)).c_str());
						printf("user Department:%s\r\n", WcharToChar((char*)cardinfo.arrTwoIdSignedDepartment, sizeof(cardinfo.arrTwoIdSignedDepartment)).c_str());
                        
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
