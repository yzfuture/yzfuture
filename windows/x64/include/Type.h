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


#pragma pack(push)
#pragma pack(1)
//����֤��Ϣ�ṹ
typedef struct _TwoIdInfoStruct 
{
	char arrName[30];					//���� UNICODE
	char arrSex[2];					//�Ա� UNICODE
	char arrNation[4];					//���� UNICODE
	char arrBirthday[16];				//�������� UNICODE YYYYMMDD
	char arrAddress[70];				//סַ UNICODE
	char arrNo[36];					//���֤���� UNICODE
	char arrSignedDepartment[30];		//ǩ������ UNICODE
	char arrValidityPeriodBegin[16];	//��Ч����ʼ���� UNICODE YYYYMMDD
	char arrValidityPeriodEnd[16];		//��Ч�ڽ�ֹ���� UNICODE YYYYMMDD ��Ч��Ϊ����ʱ�洢�����ڡ�

	char arrOtherNO[18]; // ͨ��֤�����
	char arrSignNum[4];		// ǩ������
	char arrRemark1[6];	// Ԥ����
	char arrType[2];		// ֤�����ͱ�ʶ
	char arrRemark2[6];	// Ԥ����

	unsigned char arrPhoto[1024];		//��Ƭ��Ϣ
	unsigned char arrFingerprint[1024];//ָ����Ϣ
}TwoIdInfoStruct;

typedef struct _ForeignerInfoOld // ��������þ�ס֤
{
	char arrEnName[120];					//Ӣ����
	char arrSex[2];					//�Ա� UNICODE
	char arrNo[30];					//15���ַ��ľ���֤���� UNICODE
	char arrCountry[6];				//���� UNICODE GB/T2659-2000
	char arrName[30];				//�������� UNICODE ���û��������������ȫΪ0x0020
	char arrValidityPeriodBegin[16];//ǩ������ UNICODE YYYYMMDD
	char arrValidityPeriodEnd[16];	//��ֹ���� UNICODE YYYYMMDD
	char arrBirthday[16];			//�������� UNICODE YYYYMMDD

	char arrVersion[4]; // �汾��
	char arrSignedDepartment[8];		//ǩ�����ش��� UNICODE ֤��оƬ�ڲ��洢ǩ������
	char arrType[2];		// ֤�����ͱ�ʶ
	char arrRemark2[6];	// Ԥ����

	unsigned char arrPhoto[1024];		//��Ƭ��Ϣ
	unsigned char arrFingerprint[1024];//ָ����Ϣ
}ForeignerInfoOld;

typedef struct _ForeignerInfoNew  // ��������þ�ס֤�°�
{
	char arrName[30];					//���� UNICODE
	char arrSex[2];					//�Ա� UNICODE
	char arrNation[4];					//���� UNICODE
	char arrBirthday[16];				//�������� UNICODE YYYYMMDD
	char arrEnName[70];				//�������� UNICODE
	char arrNo[36];					//���֤���� UNICODE
	char arrSignedDepartment[30];		//ǩ������ UNICODE
	char arrValidityPeriodBegin[16];	//��Ч����ʼ���� UNICODE YYYYMMDD
	char arrValidityPeriodEnd[16];		//��Ч�ڽ�ֹ���� UNICODE YYYYMMDD ��Ч��Ϊ����ʱ�洢�����ڡ�

	char arrOtherNO[18]; // ͨ��֤�����
	char arrSignNum[4];		// ǩ������
	char arrCountry[6];		//���� UNICODE GB/T2659-2000
	char arrType[2];		// ֤�����ͱ�ʶ
	char arrRemark2[6];	// Ԥ����

	unsigned char arrPhoto[1024];		//��Ƭ��Ϣ
	unsigned char arrFingerprint[1024];//ָ����Ϣ
}ForeignerInfoNew;

typedef enum _eCardFormatType
{
	eTwoIDType = ' ', // ���֤
	eTwoGATType = 'J', // �۰�̨�����ס֤
	eOldForeignerType = 'I', // ��������þ������֤
	eNewForeignerType = 'Y', // ��������þ������֤(�°�)
}eCardFormatType;

typedef struct _CardInfoStruct
{
	char	etype; // eCardFormatType
	union
	{
		TwoIdInfoStruct	 twoId; // ���֤/�۰�̨�����ס֤
		ForeignerInfoOld foreigner; // �ɰ���������þ�ס֤
		ForeignerInfoNew newForeigner; // �°���������þ�ס֤
	}info;
}CardInfoStruct;

#pragma pack(pop)