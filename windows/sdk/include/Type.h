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
