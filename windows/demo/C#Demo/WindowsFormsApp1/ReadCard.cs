using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    public static class ReadCard
    {
        [DllImport("readCardInfo.dll")]//readCardInfo.dll
        public static extern void cardReadInit();// 初始化操作

        [DllImport("readCardInfo.dll")]//readCardInfo.dll
        public static extern void setDeviceType(int nDeviceType);// 0-标准读卡器(默认) 1-离线读卡器

        [DllImport("readCardInfo.dll", CharSet = CharSet.Ansi, ExactSpelling = false,
             CallingConvention = CallingConvention.StdCall)]//readCardInfo.dll
        public static extern bool loginCardServerEx(string szServerIp, int nServerPort,  ref int nerr);//登录解码服务器

        // szAppKey:请参照《NFC服务注册流程 V2.pdf》申请
        // szAppSecret:请参照《NFC服务注册流程 V2.pdf》申请
        // szUserData:请参照《NFC服务注册流程 V2.pdf》申请
        [DllImport("readCardInfo.dll", CharSet = CharSet.Ansi, ExactSpelling = false,
             CallingConvention = CallingConvention.StdCall)]//readCardInfo.dll
        public static extern bool loginCardServer(string szServerIp, int nServerPort,
            string szAppKey, string szAppSecret, string szAppUserId, ref int nerr);//登录解码服务器

        [DllImport("readCardInfo.dll", CharSet = CharSet.Ansi, ExactSpelling = false,
             CallingConvention = CallingConvention.StdCall)]//readCardInfo.dll
        public static extern int cardOpenDevice(int nouttime, ref int nerr, int nDeviceNo);//打开读卡器硬件设备

        [DllImport("readCardInfo.dll")]//readCardInfo.dll
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool setCardType(int nDeviceHandle, int ctype);//设置卡片类型 0-A卡   1-B卡


        [DllImport("readCardInfo.dll")]//readCardInfo.dll
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool cardFindCard(int nDeviceHandle, ref bool bmove);//寻卡

        [DllImport("readCardInfo.dll",CharSet = CharSet.Ansi, ExactSpelling = false,
             CallingConvention = CallingConvention.StdCall)]//readCardInfo.dll
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool cardReadTwoCard(int nDeviceHandle, int cardCB, ref TwoIdInfoStruct cardinfo);//读卡

        [DllImport("readCardInfo.dll")]//readCardInfo.dll
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool decodeCardImage(byte[] srcimage, byte[] outimage, ref int outlen);

        [DllImport("readCardInfo.dll")]//readCardInfo.dll
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool cardCloseDevice(int nDeviceHandle);//关闭

        [DllImport("readCardInfo.dll")]//readCardInfo.dll
        public static extern void logoutCardServer();// 登出服务器

        [DllImport("readCardInfo.dll")]//readCardInfo.dll
        public static extern void cardReadUninit();// 反初始化操作

        public static bool decodeImage(byte[] srcimage, byte[] outimage, ref int outlen)
        {
            return decodeCardImage(srcimage, outimage, ref outlen);
        }

        public static TwoIdInfoStruct ReadCardNo(Boolean bonLine)
        {
            TwoIdInfoStruct sttTwoIdInfo = new TwoIdInfoStruct();
            sttTwoIdInfo.arrTwoIdName = new byte[30];					//姓名 UNICODE
            sttTwoIdInfo.arrTwoIdSex = new byte[2];					//性别 UNICODE
            sttTwoIdInfo.arrTwoIdNation = new byte[4];					//民族 UNICODE
            sttTwoIdInfo.arrTwoIdBirthday = new byte[16];				//出生日期 UNICODE YYYYMMDD
            sttTwoIdInfo.arrTwoIdAddress = new byte[70];				//住址 UNICODE
            sttTwoIdInfo.arrTwoIdNo = new byte[36];					//身份证号码 UNICODE
            sttTwoIdInfo.arrTwoIdSignedDepartment = new byte[30];		//签发机关 UNICODE
            sttTwoIdInfo.arrTwoIdValidityPeriodBegin = new byte[16];	//有效期起始日期 UNICODE YYYYMMDD
            sttTwoIdInfo.arrTwoIdValidityPeriodEnd = new byte[16];		//有效期截止日期 UNICODE YYYYMMDD 有效期为长期时存储“长期”
            
            sttTwoIdInfo.arrTwoOtherNO = new byte[18];
            sttTwoIdInfo.arrTwoSignNum = new byte[4];
            sttTwoIdInfo.arrTwoRemark1 = new byte[6];
            sttTwoIdInfo.arrTwoType = new byte[2];
            sttTwoIdInfo.arrTwoRemark2 = new byte[6];

            sttTwoIdInfo.arrTwoIdNewAddress = new byte[70];			//最新住址 UNICODE
            sttTwoIdInfo.arrReserve = new byte[2];			//最新住址 UNICODE
            sttTwoIdInfo.arrTwoIdPhoto = new byte[1024];		//照片信息
            sttTwoIdInfo.arrTwoIdFingerprint = new byte[1024];//指纹信息
            sttTwoIdInfo.arrTwoIdPhotoJpeg = new byte[4096];	//照片信息 JPEG 格式
            sttTwoIdInfo.unTwoIdPhotoJpegLength = 0;	//照片信息长度 JPEG格式

            /*
             * cardReadInit
             * loginCardServer
             * logoutCardServer
             * cardReadUninit
             * 以上四个接口就自己按照自己的程序逻辑处理，此处只是展示用法做为示例用
             */
            cardReadInit();
            string szip = "id.yzfuture.cn";
            int nindex = 0;
            int nerr = 0;
            if (loginCardServerEx(szip, 443, ref nerr))
            {
                if (!bonLine)
                {
                    nindex = 0;
                    setDeviceType(0);
                }
                else
                {
                    nindex = 1001;
                    setDeviceType(1);
                }

                int hlHandle = cardOpenDevice(2, ref nerr, nindex);
                if (hlHandle > 0)
                {
                    bool bmove = true;
                    if (setCardType(hlHandle, 1))
                    {
                        if (cardFindCard(hlHandle, ref bmove))
                        {
                            int cb = 0;
                            bool bret = cardReadTwoCard(hlHandle, cb, ref sttTwoIdInfo);
                            if (!bret)
                            {
                                MessageBox.Show("解码失败请重试");
                            }
                        }
                    }
                    cardCloseDevice(hlHandle);
                }
            }
            logoutCardServer();
            cardReadUninit();
            return sttTwoIdInfo;

        }
    }

    public struct TwoIdInfoStruct
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 30)]
        public byte[] arrTwoIdName;					//姓名 UNICODE
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        public byte[] arrTwoIdSex;					//性别 UNICODE
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
        public byte[] arrTwoIdNation;					//民族 UNICODE
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] arrTwoIdBirthday;				//出生日期 UNICODE YYYYMMDD
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 70)]
        public byte[] arrTwoIdAddress;				//住址 UNICODE
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 36)]
        public byte[] arrTwoIdNo;					//身份证号码 UNICODE
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 30)]
        public byte[] arrTwoIdSignedDepartment;		//签发机关 UNICODE
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] arrTwoIdValidityPeriodBegin;	//有效期起始日期 UNICODE YYYYMMDD
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] arrTwoIdValidityPeriodEnd;		//有效期截止日期 UNICODE YYYYMMDD 有效期为长期时存储“长期”
        
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 18)]
        public byte[] arrTwoOtherNO;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
        public byte[] arrTwoSignNum;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
        public byte[] arrTwoRemark1;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        public byte[] arrTwoType;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
        public byte[] arrTwoRemark2;
        
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 70)]
        public byte[] arrTwoIdNewAddress;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        public byte[] arrReserve;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 1024)]
        public byte[] arrTwoIdPhoto;		//照片信息
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 1024)]
        public byte[] arrTwoIdFingerprint;//指纹信息
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4096)]
        public byte[] arrTwoIdPhotoJpeg;	//照片信息 JPEG 格式
        [MarshalAs(UnmanagedType.U4)]
        public UInt32 unTwoIdPhotoJpegLength;	//照片信息长度 JPEG格式
    }

    public class CardType
    {
        private int _unkwonType = -1;
        private int _ACardType = 0; // A卡
        private int _BCardType = 1;  // B卡

    }

}
