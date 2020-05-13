#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <map>
#include <chrono>
#include <string>
using namespace std;

#include <boost/filesystem/path.hpp>  
#include <boost/filesystem/operations.hpp>  
#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/ini_parser.hpp>  

#include "Type.h"
#include "readCardInfoInterface.h"


void onCardReadProgress(unsigned int nProgress)
{
	printf("read progress[%u]\r\n", nProgress);
}

int readCardInfo()
{
	std::string	szFactoryFlag = "";
	std::string szServerIP = "id.vdin01.com";
	int			nServerPort = 8848;
	int			nindex = 0;
	bool		bTest = false;

	std::string szlog = boost::filesystem::initial_path<boost::filesystem::path>().string();
	std::string	szConfig = szlog + "/Config.ini";
	using namespace boost::property_tree;
	try
	{
		ptree pt;
		read_ini(szConfig, pt);
		szFactoryFlag = pt.get<std::string>("Config.FactoryFlag");
		szServerIP = pt.get<std::string>("Config.DecodeIP");
		nServerPort = pt.get<int>("Config.DecodePort");
		nindex = pt.get<int>("Config.DeviceIndex");
		bTest = pt.get<bool>("Config.TestServer");
	}
	catch (...)
	{
	}


	auto itstart = std::chrono::steady_clock::now();

	bool		bsuccess(false);
	TwoIdInfoStructEx cardinfo;
	std::string	szerror = "";

	YZWLHandle hlHandle = cardOpenDevice(2, nindex);;
	if (hlHandle >= 0)
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
					bool		bret = cardReadTwoCard(hlHandle, &onCardReadProgress,
						(char*)szFactoryFlag.c_str(), (char*)szServerIP.c_str(),
						nServerPort, cardinfo, bTest);
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
						convertCardInfoUncodeToUtf8(cardinfo);

						printf("user NO:%s\r\n", cardinfo.arrTwoIdNo);
						printf("user Name:%s\r\n", cardinfo.arrTwoIdName);
						printf("user Address:%s\r\n", cardinfo.arrTwoIdAddress);
						printf("user Birthday:%s\r\n", cardinfo.arrTwoIdBirthday);
						printf("user Period:%s~%s\r\n", cardinfo.arrTwoIdValidityPeriodBegin, cardinfo.arrTwoIdValidityPeriodEnd);
						printf("user Department:%s\r\n", cardinfo.arrTwoIdSignedDepartment);

						char szBmp[1024 * 40] = { 0 };
						int outlen = 1024 * 40;
						if (decodeCardImage(cardinfo.arrTwoIdPhoto, szBmp, outlen))
						{
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
