
import com.sun.jna.Library;
import com.sun.jna.Memory;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;
public interface ReadCardInfoLib extends Library {

    long    cardOpenDeviceEx(String szip, int nport, int nouttime, IntByReference nerr, int  nDeviceNo);

    boolean cardBeep(long nDeviceHandle);

    boolean setCardType(long nDeviceHandle);

    boolean cardFindCard(long nDeviceHandle, IntByReference nlen);

    boolean cardReadTwoCard(long nDeviceHandle, mycallBack cb, int a, TwoCardByteArray cardinfo);

    boolean cardGetDeviceNO(long nDeviceHandle, Memory szno, IntByReference nlen);
    boolean cardGetDeviceSN(long nDeviceHandle, Memory szsn, IntByReference nlen);

    boolean decodeCardImage(byte[] srcimage, Memory  outimage, IntByReference outlen);

    int     cardGetLastErrorCode(long nDeviceHandle);

    void    cardCloseDevice(long nDeviceHandle);

    boolean cardGetLastError(long nDeviceHandle, Memory  outerror, IntByReference nlen);
}