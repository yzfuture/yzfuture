VERSION 5.00
Begin VB.Form Form4 
   Caption         =   "���֤"
   ClientHeight    =   5025
   ClientLeft      =   120
   ClientTop       =   465
   ClientWidth     =   8700
   LinkTopic       =   "Form3"
   ScaleHeight     =   5025
   ScaleWidth      =   8700
   StartUpPosition =   3  '����ȱʡ
   Begin VB.CheckBox Check1 
      Caption         =   "���߶�����"
      Height          =   495
      Left            =   5160
      TabIndex        =   1
      Top             =   3720
      Width           =   1335
   End
   Begin VB.CommandButton Command1 
      Caption         =   "�����֤"
      Height          =   495
      Left            =   6840
      TabIndex        =   0
      Top             =   3720
      Width           =   1215
   End
End
Attribute VB_Name = "Form4"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Type TwoIdInfoStructEx
    arrTwoIdName As String * 30            '���� UNICODE
    arrTwoIdSex As String * 2               '�Ա� UNICODE
    arrTwoIdNation As String * 4            '���� UNICODE
    arrTwoIdBirthday As String * 16         '�������� UNICODE YYYYMMDD
    arrTwoIdAddress As String * 70               'סַ UNICODE
    arrTwoIdNo As String * 36               '���֤���� UNICODE
    arrTwoIdSignedDepartment As String * 30 'ǩ������ UNICODE
    arrTwoIdValidityPeriodBegin As String * 16 '��Ч����ʼ���� UNICODE YYYYMMDD
    arrTwoIdValidityPeriodEnd As String * 16   '��Ч�ڽ�ֹ���� UNICODE YYYYMMDD ��Ч��Ϊ����ʱ�洢�����ڡ�
    
    arrTwoOtherNO As String * 18            'ͨ��֤�����
    arrTwoSignNum As String * 4             'ǩ������
    arrTwoRemark1 As String * 6             'Ԥ����
    arrTwoType As String * 2                '֤�����ͱ�ʶ
    arrTwoRemark2 As String * 6             'Ԥ����
    
    arrTwoIdNewAddress As String * 70       '����סַ UNICODE
    arrReserve As String * 2                '�����ֽ� �ֽڶ�����
    arrTwoIdPhoto As String * 1024          '��Ƭ��Ϣ
    arrTwoIdFingerprint As String * 1024    'ָ����Ϣ
    arrTwoIdPhotoJpeg As String * 4096      '��Ƭ��Ϣ JPEG ��ʽ
    unTwoIdPhotoJpegLength As Long          '��Ƭ��Ϣ���� JPEG��ʽ
End Type

Private Declare Sub cardReadInit Lib "readCardInfo.dll" ()

Private Declare Function loginCardServer Lib "readCardInfo.dll" (ByVal szServerIp As String, ByVal nServerPort As Long, ByVal szAppKey As String, ByVal szAppSecret As String, ByVal userData As String, nerr As Long) As Boolean

Private Declare Function loginCardServerEx Lib "readCardInfo.dll" (ByVal szServerIp As String, ByVal nServerPort As Long, nerr As Long) As Boolean

Private Declare Sub setDeviceType Lib "readCardInfo.dll" (ByVal nDeviceType As Long)

Private Declare Function cardOpenDevice Lib "readCardInfo.dll" (ByVal nouttime As Long , nerr As Long, ByVal nDeviceNo As Long) As Boolean

Private Declare Function setCardType Lib "readCardInfo.dll" (ByVal nDeviceHandle As Long, ByVal ctype As Long) As Boolean
Private Declare Function cardFindCard Lib "readCardInfo.dll" (ByVal nDeviceHandle As Long, bmove As Long) As Boolean
Private Declare Function cardSelectCard Lib "readCardInfo.dll" (ByVal nDeviceHandle As Long) As Boolean
Private Declare Function cardReadTwoCard Lib "readCardInfo.dll" (ByVal nDeviceHandle As Long, ByVal cardCB As Long, ByRef cardinfo As TwoIdInfoStructEx) As Boolean
Private Declare Function cardBeep Lib "readCardInfo.dll" (ByVal nDeviceHandle As Long) As Boolean

Private Declare Sub cardCloseDevice Lib "readCardInfo.dll" (ByVal nDeviceHandle As Long)

Private Declare Function decodeCardImage Lib "readCardInfo.dll" (ByVal srcimage As String, ByVal outimage As String, outlen As Long) As Boolean

Private Declare Sub logoutCardServer Lib "readCardInfo.dll" ()

Private Declare Sub cardReadUninit Lib "readCardInfo.dll" ()

            /*
             * cardReadInit
             * loginCardServerEx
             * logoutCardServer
             * cardReadUninit
             * �����ĸ��ӿھ��Լ������Լ��ĳ����߼������˴�ֻ��չʾ�÷���Ϊʾ����
             */


Private Sub Command1_Click()
    //Dim szAppKey As String: szAppKey = "����ա�NFC����ע������ V2.pdf������"
    //Dim szAppSecret As String: szAppSecret = "����ա�NFC����ע������ V2.pdf������"
    //Dim szUserData As String: szUserData = "����ա�NFC����ע������ V2.pdf������"
    Dim szip As String: szip = "id.yzfuture.cn"
    Dim yzwlHandle As Long: yzwlHandle = -1
    Dim nDeviceNo As Long: nDeviceNo = 0
    Dim tmpCardInfo As TwoIdInfoStructEx
    
    If Form4.Check1.Value = 1 Then
        nDeviceNo = 1001
        setDeviceType (1)
    Else
        nDeviceNo = 0
        setDeviceType (0)
    End If

    Dim nerr As Long
    If loginCardServerEx(szip, 443, nerr) Then
        yzwlHandle = cardOpenDevice(2, nerr, 0)
        If yzwlHandle > 0 Then
            If setCardType(yzwlHandle, 1) Then
                Dim bmove As Long
                If cardFindCard(yzwlHandle, bmove) Then
                    If cardSelectCard(yzwlHandle) Then
                        If cardReadTwoCard(yzwlHandle, 0, tmpCardInfo) Then
                            Call cardBeep(yzwlHandle)
                            Debug.Print "����ɹ�:"
                            Debug.Print StrConv(tmpCardInfo.arrTwoIdName, vbFromUnicode)
                        End If
                    End If
                End If
            End If
            Call cardCloseDevice(a)
        End If
    End If
    logoutCardServer()
End Sub
Private Sub Form_Load()
    Call cardReadInit
End Sub
Private Sub Form_Unload(Cancel As Integer)
    Call cardReadUninit
End Sub
