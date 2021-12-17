#pragma once

#include <stdint.h>
				
#ifdef FOR_WINDOWS10
typedef long long YZWLHandle;				
#else
typedef long YZWLHandle;				
#endif

typedef enum cardType
{
	unkwonType = -1,
	ACardType = 0,
	BCardType = 1
};

typedef struct TwoIdInfoStructEx
{
	char arrTwoIdName[30];					//���� UNICODE
	char arrTwoIdSex[2];					//�Ա� UNICODE
	char arrTwoIdNation[4];					//���� UNICODE
	char arrTwoIdBirthday[16];				//�������� UNICODE YYYYMMDD
	char arrTwoIdAddress[70];				//סַ UNICODE
	char arrTwoIdNo[36];					//���֤���� UNICODE
	char arrTwoIdSignedDepartment[30];		//ǩ������ UNICODE
	char arrTwoIdValidityPeriodBegin[16];	//��Ч����ʼ���� UNICODE YYYYMMDD
	char arrTwoIdValidityPeriodEnd[16];		//��Ч�ڽ�ֹ���� UNICODE YYYYMMDD ��Ч��Ϊ����ʱ�洢�����ڡ�

	char arrTwoOtherNO[18]; // ͨ��֤�����
	char arrTwoSignNum[4];		// ǩ������
	char arrTwoRemark1[6];	// Ԥ����
	char arrTwoType[2];		// ֤�����ͱ�ʶ
	char arrTwoRemark2[6];	// Ԥ����

	char arrTwoIdNewAddress[70];			//����סַ UNICODE
	char arrReserve[2];						//�����ֽ� �ֽڶ�����
	unsigned char arrTwoIdPhoto[1024];		//��Ƭ��Ϣ
	unsigned char arrTwoIdFingerprint[1024];//ָ����Ϣ
	unsigned char arrTwoIdPhotoJpeg[4096];	//��Ƭ��Ϣ JPEG ��ʽ
	unsigned int  unTwoIdPhotoJpegLength;	//��Ƭ��Ϣ���� JPEG��ʽ
};


typedef struct _TwoForeignerInfoStruct // ��������þ�ס֤
{
	char arrEnName[120];					//Ӣ����
	char arrSex[2];					//�Ա� UNICODE
	char arrNo[30];					//15���ַ��ľ���֤���� UNICODE
	char arrNation[6];				//���� UNICODE GB/T2659-2000
	char arrCnName[30];				//�������� UNICODE ���û��������������ȫΪ0x0020
	char arrBeginDate[16];			//ǩ������ UNICODE YYYYMMDD
	char arrEndDate[16];			//��ֹ���� UNICODE YYYYMMDD
	char arrBirthday[16];			//�������� UNICODE YYYYMMDD

	char arrVersion[4]; // �汾��
	char arrSignedDepartment[8];		//ǩ�����ش��� UNICODE ֤��оƬ�ڲ��洢ǩ������
	char arrType[2];		// ֤�����ͱ�ʶ
	char arrTwoRemark2[6];	// Ԥ����

	unsigned char arrPhoto[1024];		//��Ƭ��Ϣ
	unsigned char arrFingerprint[1024];//ָ����Ϣ
}TwoForeignerInfoStruct;

typedef enum _eCardFormatType
{
	eTwoIDType = ' ', // ���֤
	eTwoGATType = 'J', // �۰�̨�����ס֤
	eTwoForeignerType = 'I', // ��������þ������֤
}eCardFormatType;

typedef struct _TwoCardInfoStruct
{
	eCardFormatType etype;
	TwoIdInfoStructEx twoInfo; // eTwoIDType/eTwoGATType
	TwoForeignerInfoStruct foreignerInfo; // eTwoForeignerType
}TwoCardInfoStruct;
