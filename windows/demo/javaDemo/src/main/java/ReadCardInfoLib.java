import com.sun.jna.Memory;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;
import com.sun.jna.ptr.ByteByReference;
import com.sun.jna.ptr.IntByReference;
import com.sun.jna.win32.StdCallLibrary;

import java.util.Arrays;
import java.util.List;

/**
 * Created on 2021/3/31.
 *
 * @author hxj
 */
public interface ReadCardInfoLib extends StdCallLibrary {

    void    cardReadInit();

    void    setDeviceType(int nDeviceType);

    boolean loginCardServerEx(String szip, int nport, IntByReference nerr);

    boolean loginCardServer(String szip, int nport, String szAppkey, String szAppSecret, String szAppUserId, IntByReference nerr);

    int     cardOpenDevice(int nouttime, IntByReference nerr,int nDeviceNo);

    boolean cardBeep(long nDeviceHandle);

    boolean setCardType(long nDeviceHandle, int ctype);

    boolean cardFindCard(int nDeviceHandle, IntByReference nlen);
    boolean cardSelectCard(int nDeviceHandle);

    boolean cardReadTwoCard(int nDeviceHandle, mycallBack cb, TwoCardByteArray cardinfo);

    boolean cardGetDeviceNO(int nDeviceHandle, Memory szno, IntByReference nlen);
    boolean cardGetDeviceSN(int nDeviceHandle, Memory szsn, IntByReference nlen);

    boolean decodeCardImage(byte[] srcimage, Memory  outimage, IntByReference outlen);

    int     cardGetLastErrorCode(int nDeviceHandle);

    void    cardCloseDevice(int nDeviceHandle);

    void    logoutCardServer();

    void    cardReadUninit();
}
