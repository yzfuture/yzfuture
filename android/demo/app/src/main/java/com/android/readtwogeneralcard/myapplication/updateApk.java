package com.android.readtwogeneralcard.myapplication;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.DownloadManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.net.Uri;
import android.os.Build;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v4.content.FileProvider;

import com.google.gson.Gson;
import com.readTwoGeneralCard.COMUpdateResponse;
import com.readTwoGeneralCard.Collections;
import com.readTwoGeneralCard.PermissionUtil;
import com.readTwoGeneralCard.UpdateUtils;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class updateApk {

    private     Context     thisContext = null;
    private     Handler     lpHandler = null;
    private     DownloadManager downloadManager= null;
    private     long        downloadId = 0;
    private     Activity    thisActivi = null;

    public void update(Context active, Activity activi, Handler handle)
    {
        thisActivi = activi;
        thisContext = active;
        lpHandler = handle;
        if (thisContext != null)
        {
            new Thread(new Runnable(){
                @Override
                public void run() {
                    onUpdate();
                }
            }).start();
        }
    }

    private void onUpdate()
    {
        int     nlocalVersion   = versionNumber();
        String  szupdate        = getRemoteVersion(nlocalVersion);
        if (!szupdate.isEmpty())
        {
                Gson gson=new Gson();
                COMUpdateResponse comUpdateResponse = gson.fromJson(szupdate, COMUpdateResponse.class);
                if (comUpdateResponse.collection != null && comUpdateResponse.collection.size() > 0) {
                    final Collections collections = comUpdateResponse.collection.get(0);

                    if (collections.versionNumber > nlocalVersion) {

                        thisActivi.runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                showUpdateUI(collections);
                            }
                        });

                    }
                }
        }
    }

    private void showUpdateUI(final Collections collections) {
        AlertDialog.Builder upDialogBuilder = new AlertDialog.Builder(thisContext);
        upDialogBuilder.setTitle(collections.changeLogTitle);
        upDialogBuilder.setMessage(collections.changeBody);
        upDialogBuilder.setPositiveButton("确定",
                new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        dialog.dismiss();
                        if (checkPermissions(thisActivi, Manifest.permission.WRITE_EXTERNAL_STORAGE, 1)) {
                            UpdateUtils.getInstance(thisContext).downloadAPK(collections.appUrl, "nfcupdate.apk", "二代证更新");
                        }
                    }
                });
        AlertDialog updateDialog = upDialogBuilder.create();
        updateDialog.setCanceledOnTouchOutside(false);
        updateDialog.show();
    }

    private static boolean checkPermissions(Activity activity, String permission, int requestCode) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (ContextCompat.checkSelfPermission(activity, permission) != PackageManager.PERMISSION_GRANTED) {
                ActivityCompat.requestPermissions(activity, new String[]{permission}, requestCode);
                return false;
            }
        }
        return true;
    }

    private int versionNumber() {
        PackageInfo info;
        try {
            info = thisContext.getPackageManager().getPackageInfo(thisContext.getPackageName(), 0);
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
            return 1;
        }
        return info.versionCode;
    }

    private String getRemoteVersion(int nlocalVersion)
    {
        String      szRet = "";
        String      szUrl = "http://zafk.vdin01.com/picasso-wan/practitioner-api/v1/release?packageName=cn.com.yzwl.NFC.androidphone&versionNumber=" + String.valueOf(nlocalVersion);
        try {
            //1. URL
            URL url = new URL(szUrl);
            //2. HttpURLConnection
            HttpURLConnection conn=(HttpURLConnection)url.openConnection();
            //3. set(GET)
            conn.setRequestMethod("GET");
            //4. getInputStream
            InputStream is = conn.getInputStream();
            //5. 解析is，获取responseText，这里用缓冲字符流
            BufferedReader reader = new BufferedReader(new InputStreamReader(is));
            StringBuilder sb = new StringBuilder();
            String line = null;
            while((line=reader.readLine()) != null){
                sb.append(line);
            }
            //获取响应文本
            szRet = sb.toString();
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }
        return szRet;
    }
}
