#include "mainwindow.h"
#include "ui_mainwindow.h"


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

char MainWindow::buffer[1024] = { 0 };
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTextCursor cursor=ui->te->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->te->setTextCursor(cursor);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setLogTxt(char* fmt, ...)
{
    memset(buffer, 0, sizeof(buffer));
    va_list     argptr;
    va_start(argptr, fmt);
    int         cnt = vsprintf(buffer, fmt, argptr);
    va_end(argptr);

    ui->te->insertPlainText(QString::fromLocal8Bit(buffer));
    printf("%s", buffer);
}

void MainWindow::onCardReadProgress(void* userData, unsigned int nProgress)
{
}

int MainWindow::readCardInfo(int nindex)
{
    std::string szServerIP = "id.yzfuture.cn";
    int			nServerPort = 443;

    bool		bsuccess(false);
    std::string	szerror = "";
    int			nerr;

    YZWLHandle hlHandle = cardOpenDeviceEx(
        (char*)szServerIP.c_str(),
        nServerPort,
        2,
        nerr,
        nindex);
    if (hlHandle >= 0)
    {
        cardBeep(hlHandle);

        char	arrcardNo[32] = { 0 };
        int		ncardNo = sizeof(arrcardNo);
        if (getACardNo(hlHandle, arrcardNo, ncardNo))
        {
            setLogTxt("getACardNo[%s]\r\n", arrcardNo);
        }
        else
        {
            setLogTxt("The card is no a type\r\n");
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
                    setLogTxt("cardGetCardSN[%s]\r\n", szSN);

                    CardInfoStruct cardinfo;
                    bool		bret = cardReadTwoCardEx(hlHandle, &onCardReadProgress, NULL, cardinfo);
                    int		nSerialNumber(0);
                    char	szSerialNumber[1024] = { 0 };
                    cardGetSerialNumber(hlHandle, szSerialNumber, nSerialNumber);
                    setLogTxt("Serial Number [%s]\r\n", szSerialNumber);
                    if (!bret)
                    {
                        int		nlen(0);
                        char	szError[1024] = { 0 };
                        cardGetErrorInfo(hlHandle, szError, nlen, cardGetLastErrorCode(hlHandle));
                    }
                    else
                    {
                        bsuccess = true;
                        setLogTxt("Decode success, converting Unicode to ANSI to get Chinese  +++++++++++++\r\n");

                        unsigned char*	lpphoto = nullptr;
                        if (cardinfo.etype == (char)eOldForeignerType)
                        {
                            setLogTxt("\r\n========Foreigner's Residence Permit(old)==========\r\n");
                            ForeignerInfoOld	oldInfo;
                            memset(&oldInfo, 0, sizeof(ForeignerInfoOld));
                            convertUncodeToUtf8(cardinfo.info.foreigner.arrEnName, sizeof(cardinfo.info.foreigner.arrEnName), oldInfo.arrEnName);
                            setLogTxt("EnName:%s\r\n", oldInfo.arrEnName);
                            convertUncodeToUtf8(cardinfo.info.foreigner.arrName, sizeof(cardinfo.info.foreigner.arrName), oldInfo.arrName);
                            setLogTxt("Name:%s\r\n", oldInfo.arrName);
                            convertUncodeToUtf8(cardinfo.info.foreigner.arrSex, sizeof(cardinfo.info.foreigner.arrSex), oldInfo.arrSex);
                            setLogTxt("Sex:%s\r\n", oldInfo.arrSex);
                            convertUncodeToUtf8(cardinfo.info.foreigner.arrNo, sizeof(cardinfo.info.foreigner.arrNo), oldInfo.arrNo);
                            setLogTxt("NO:%s\r\n", oldInfo.arrNo);
                            convertUncodeToUtf8(cardinfo.info.foreigner.arrCountry, sizeof(cardinfo.info.foreigner.arrCountry), oldInfo.arrCountry);
                            setLogTxt("Country:%s\r\n", oldInfo.arrCountry);
                            convertUncodeToUtf8(cardinfo.info.foreigner.arrBirthday, sizeof(cardinfo.info.foreigner.arrBirthday), oldInfo.arrBirthday);
                            setLogTxt("Birthday:%s\r\n", oldInfo.arrBirthday);
                            convertUncodeToUtf8(cardinfo.info.foreigner.arrValidityPeriodBegin, sizeof(cardinfo.info.foreigner.arrValidityPeriodBegin), oldInfo.arrValidityPeriodBegin);
                            setLogTxt("BeginData:%s\r\n", oldInfo.arrValidityPeriodBegin);
                            convertUncodeToUtf8(cardinfo.info.foreigner.arrValidityPeriodEnd, sizeof(cardinfo.info.foreigner.arrValidityPeriodEnd), oldInfo.arrValidityPeriodEnd);
                            setLogTxt("EndDate:%s\r\n", oldInfo.arrValidityPeriodEnd);
                            lpphoto = cardinfo.info.foreigner.arrPhoto;
                        }
                        else if (cardinfo.etype == (char)eNewForeignerType)
                        {
                            setLogTxt("\r\n========Foreigner's Residence Permit(new)==========\r\n");
                            ForeignerInfoNew newForeigner;
                            convertUncodeToUtf8(cardinfo.info.newForeigner.arrEnName, sizeof(cardinfo.info.newForeigner.arrEnName), newForeigner.arrEnName);
                            setLogTxt("EnName:%s\r\n", newForeigner.arrEnName);
                            convertUncodeToUtf8(cardinfo.info.newForeigner.arrName, sizeof(cardinfo.info.newForeigner.arrName), newForeigner.arrName);
                            setLogTxt("Name:%s\r\n", newForeigner.arrName);
                            convertUncodeToUtf8(cardinfo.info.newForeigner.arrNo, sizeof(cardinfo.info.newForeigner.arrNo), newForeigner.arrNo);
                            setLogTxt("NO:%s\r\n", newForeigner.arrNo);
                            convertUncodeToUtf8(cardinfo.info.newForeigner.arrCountry, sizeof(cardinfo.info.newForeigner.arrCountry), newForeigner.arrCountry);
                            setLogTxt("Country:%s\r\n", newForeigner.arrCountry);
                            convertUncodeToUtf8(cardinfo.info.newForeigner.arrBirthday, sizeof(cardinfo.info.newForeigner.arrBirthday), newForeigner.arrBirthday);
                            setLogTxt("Birthday:%s\r\n", newForeigner.arrBirthday);
                            convertUncodeToUtf8(cardinfo.info.newForeigner.arrValidityPeriodBegin, sizeof(cardinfo.info.newForeigner.arrValidityPeriodBegin), newForeigner.arrValidityPeriodBegin);
                            setLogTxt("BeginData:%s\r\n", newForeigner.arrValidityPeriodBegin);
                            convertUncodeToUtf8(cardinfo.info.newForeigner.arrValidityPeriodEnd, sizeof(cardinfo.info.newForeigner.arrValidityPeriodEnd), newForeigner.arrValidityPeriodEnd);
                            setLogTxt("EndDate:%s\r\n", newForeigner.arrValidityPeriodEnd);
                            lpphoto = cardinfo.info.newForeigner.arrPhoto;
                        }
                        else
                        {
                            if (cardinfo.etype == (char)eTwoGATType) setLogTxt("\r\n========Hong Kong, Macao, and Taiwan Residence Permit==========\r\n");
                            else setLogTxt("\r\n========TwoID==========\r\n");
                            TwoIdInfoStruct twoId;
                            convertUncodeToUtf8(cardinfo.info.twoId.arrNo, sizeof(cardinfo.info.twoId.arrNo), twoId.arrNo);
                            setLogTxt("NO:%s\r\n", twoId.arrNo);
                            convertUncodeToUtf8(cardinfo.info.twoId.arrName, sizeof(cardinfo.info.twoId.arrName), twoId.arrName);
                            setLogTxt("Name:%s\r\n", twoId.arrName);
                            convertUncodeToUtf8(cardinfo.info.twoId.arrAddress, sizeof(cardinfo.info.twoId.arrAddress), twoId.arrAddress);
                            setLogTxt("Address:%s\r\n", twoId.arrAddress);
                            convertUncodeToUtf8(cardinfo.info.twoId.arrBirthday, sizeof(cardinfo.info.twoId.arrBirthday), twoId.arrBirthday);
                            setLogTxt("Birthday:%s\r\n", twoId.arrBirthday);
                            convertUncodeToUtf8(cardinfo.info.twoId.arrValidityPeriodBegin, sizeof(cardinfo.info.twoId.arrValidityPeriodBegin), twoId.arrValidityPeriodBegin);
                            convertUncodeToUtf8(cardinfo.info.twoId.arrValidityPeriodEnd, sizeof(cardinfo.info.twoId.arrValidityPeriodEnd), twoId.arrValidityPeriodEnd);
                            setLogTxt("Date:%s~%s\r\n", twoId.arrValidityPeriodBegin, twoId.arrValidityPeriodEnd);
                            convertUncodeToUtf8(cardinfo.info.twoId.arrSignedDepartment, sizeof(cardinfo.info.twoId.arrSignedDepartment), twoId.arrSignedDepartment);
                            setLogTxt("SignedDepartment:%s\r\n", twoId.arrSignedDepartment);
                            lpphoto = cardinfo.info.twoId.arrPhoto;
                        }
                        if (lpphoto)
                        {
                            char*   szBmp = new char[1024*40]{0};
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
                            if (szBmp)
                            {
                                delete szBmp;
                                szBmp = NULL;
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
            setLogTxt("setcardtype failed[%d]\r\n", cardGetLastErrorCode(hlHandle));
        }
    }
    else
    {
        int		nlen(0);
        char	szError[1024] = { 0 };
        cardGetErrorInfo(hlHandle, szError, nlen, -19997);
        setLogTxt("open device failed\r\n");
    }
    if (hlHandle >= 0)
    {
        cardCloseDevice(hlHandle);
        hlHandle = -1;
    }
    return 0;
}


void MainWindow::on_btn_clicked()
{
    int     nvalue = ui->cb->currentText().toInt();
    readCardInfo(nvalue);
}
