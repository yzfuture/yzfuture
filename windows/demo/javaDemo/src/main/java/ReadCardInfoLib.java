import com.sun.jna.Memory;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;
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

    int     cardOpenDevice(String szAppKey, String szAppSecret, String szServerIp, int nServerPort, String szUserData, int nouttime, IntByReference nerr,int nDeviceNo);

    boolean cardBeep(long nDeviceHandle);

    boolean setCardType(long nDeviceHandle, int ctype);

    boolean cardFindCard(int nDeviceHandle, IntByReference nlen);
    boolean cardSelectCard(int nDeviceHandle);

    boolean cardReadTwoCard(int nDeviceHandle, mycallBack cb, TwoCardByteArray cardinfo);

    boolean cardGetDeviceNO(int nDeviceHandle, Memory szno, IntByReference nlen);
    boolean cardGetDeviceSN(int nDeviceHandle, Memory szsn, IntByReference nlen);

    int     cardGetLastErrorCode(int nDeviceHandle);

    void    cardCloseDevice(int nDeviceHandle);

    void    cardReadUninit();
}
