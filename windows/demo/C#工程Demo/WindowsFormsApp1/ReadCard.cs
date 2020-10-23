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
        /// <summary>
        /// 	cardOpenDevice
        //打开读卡器硬件设备。
        //nouttime:超时时间（秒）
        //nDeviceNo:读卡器序号，默认为0（为解决同时有多个相同读卡器的情况，可以选其中某一个为读卡设备）
        //返回值：
        //成功 - 读卡器句柄
        //失败 <= 0
        /// </summary>
        /// <param name="Port"></param>
        /// <returns></returns>
        [DllImport("readCardInfo.dll")]//readCardInfo.dll
        public static extern int cardOpenDevice(int nouttime, int nDeviceNo);//打开读卡器硬件设备


        [DllImport("readCardInfo.dll")]//readCardInfo.dll
        public static extern bool setCardType(int nDeviceHandle, int ctype);//设置卡片类型


        [DllImport("readCardInfo.dll")]//readCardInfo.dll
        public static extern bool cardFindCard(int nDeviceHandle, ref bool bmove);//寻卡

        [DllImport("readCardInfo.dll",CharSet = CharSet.Ansi, ExactSpelling = false,
             CallingConvention = CallingConvention.StdCall)]//readCardInfo.dll
        public static extern bool cardReadTwoCard(int nDeviceHandle, int cardCB, string szFactoryFlag, string szServerIp, int nServerPort, ref TwoIdInfoStruct cardinfo, bool bTest);//读卡

        [DllImport("readCardInfo.dll")]//readCardInfo.dll
        public static extern bool decodeCardImage(byte[] srcimage, byte[] outimage, ref int outlen);

        [DllImport("readCardInfo.dll")]//readCardInfo.dll
        public static extern bool cardCloseDevice(int nDeviceHandle);//关闭

        public static bool decodeImage(byte[] srcimage, byte[] outimage, ref int outlen)
        {
            return decodeCardImage(srcimage, outimage, ref outlen);
        }

        public static TwoIdInfoStruct ReadCardNo()
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

            int hlHandle = cardOpenDevice(5, 0);
            if (hlHandle > 0)
            {
                bool bmove = true;
                if (setCardType(hlHandle, 1))
                {
                    if (cardFindCard(hlHandle, ref bmove))
                    {
                        int cb = 0;
                        string szAppkey = "99ffb2f98a29071107c7a09ad2c6d096";
                        string szip = "id.yzfuture.cn";
                        bool bret = cardReadTwoCard(hlHandle, cb, szAppkey, szip, 8848, ref sttTwoIdInfo, false);
                        if (bret)
                        {
                            MessageBox.Show("解码成功:" + Encoding.Unicode.GetString(sttTwoIdInfo.arrTwoIdName));
                            MessageBox.Show("身份证号:" + Encoding.Unicode.GetString(sttTwoIdInfo.arrTwoIdNo));
                        }
                    }
                }
                cardCloseDevice(hlHandle);
            }
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
