import com.sun.jna.Callback;

/**
 * @author TygerZH
 * @date 2021-03-31 16:43
 */
public class mycallBack implements Callback {

    public void cardReadProgress(int nProgress, int nhandle)
    {
        System.out.println("progress:" + nProgress);
    }
}
