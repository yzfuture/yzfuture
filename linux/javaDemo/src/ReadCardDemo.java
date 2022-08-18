import com.sun.jna.Memory;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;

import java.io.*;

public class ReadCardDemo {

    public static void main(String[] args) throws InterruptedException, UnsupportedEncodingException {
        ReadCardInfoLib lib = Native.load("readCardInfo_x86_linux", ReadCardInfoLib.class);

        String  szip = "id.yzfuture.cn";
        int     nindex = 1001;
        long     yzwlHandle=-1;

        IntByReference nerrCode = new IntByReference(100) ;
        yzwlHandle = lib.cardOpenDeviceEx(szip, 443, 2, nerrCode, nindex);
        if ( yzwlHandle != -1)
        {
            Memory szDeviceDN = new Memory(100);
            IntByReference nDeviceDN = new IntByReference(100) ;
            lib.cardGetDeviceSN(yzwlHandle, szDeviceDN, nDeviceDN);
            String  szDN = new String(szDeviceDN.getByteArray(0, nDeviceDN.getValue()));;
            System.out.println("Device DN："+szDN);

            if (lib.setCardType(yzwlHandle))
            {
                IntByReference nmove = new IntByReference(100) ;
                if (lib.cardFindCard(yzwlHandle, nmove))
                {
                    mycallBack cb = new mycallBack();
                    TwoCardByteArray  info = new TwoCardByteArray();
                    Pointer puserdata = new Memory(1024);
                    int a = 0;
                    if (lib.cardReadTwoCard(yzwlHandle,cb, a, info))
                    {
                        String  szName = new String(info.arrTwoIdName, 0, info.arrTwoIdName.length, "UTF-16LE").trim();
                        if (!szName.isEmpty())
                        {
                            lib.cardBeep(yzwlHandle);

                            Memory  szoutbmp = new Memory(40*1024);
                            IntByReference nbmplen = new IntByReference(40*1024) ;
                            if (lib.decodeCardImage(info.arrTwoIdPhoto, szoutbmp, nbmplen))
                            {
                                System.out.println("解码完成："+szName + ":"+nbmplen.toString());

                                try{
                                    File file = new File("wlt.bmp");
                                    //为写入文件提供流通道
                                    FileOutputStream outputStream = new FileOutputStream(file);
                                    //将内存的字节数组内容 弄进 文件中
                                    outputStream.write(szoutbmp.getByteArray(0, nbmplen.getValue()),
                                            0,nbmplen.getValue());
                                    //关闭流
                                    outputStream.close();

                                }catch(IOException e){
                                    e.printStackTrace();
                                }
                            }
                            else
                            {
                                System.out.println("解码图片失败");
                            }
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

            lib.cardCloseDevice(yzwlHandle);
        }
    }
}
