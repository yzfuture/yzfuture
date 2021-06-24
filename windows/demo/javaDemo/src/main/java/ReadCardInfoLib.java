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

    int cardOpenDevice(int nouttime,int nDeviceNo);

    boolean cardBeep(long nDeviceHandle);

    boolean cardReadTwoCard(int nDeviceHandle, mycallBack cb,
                            String szFactoryFlag, String szServerIp, int nServerPort,
                            TwoCardByteArray cardinfo, boolean bTest);

    boolean cardGetDeviceNO(int nDeviceHandle, Memory szno, IntByReference nlen);
    boolean cardGetDeviceSN(int nDeviceHandle, Memory szsn, IntByReference nlen);
    void cardCloseDevice(int nDeviceHandle);
}
