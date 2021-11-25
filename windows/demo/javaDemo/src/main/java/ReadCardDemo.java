import com.sun.jna.Memory;
import com.sun.jna.Native;
import com.sun.jna.ptr.IntByReference;

import java.io.UnsupportedEncodingException;
import java.util.Scanner;

public class ReadCardDemo {
    public static void main(String[] args) throws InterruptedException, UnsupportedEncodingException {
        ReadCardInfoLib lib = Native.load("readCardInfo.dll", ReadCardInfoLib.class);
        lib.cardReadInit();

        String  szAppKey = "请参照《NFC服务注册流程 V2.pdf》申请";
        String  szAppSecret = "请参照《NFC服务注册流程 V2.pdf》申请";
        String  szUserData = "请参照《NFC服务注册流程 V2.pdf》申请";
        String  szip = "id.yzfuture.cn";
        int     nindex = 0;
        int     yzwlHandle=0;

        for (int i=0; i<100; i++)
        {
            Scanner sc = new Scanner(System.in);
            String  str = "";
            do {
                System.out.println("请选择读卡器:(0-标准读卡器 1-离线读卡器)");
                str = sc.next();
            }while(!str.equals("0") && !str.equals("1"));

            if (str.equals("0"))
            {
                nindex = 0;
                lib.setDeviceType(0);
            }
            else
            {
                nindex = 1001;
                lib.setDeviceType(1);
            }

            IntByReference nerrCode = new IntByReference(100) ;
            yzwlHandle = lib.cardOpenDevice(szAppKey, szAppSecret, szip, 443, szUserData, 2, nerrCode, nindex);
            Memory szDeviceDN = new Memory(100);
            IntByReference nDeviceDN = new IntByReference(100) ;
            lib.cardGetDeviceSN(yzwlHandle, szDeviceDN, nDeviceDN);
            String  szDN = new String(szDeviceDN.getByteArray(0, nDeviceDN.getValue()));;
            System.out.println("Device DN："+szDN);

            if (lib.setCardType(yzwlHandle, 1))
            {
                IntByReference nmove = new IntByReference(100) ;
                if (lib.cardFindCard(yzwlHandle, nmove))
                {
                    if (lib.cardSelectCard(yzwlHandle))
                    {
                        mycallBack cb = new mycallBack();
                        TwoCardByteArray  info = new TwoCardByteArray();
                        if (lib.cardReadTwoCard(yzwlHandle,cb, info))
                        {
                            String  szName = new String(info.arrTwoIdName, 0, info.arrTwoIdName.length, "UTF-16LE").trim();
                            if (!szName.isEmpty())
                            {
                                lib.cardBeep(yzwlHandle);
                                System.out.println("解码完成："+szName);
                            }
                            else
                            {
                                System.out.println("解码失败");
                            }
                        }
                        else
                        {
                            int     nerrorCode = lib.cardGetLastErrorCode(yzwlHandle);
                            System.out.println("解码失败:" + nerrorCode);
                        }
                    }
                }
            }
        }

        lib.cardCloseDevice(yzwlHandle);
        lib.cardReadUninit();
    }
}
