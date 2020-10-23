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
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
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
    arrTwoIdName As String * 30            '姓名 UNICODE
    arrTwoIdSex As String * 2               '性别 UNICODE
    arrTwoIdNation As String * 4            '民族 UNICODE
    arrTwoIdBirthday As String * 16         '出生日期 UNICODE YYYYMMDD
    arrTwoIdAddress As String * 70               '住址 UNICODE
    arrTwoIdNo As String * 36               '身份证号码 UNICODE
    arrTwoIdSignedDepartment As String * 30 '签发机关 UNICODE
    arrTwoIdValidityPeriodBegin As String * 16 '有效期起始日期 UNICODE YYYYMMDD
    arrTwoIdValidityPeriodEnd As String * 16   '有效期截止日期 UNICODE YYYYMMDD 有效期为长期时存储“长期”
    
    arrTwoOtherNO As String * 18            '通行证类号码
    arrTwoSignNum As String * 4             '签发次数
    arrTwoRemark1 As String * 6             '预留区
    arrTwoType As String * 2                '证件类型标识
    arrTwoRemark2 As String * 6             '预留区
    
    arrTwoIdNewAddress As String * 70       '最新住址 UNICODE
    arrReserve As String * 2                '保留字节 字节对齐用
    arrTwoIdPhoto As String * 1024          '照片信息
    arrTwoIdFingerprint As String * 1024    '指纹信息
    arrTwoIdPhotoJpeg As String * 4096      '照片信息 JPEG 格式
    unTwoIdPhotoJpegLength As Long          '照片信息长度 JPEG格式
End Type

Private Declare Function cardOpenDevice Lib "readCardInfo.dll" (ByVal nouttime As Long, ByVal nDeviceNo As Long) As Long
Private Declare Function cardReadTwoCard Lib "readCardInfo.dll" (ByVal nDeviceHandle As Long, ByVal cardCB As Long, ByVal szFactoryFlag As String, ByVal szServerIp As String, ByVal nServerPort As Long, ByRef cardinfo As TwoIdInfoStructEx, ByVal bTest As Boolean) As Boolean
Private Declare Function cardCloseDevice Lib "readCardInfo.dll" (ByVal nDeviceHandle As Long) As Long
Private Declare Function decodeCardImage Lib "readCardInfo.dll" (ByVal srcimage As String, ByVal outimage As String, outlen As Long) As Boolean
Private Declare Function cardBeep Lib "readCardInfo.dll" (ByVal nDeviceHandle As Long) As Long



Private Sub Command1_Click()
    Dim a As Long, b As Boolean, tmpCardInfo As TwoIdInfoStructEx
    a = cardOpenDevice(2, 0)
    Debug.Print a
    If a > 0 Then
        b = cardReadTwoCard(a, 0, "99ffb2f98a29071107c7a09ad2c6d096", "id.yzfuture.cn", 8848, tmpCardInfo, False)
        Call cardBeep(a)
        Debug.Print "cardReadTwoCard:", b
        Debug.Print StrConv(tmpCardInfo.arrTwoIdName, vbFromUnicode)
        
        'Debug.Print StrConv(tmpCardInfo.arrTwoIdName, vbWide)
        Debug.Print StrConv(tmpCardInfo.arrTwoIdAddress, vbFromUnicode)
        'Debug.Print StrConv(tmpCardInfo.arrTwoIdAddress, vbUnicode)
        Call cardCloseDevice(a)
    Debug.Print "ok"
    End If
End Sub

Private Sub Form_Load()
    'DecodeIP = id.yzfuture.cn
'DecodePort = 8848
'APPKEY=99ffb2f98a29071107c7a09ad2c6d096
End Sub
