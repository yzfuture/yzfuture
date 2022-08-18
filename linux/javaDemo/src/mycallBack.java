import com.sun.jna.Callback;

import java.awt.*;

/**
 * @author TygerZH
 * @date 2021-03-31 16:43
 */
public class mycallBack implements Callback {

    public void cardReadProgress(int userdata, int nProgress)
    {
        System.out.println("progress:" + nProgress);
    }
}
