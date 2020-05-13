package com.android.readtwogeneralcard.myapplication;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

/**
 * Created by Administrator on 2017/11/3.
 */

public class logTest {

    private static OutputStream fosto = null;

    public static void log(String   szlog)
    {
        if (fosto == null)
        {
            try {
                fosto = new FileOutputStream("/sdcard/readCard.txt");
            } catch (FileNotFoundException e) {
                e.printStackTrace();
                fosto = null;
            }
        }

        if (fosto != null)
        {
            try {
                fosto.write(szlog.getBytes(), 0, szlog.length());

                String  szTxt = "\r\n";
                fosto.write(szTxt.getBytes(), 0, szTxt.length());

            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
