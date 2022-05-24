package com.yzwl.nfcdemo;

import static android.hardware.usb.UsbManager.ACTION_USB_DEVICE_ATTACHED;
import static android.hardware.usb.UsbManager.ACTION_USB_DEVICE_DETACHED;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;
import android.nfc.NfcAdapter;
import android.nfc.tech.NfcA;
import android.nfc.tech.NfcB;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.readTwoGeneralCard.ActiveCallBack;
import com.readTwoGeneralCard.OTGReadCardAPI;
import com.readTwoGeneralCard.TwoCardInfo;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class MainActivity extends AppCompatActivity{

    //注册获取
    public final static String APP_KEY = "";
    //注册获取
    public final static String APP_SECRETE = "=";
    //绑定设备获取
    public final static String USER_DATA = "";

    public final static String NFC_SERVICE = "id.yzfuture.cn";
    public final static int NFC_PORT = 443;

    //读卡是耗时操作 放到子线程完成
    private  ExecutorService nfcExecutorService;
    //读卡api
    private OTGReadCardAPI mReadCardAPI;

    //NFC对象定义
    //nfc控制器
    private NfcAdapter mNfcAdapter;
    public static final String NFC_TAG = "NFC_TAG";

    //otg对象定义
    //用于获取OTG使用权限
    private UsbManager mUsbManager;

    /**
     * 读卡回调
     */
    protected ActiveCallBack readCallBack = new ActiveCallBack() {

        /**
         * 读卡进度回调
         * @param i 0 ~ 19
         * @param s
         */
        @Override
        public void readProgress(int i, String s) {
            Log.d(NFC_TAG, "" + i * 100 / 19 );
        }

        @Override
        public void setUserInfo(String s) {
        }

        @Override
        public void upgradeInfo(String s) {
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView txtTest = findViewById(R.id.txt_text);
        txtTest.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                readOTG();
            }
        });

        //初始化NFC对象
        nfcExecutorService = Executors.newSingleThreadExecutor();
        mNfcAdapter = NfcAdapter.getDefaultAdapter(MainActivity.this);
        if (mNfcAdapter == null){
            Log.e(NFC_TAG, "当前设备不支持NFC");
            if (!mNfcAdapter.isEnabled()){
                Log.e(NFC_TAG, "NFC未打开，提示用户去《设置》打开");
            }
        }

        // 初始化USB对象
        mUsbManager = (UsbManager) getSystemService(Context.USB_SERVICE);

        //初始化相关对象
        try {
            mReadCardAPI = new OTGReadCardAPI(MainActivity.this, readCallBack);
            //初始化API参数
            mReadCardAPI.initReadCard(NFC_SERVICE, NFC_PORT, APP_KEY, APP_SECRETE, USER_DATA);
        }catch (Exception e){
            e.printStackTrace();
        }
    }

    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        //用户身份证靠近手机会来到这个回调 在这个地方读卡
        startReadCard(intent);
    }

    private void readOTG(){
        startReadCard(null);
    }

    private void startReadCard(final Intent intent){
        if (mReadCardAPI == null)return;
        nfcExecutorService.execute(new Runnable() {
            @Override
            public void run() {
                try {
                    //设置读卡器类型 0联网 1离线，可以只设置一次
                    mReadCardAPI.setDeviceType(0);
                    //与手机读卡调用方法一至 读卡句柄填空即可
                    int num =  mReadCardAPI.NfcReadCard(intent);
                    if (num == 90) {
                        //读卡成功 获取读卡信息然后根据需求刷新UI
                        TwoCardInfo cardInfo = mReadCardAPI.GetTwoCardInfo();
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                Log.e(NFC_TAG, "姓名：" + cardInfo.szTwoIdName);
                                Log.e(NFC_TAG, "性别：" + cardInfo.szTwoIdSex);
                                Log.e(NFC_TAG, "民族：" + cardInfo.szTwoIdNation);
                                Log.e(NFC_TAG, "出生日期：" + cardInfo.szTwoIdBirthday);
                                Log.e(NFC_TAG, "住址：" + cardInfo.szTwoIdAddress);
                                Log.e(NFC_TAG, "身份证号：" + cardInfo.szTwoIdNo);
                                Log.e(NFC_TAG, "签发机关：" + cardInfo.szTwoIdSignedDepartment);
                                Log.e(NFC_TAG, "有限期限：" + cardInfo.szTwoIdValidityPeriodBegin + "-" + cardInfo.szTwoIdValidityPeriodEnd);

                            }
                        });
                    }else {
                        //读卡失败 获取错误码和错误信息
                        Log.e(NFC_TAG, "错误信息:" + mReadCardAPI.GetErrorCode() + mReadCardAPI.GetErrorInfo());
                    }
                }catch (Exception e){
                    Log.e(NFC_TAG, "身份证读卡失败");
                    e.printStackTrace();
                }

            }
        });
    }

    private static final String ACTION_USB_PERMISSION = "com.android.usb.USB_PERMISSION";

    /**
     * 创建一个BroadcastReceiver 用于监听USB接入变化，以及
     */
    private final BroadcastReceiver mUsbPreReceiver = new BroadcastReceiver() {
        @SuppressLint("NewApi")
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            UsbDevice device = (UsbDevice) intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
            if (TextUtils.isEmpty(action) || device == null || mUsbManager == null)return;
            switch (action){
                case ACTION_USB_DEVICE_ATTACHED:
                    //用户插入读卡器以后会来到这个回调，以下代码用于向用户请求USB权限
                    try {
                        PendingIntent pendingIntent = PendingIntent.getBroadcast(MainActivity.this,
                                0,
                                new Intent(ACTION_USB_PERMISSION),
                                android.os.Build.VERSION.SDK_INT >= 31 ? PendingIntent.FLAG_MUTABLE : 0);
                        mUsbManager.requestPermission(device, pendingIntent);
                    }catch (Exception e){
                        e.printStackTrace();
                        Log.e(NFC_TAG, "请求USB授权失败");
                    }
                    break;
                case ACTION_USB_DEVICE_DETACHED:
                    // 拔出USB 根据自身需求处理
                    break;
                case ACTION_USB_PERMISSION:
                    //用户点击同意或拒绝以后来到这个回调
                    if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
                        //用户点击了同意
                        Log.e(NFC_TAG, "用户同意USB授权");
                    }else{
                        //用户点击拒绝了
                        Log.e(NFC_TAG, "用户拒绝USB授权");
                    }
                    break;
                default:
                    break;
            }
        }
    };

    @Override
    protected void onResume() {
        super.onResume();
        //向系统申请处理NFC句柄
        if (mNfcAdapter != null){
            try {
                Intent intent = new Intent(MainActivity.this, MainActivity.class);
                intent.addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
                PendingIntent pendingIntent = PendingIntent.getActivity(MainActivity.this,
                        0,
                        intent,
                        android.os.Build.VERSION.SDK_INT >= 31 ? PendingIntent.FLAG_MUTABLE : 0);
                IntentFilter tagDetected = new IntentFilter(NfcAdapter.ACTION_TECH_DISCOVERED);
                tagDetected.addCategory(Intent.CATEGORY_DEFAULT);
                String[][] techLists = new String[][]{new String[]{NfcB.class.getName()}, new String[]{NfcA.class.getName()}};
                mNfcAdapter.enableForegroundDispatch(MainActivity.this, pendingIntent, new IntentFilter[]{tagDetected}, techLists);
            }catch (Exception e){
                Log.e(NFC_TAG, "向系统申请处理NFC句柄失败");
                e.printStackTrace();
            }
        }

        //监听USB OTG接入的情况
        IntentFilter dFilter = new IntentFilter();
        //插入设备监听
        dFilter.addAction(ACTION_USB_DEVICE_ATTACHED);
        //拔出设备监听
        dFilter.addAction(ACTION_USB_DEVICE_DETACHED);
        //打开设备监听
        dFilter.addAction(ACTION_USB_PERMISSION);
        registerReceiver(mUsbPreReceiver, dFilter);
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (mNfcAdapter != null){
            mNfcAdapter.disableForegroundDispatch(MainActivity.this);
        }

        unregisterReceiver(mUsbPreReceiver);
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (mReadCardAPI != null){
            mReadCardAPI.uninitReadCard();
        }
    }
}