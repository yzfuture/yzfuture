import com.sun.jna.Native;
import com.sun.jna.ptr.IntByReference;

import java.io.UnsupportedEncodingException;

/**
 * Created on 2021/3/31.
 *
 * @author hxj
 */
public class ReadCardDemo {
    public static void main(String[] args) throws InterruptedException, UnsupportedEncodingException {
        ReadCardInfoLib lib = Native.load("readCardInfo.dll", ReadCardInfoLib.class);
        int yzwlHandle = lib.cardOpenDevice(0, 0);
        String  szkey = "99ffb2f98a29071107c7a09ad2c6d096";
        String  szIP = "id.yzfuture.cn";
        Memory szDeviceDN = new Memory(100);
        IntByReference nDeviceDN = new IntByReference(100) ;
        lib.cardGetDeviceSN(yzwlHandle, szDeviceDN, nDeviceDN);
        String  szDN = new String(szDeviceDN.getByteArray(0, nDeviceDN.getValue()));;
        System.out.println("Device DN："+szDN);
        for (int i=0; i<100; i++)
        {
            mycallBack cb = new mycallBack();
            TwoCardByteArray  info = new TwoCardByteArray();
            if (lib.cardReadTwoCard(yzwlHandle,cb, szkey, szIP,
                    8848, info, true))
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
                System.out.println("解码失败");
            }
            Thread.sleep(2000);
        }

        lib.cardCloseDevice(yzwlHandle);
    }
}
