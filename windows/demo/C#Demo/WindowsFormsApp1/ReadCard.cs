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

        [DllImport("readCardInfo.dll", CharSet = CharSet.Ansi, ExactSpelling = false,
             CallingConvention = CallingConvention.StdCall)]//readCardInfo.dll
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool cardReadTwoCardEx(int nDeviceHandle, int cardCB, ref CardInfoStruct cardinfo);//读卡

        [DllImport("readCardInfo.dll")]//readCardInfo.dll
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool decodeCardImage(byte[] srcimage, byte[] outimage, ref int outlen);

        [DllImport("readCardInfo.dll")]//readCardInfo.dll
        [return: MarshalAs(UnmanagedType.I1)]
        public static extern bool twoIdToImage(TwoIdInfoStructEx twoId, byte[] outimage, ref int outlen, int ntype, int nformat);
        
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
        public static byte[] ToByteArray(CardInfoStruct cardinfo)
        {
            int size = Marshal.SizeOf(cardinfo);
            IntPtr ptr = Marshal.AllocHGlobal(size);
            Marshal.StructureToPtr(cardinfo, ptr, false);
            byte[] bytes = new byte[size];
            Marshal.Copy(ptr, bytes, 0, size);
            Marshal.FreeHGlobal(ptr);
            return bytes;
        }
        public static byte[] StructToBytes(CardInfoStruct structure)
        {
            int size = Marshal.SizeOf(structure);
            byte[] buffer = new byte[size];

            IntPtr ptr = Marshal.AllocHGlobal(size);
            try
            {
                Marshal.StructureToPtr(structure, ptr, false);
                Marshal.Copy(ptr, buffer, 0, size);
            }
            finally
            {
                Marshal.FreeHGlobal(ptr);
            }

            return buffer;
        }
        public static bool getSFZBmp(CardInfoStruct cardinfo, byte[] outimage, ref int outlen)
        {
            return twoIdToImage(cardinfo.info.twoId, outimage, ref outlen, 3, 1);
        }

        public static CardInfoStruct ReadCardNo(Boolean bonLine)
        {
            CardInfoStruct sttTwoIdInfo = new CardInfoStruct();

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
            if (bonLine)
            {
                nindex = 1001;
                setDeviceType(1);
            }
            else
            {
                nindex = 0;
                setDeviceType(0);
                if (!loginCardServerEx(szip, 443, ref nerr))
                {
                    logoutCardServer();
                    cardReadUninit();
                    return sttTwoIdInfo;
                }
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
                        bool bret = cardReadTwoCardEx(hlHandle, cb, ref sttTwoIdInfo);
                        if (!bret)
                        {
                            MessageBox.Show("解码失败请重试");
                        }
                    }
                }
                cardCloseDevice(hlHandle);
            }
            if (!bonLine)
            {
                logoutCardServer();
            }
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

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct TwoIdInfoStructEx
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 30)]
        public byte[] arrName;					//姓名 UNICODE
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        public byte[] arrSex;					//性别 UNICODE
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
        public byte[] arrNation;					//民族 UNICODE
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] arrBirthday;				//出生日期 UNICODE YYYYMMDD
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 70)]
        public byte[] arrAddress;				//住址 UNICODE
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 36)]
        public byte[] arrNo;					//身份证号码 UNICODE
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 30)]
        public byte[] arrSignedDepartment;		//签发机关 UNICODE
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] arrValidityPeriodBegin;	//有效期起始日期 UNICODE YYYYMMDD
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] arrValidityPeriodEnd;		//有效期截止日期 UNICODE YYYYMMDD 有效期为长期时存储“长期”
        
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 18)]
        public byte[] arrOtherNO;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
        public byte[] arrSignNum;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
        public byte[] arrRemark1;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        public byte[] arrType;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
        public byte[] arrRemark2;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 1024)]
        public byte[] arrPhoto;		//照片信息
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 1024)]
        public byte[] arrFingerprint;//指纹信息
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct ForeignerInfoOld
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 120)]
        public byte[] arrEnName;					//英文名
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        public byte[] arrSex;					//性别 UNICODE
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 30)]
        public byte[] arrNo;					//15个字符的居留证号码 UNICODE
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
        public byte[] arrCountry;				//国籍 UNICODE GB/T2659-2000
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 30)]
        public byte[] arrName;				//中文姓名 UNICODE 如果没有中文姓名，则全为0x0020
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] arrValidityPeriodBegin;	//签发日期 UNICODE YYYYMMDD
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] arrValidityPeriodEnd;		//终止日期 UNICODE YYYYMMDD
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] arrBirthday;		//出生日期 UNICODE YYYYMMDD

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
        public byte[] arrVersion;// 版本号
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
        public byte[] arrSignedDepartment;//签发机关代码 UNICODE 证件芯片内不存储签发机关
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        public byte[] arrType;// 证件类型标识
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
        public byte[] arrRemark2;// 预留区

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 1024)]
        public byte[] arrPhoto;		//照片信息
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 1024)]
        public byte[] arrFingerprint;//指纹信息
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct ForeignerInfoNew
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 30)]
        public byte[] arrName;					//姓名 UNICODE
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        public byte[] arrSex;					//性别 UNICODE
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
        public byte[] arrNation;					//民族 UNICODE
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] arrBirthday;				//出生日期 UNICODE YYYYMMDD
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 70)]
        public byte[] arrEnName;				//外文姓名 UNICODE
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 36)]
        public byte[] arrNo;					//身份证号码 UNICODE
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 30)]
        public byte[] arrSignedDepartment;		//签发机关 UNICODE
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] arrValidityPeriodBegin;	//有效期起始日期 UNICODE YYYYMMDD
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] arrValidityPeriodEnd;		//有效期截止日期 UNICODE YYYYMMDD 有效期为长期时存储“长期”

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 18)]
        public byte[] arrOtherNO;// 通行证类号码
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
        public byte[] arrSignNum;// 签发次数
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
        public byte[] arrCountry;//国籍 UNICODE GB/T2659-2000
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        public byte[] arrType;// 证件类型标识
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
        public byte[] arrRemark2;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 1024)]
        public byte[] arrPhoto;		//照片信息
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 1024)]
        public byte[] arrFingerprint;//指纹信息
    }
    public enum ECardFormatType : byte
    {
        TwoIDType = (byte)' ',   // 身份证
        TwoGATType = (byte)'J',   // 港澳台居民居住证
        OldForeignerType = (byte)'I',  // 外国人永久居留身份证
        NewForeignerType = (byte)'Y'   // 外国人永久居留身份证(新版)
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct CardInfoStruct
    {
        public byte etype;  // eCardFormatType
        public InfoUnion info;
    }

    [StructLayout(LayoutKind.Explicit)]
    public struct InfoUnion
    {
        [FieldOffset(0)]
        public TwoIdInfoStructEx twoId;  // 身份证/港澳台居民居住证

        [FieldOffset(0)]
        public ForeignerInfoOld foreigner;  // 旧版外国人永久居住证

        [FieldOffset(0)]
        public ForeignerInfoNew newForeigner;  // 新版外国人永久居住证
    }
}
