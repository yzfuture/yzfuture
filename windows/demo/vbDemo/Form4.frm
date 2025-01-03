VERSION 5.00
Begin VB.Form Form4 
   Caption         =   "身份证"
   ClientHeight    =   5025
   ClientLeft      =   120
   ClientTop       =   465
   ClientWidth     =   8700
   LinkTopic       =   "Form3"
   ScaleHeight     =   5025
   ScaleWidth      =   8700
   StartUpPosition =   3  '窗口缺省
   Begin VB.CheckBox Check1 
      Caption         =   "离线读卡器"
      Height          =   495
      Left            =   5160
      TabIndex        =   1
      Top             =   3720
      Width           =   1335
   End
   Begin VB.CommandButton Command1 
      Caption         =   "读身份证"
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
    arrTwoIdName(29) As Byte            '姓名 UNICODE
    arrTwoIdSex(1) As Byte               '性别 UNICODE
    arrTwoIdNation(3) As Byte            '民族 UNICODE
    arrTwoIdBirthday(15) As Byte         '出生日期 UNICODE YYYYMMDD
    arrTwoIdAddress(69) As Byte               '住址 UNICODE
    arrTwoIdNo(35) As Byte               '身份证号码 UNICODE
    arrTwoIdSignedDepartment(29) As Byte '签发机关 UNICODE
    arrTwoIdValidityPeriodBegin(15) As Byte '有效期起始日期 UNICODE YYYYMMDD
    arrTwoIdValidityPeriodEnd(15) As Byte   '有效期截止日期 UNICODE YYYYMMDD 有效期为长期时存储“长期”
    
    arrTwoOtherNO(17) As Byte            '通行证类号码
    arrTwoSignNum(3) As Byte             '签发次数
    arrTwoRemark1(5) As Byte             '预留区
    arrTwoType(1) As Byte                '证件类型标识
    arrTwoRemark2(5) As Byte             '预留区
    
    arrTwoIdNewAddress(69) As Byte       '最新住址 UNICODE
    arrReserve(1) As Byte                '保留字节 字节对齐用
    arrTwoIdPhoto(1023) As Byte          '照片信息
    arrTwoIdFingerprint(1023) As Byte    '指纹信息
    arrTwoIdPhotoJpeg(4095) As Byte      '照片信息 JPEG 格式
    unTwoIdPhotoJpegLength As Long          '照片信息长度 JPEG格式
End Type

Private Declare Sub cardReadInit Lib "readCardInfo.dll" ()

Private Declare Function loginCardServer Lib "readCardInfo.dll" (ByVal szServerIp As String, ByVal nServerPort As Long, ByVal szAppKey As String, ByVal szAppSecret As String, ByVal userData As String, nerr As Long) As Boolean

Private Declare Function loginCardServerEx Lib "readCardInfo.dll" (ByVal szServerIp As String, ByVal nServerPort As Long, nerr As Long) As Boolean

Private Declare Sub setDeviceType Lib "readCardInfo.dll" (ByVal nDeviceType As Long)

Private Declare Function cardOpenDevice Lib "readCardInfo.dll" (ByVal nouttime As Long , nerr As Long, ByVal nDeviceNo As Long) As Long

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
             * 以上四个接口就自己按照自己的程序逻辑处理，此处只是展示用法做为示例用
             */


Private Sub Command1_Click()
    //Dim szAppKey As String: szAppKey = "请参照《NFC服务注册流程 V2.pdf》申请"
    //Dim szAppSecret As String: szAppSecret = "请参照《NFC服务注册流程 V2.pdf》申请"
    //Dim szUserData As String: szUserData = "请参照《NFC服务注册流程 V2.pdf》申请"
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
    If loginCardServerEx(szip, 443, nerr) Then // 离线读卡器不用登录，直接打开设备就可以用，离线读卡器，这句可以屏蔽
        yzwlHandle = cardOpenDevice(2, nerr, 0)
        If yzwlHandle > 0 Then
            If setCardType(yzwlHandle, 1) Then
                Dim bmove As Long
                If cardFindCard(yzwlHandle, bmove) Then
                    If cardSelectCard(yzwlHandle) Then
                        If cardReadTwoCard(yzwlHandle, 0, tmpCardInfo) Then
                            Call cardBeep(yzwlHandle)
                            Debug.Print "解码成功:"
                            Debug.Print tmpCardInfo.arrTwoIdName
                            Debug.Print tmpCardInfo.arrTwoIdSex
                            Debug.Print tmpCardInfo.arrTwoIdNation
                            Debug.Print tmpCardInfo.arrTwoIdBirthday
                            Debug.Print tmpCardInfo.arrTwoIdAddress
                            Debug.Print tmpCardInfo.arrTwoIdNo
                            Debug.Print tmpCardInfo.arrTwoIdSignedDepartment
                            Debug.Print tmpCardInfo.arrTwoIdValidityPeriodBegin
                            Debug.Print tmpCardInfo.arrTwoIdValidityPeriodEnd
                        End If
                    End If
                End If
            End If
            Call cardCloseDevice(a)
        End If
    End If
    logoutCardServer() // 离线读卡器不用登录，这句可以屏蔽
End Sub
Private Sub Form_Load()
    Call cardReadInit
End Sub
Private Sub Form_Unload(Cancel As Integer)
    Call cardReadUninit
End Sub
