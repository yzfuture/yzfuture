package com.android.readtwogeneralcard.myapplication;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;
import android.nfc.NfcAdapter;
import android.nfc.tech.NfcA;
import android.nfc.tech.NfcB;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.text.TextUtils;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import com.blankj.utilcode.util.SPUtils;
import com.readTwoGeneralCard.ActiveCallBack;
import com.readTwoGeneralCard.OTGReadCardAPI;
import com.readTwoGeneralCard.PermissionUtil;
import com.readTwoGeneralCard.Serverinfo;
import com.readTwoGeneralCard.clientAuthInfo;
import com.readTwoGeneralCard.eCardType;
import com.readePassport.ePassportInfo;
import com.recognition.BaseRecognitionActivity;
import com.util.GASystemCamera;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.FileNotFoundException;
import java.lang.ref.WeakReference;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;

import butterknife.Bind;
import butterknife.ButterKnife;

import static java.lang.Thread.sleep;

public class IDCardScannerActivity extends Activity implements ActiveCallBack {
    @Bind(R.id.edid)
    TextView edid;
    @Bind(R.id.edname)
    TextView edname;
    @Bind(R.id.edenname)
    TextView edenname;

    @Bind(R.id.addTrue)
    TextView addTrue;
    @Bind(R.id.sexTrue)
    TextView sexTrue;
    @Bind(R.id.mzTrue)
    TextView mzTrue;
    @Bind(R.id.gjTrue)
    TextView gjTrue;
    @Bind(R.id.jgTrue)
    TextView jgTrue;
    @Bind(R.id.yxqTrue)
    TextView yxqTrue;
    @Bind(R.id.birthTrue)
    TextView birthTrue;
    @Bind(R.id.userInfo)
    TextView userInfo;

    @Bind(R.id.numtxt)
    TextView numtxt;
    @Bind(R.id.numtimetxt)
    TextView numtimetxt;

    @Bind(R.id.appkeyTxt)
    TextView appkeyTxt;
    @Bind(R.id.appSecretTxt)
    TextView appSecretTxt;
    @Bind(R.id.appUserDataTxt)
    TextView appUserDataTxt;

    @Bind(R.id.edtype)
    TextView edtype;
    @Bind(R.id.othernoTrue)
    TextView othernoTrue;
    @Bind(R.id.signTrue)
    TextView signTrue;

    @Bind(R.id.idimg)
    ImageView idimg;
    @Bind(R.id.resetBtn)
    Button resetBtn;
    @Bind(R.id.otgBtn)
    Button otgBtn;
    @Bind(R.id.numBtn)
    Button numBtn;

    @Bind(R.id.process)
    TextView process;
    @Bind(R.id.escapedTrue)
    TextView escapedTrue;
    @Bind(R.id.progressBar)
    ProgressBar progressBar;
    @Bind(R.id.deviceType)
    Switch deviceTypeBtn;

    @Bind(R.id.appkeyBtn)
    Button appkeyBtn;
    @Bind(R.id.secretBtn)
    Button secretBtn;
    @Bind(R.id.userBtn)
    Button userBtn;

    private NfcAdapter      mAdapter = null;
    private PendingIntent   pi = null;
    private IntentFilter    tagDetected = null;
    private String[][]      mTechLists;
    private final Handler   mHandler = new MyHandler(this);
    private OTGReadCardAPI  ReadCardAPI;
    private boolean         bNFC = false;

    private String          m_szUserInfo = "";
    private Bitmap          bkbmp = null;

    public static final String SP_FILE = "config_file";
    public static final String SPKEY_APPKEY = "SPKEY_APPKEY";
    public static final String SPKEY_SECRET = "SPKEY_SECRET";
    public static final String SPKEY_USERDATA = "SPKEY_USERDATA";

    private String          m_szServerIP = "id.yzfuture.cn";
    private int             m_nServerPort = 443;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_idget);
        ButterKnife.bind(this);
        Resources res = getResources();

        IntentFilter filter = new IntentFilter();
        filter.addAction(UsbManager.ACTION_USB_DEVICE_ATTACHED);
        filter.addAction(UsbManager.ACTION_USB_DEVICE_DETACHED);

        String dataDir = this.getApplicationContext().getFilesDir().getAbsolutePath();

        progressBar.setVisibility(View.VISIBLE);
        appkeyTxt.setTextIsSelectable(true);
        appSecretTxt.setTextIsSelectable(true);
        appUserDataTxt.setTextIsSelectable(true);

        String  szkey = SPUtils.getInstance(SP_FILE).getString(SPKEY_APPKEY, "");
        String   szsecret = SPUtils.getInstance(SP_FILE).getString(SPKEY_SECRET, "");
        String  szuser = SPUtils.getInstance(SP_FILE).getString(SPKEY_USERDATA, "");
        appkeyTxt.setText(szkey);
        appSecretTxt.setText(szsecret);
        appUserDataTxt.setText(szuser);

        init();
        bkbmp = BitmapFactory.decodeResource(res, R.mipmap.logo);
        userInfo.setMovementMethod(ScrollingMovementMethod.getInstance());
        numBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                getAuthInfo();
            }
        });
        resetBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                resetText();
            }
        });
        deviceTypeBtn.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean isChecked) {
                if(isChecked){
                    if (ReadCardAPI != null)
                    {
                        ReadCardAPI.setDeviceType(1);
                    }
                }else{
                    if (ReadCardAPI != null)
                    {
                        ReadCardAPI.setDeviceType(0);
                    }
                }
            }
        });

        otgBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                startReadCard(null);
            }
        });
        PermissionUtil.grantNeedPermission(this);

        appkeyBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                modifyValue(appkeyTxt, SPKEY_APPKEY);
            }
        });
        secretBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                modifyValue(appSecretTxt, SPKEY_SECRET);
            }
        });
        userBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                modifyValue(appUserDataTxt, SPKEY_USERDATA);
            }
        });
    }


    private void modifyValue(final TextView textView, final String key)
    {
        LayoutInflater li = LayoutInflater.from(IDCardScannerActivity.this);
        View promptsView = li.inflate(R.layout.modifyappkey_idget, null);

        AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(IDCardScannerActivity.this);
        alertDialogBuilder.setView(promptsView);
        final EditText    newtxt = (EditText) promptsView.findViewById(R.id.newtxt);
        Button modifybtn = (Button)promptsView.findViewById(R.id.modifybtn);
        String oldTxt = textView.getText().toString();
        newtxt.setText(oldTxt);
        final AlertDialog alertDialog = alertDialogBuilder.create();
        modifybtn.setOnClickListener(new Button.OnClickListener(){
            @Override
            public void onClick(View v) {
                String  sznewValue = newtxt.getText().toString();
                SPUtils.getInstance(SP_FILE).put(key, sznewValue);
                textView.setText(sznewValue);

                if (ReadCardAPI != null)
                {
                    final String      szAppKey = appkeyTxt.getText().toString();
                    final String      szAppSecret = appSecretTxt.getText().toString();
                    final String      szAppUserData = appUserDataTxt.getText().toString();
                    ReadCardAPI.initReadCard(m_szServerIP, m_nServerPort, szAppKey, szAppSecret, szAppUserData);
                    getAuthInfo();
                }

                alertDialog.dismiss();
            }
        });
        alertDialog.show();
    }

    public void init() {
        ReadCardAPI = new OTGReadCardAPI(getApplicationContext(), this);
        if (ReadCardAPI != null)
        {
            final String      szAppKey = appkeyTxt.getText().toString();
            final String      szAppSecret = appSecretTxt.getText().toString();
            final String      szAppUserData = appUserDataTxt.getText().toString();
            ReadCardAPI.initReadCard(m_szServerIP, m_nServerPort, szAppKey, szAppSecret, szAppUserData);
            getAuthInfo();
            mAdapter = NfcAdapter.getDefaultAdapter(getApplicationContext());
            if (mAdapter != null) {
                init_NFC();
            } else {
                if (ReadCardAPI != null) {
                    setdialog("本机不支持NFC功能！");
                }
            }
        }
    }

    private void init_NFC() {
        pi = PendingIntent.getActivity(this, 0, new Intent(this, getClass())
                .addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP), 0);
        tagDetected = new IntentFilter(NfcAdapter.ACTION_TECH_DISCOVERED);//.ACTION_TAG_DISCOVERED);
        tagDetected.addCategory(Intent.CATEGORY_DEFAULT);
        mTechLists = new String[][]{new String[]{NfcB.class.getName()}, new String[]{NfcA.class.getName()}};
        if (mAdapter != null && !mAdapter.isEnabled()) {
            Toast.makeText(this, "NFC尚未开启", Toast.LENGTH_SHORT).show();
        }
    }

    private static class MyHandler extends Handler {
        private final WeakReference<IDCardScannerActivity> mActivity;

        public MyHandler(IDCardScannerActivity activity) {
            mActivity = new WeakReference<IDCardScannerActivity>(activity);
        }

        @Override
        public void handleMessage(Message msg) {
            System.out.println(msg);
            if (mActivity.get() == null) {
                return;
            }
            if (msg.what == 0)
            {
                mActivity.get().progressBar.setProgress(msg.arg1);
                mActivity.get().userInfo.setText(mActivity.get().m_szUserInfo);
            }
            else if (msg.what == 1002)
                throw new RuntimeException();
            else {
                try {
                    mActivity.get().todo(msg);
                } catch (FileNotFoundException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public void setdialog(String tips) {
        final Dialog dialog = new Dialog(this);
        LayoutInflater li = LayoutInflater.from(this);
        View view = li.inflate(R.layout.dialog_fail, null);
        TextView tip = (TextView)view.findViewById(R.id.d_content);
        TextView sure = (TextView)view.findViewById(R.id.d_bt);
        tip.setText(tips);
        dialog.setContentView(view);
        sure.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                dialog.dismiss();
            }

        });
        dialog.show();
    }

    private void resetText()
    {
        edtype.setText("");
        edname.setText("");
        edenname.setText("");
        edid.setText("");

        addTrue.setText("");
        sexTrue.setText("");
        mzTrue.setText("");
        jgTrue.setText("");
        birthTrue.setText("");

        yxqTrue.setText("");
        gjTrue.setText("");

        userInfo.setText("");
        m_szUserInfo = "";
        process.setText("");
        progressBar.setProgress(0);
        idimg.setBackground(new BitmapDrawable(bkbmp));
    }
    public void todo(Message msg) throws FileNotFoundException {
        if (msg.what == -100)
        {
            setdialog("不支持此卡类型！");
            return;
        }
        else if (msg.what == 1)
        {
            if (ReadCardAPI.GetTwoCardInfo().arrTwoIdPhoto != null) {
                idimg.setBackground(new BitmapDrawable(Bytes2Bimap(ReadCardAPI.GetTwoCardInfo().arrTwoIdPhoto)));
            }
            if (ReadCardAPI.GetTwoCardInfo().szTwoType.equals("J")) {
                edtype.setText("港澳居民居住证");
                othernoTrue.setText(ReadCardAPI.GetTwoCardInfo().szTwoOtherNO);
                signTrue.setText(ReadCardAPI.GetTwoCardInfo().szTwoSignNum);
            } else {
                edtype.setText("居民身份证");
                othernoTrue.setText("");
                signTrue.setText("");
            }
            edid.setText(ReadCardAPI.GetTwoCardInfo().szTwoIdNo.trim());
            edname.setText(ReadCardAPI.GetTwoCardInfo().szTwoIdName.trim());
            addTrue.setText(ReadCardAPI.GetTwoCardInfo().szTwoIdAddress.trim());
            sexTrue.setText(ReadCardAPI.GetTwoCardInfo().szTwoIdSex.trim());
            mzTrue.setText(ReadCardAPI.GetTwoCardInfo().szTwoIdNation.trim() + "族");
            gjTrue.setText("中国");
            jgTrue.setText(ReadCardAPI.GetTwoCardInfo().szTwoIdSignedDepartment.trim());
            birthTrue.setText(ReadCardAPI.GetTwoCardInfo().szTwoIdBirthday.substring(0, 4) + "-" + ReadCardAPI.GetTwoCardInfo().szTwoIdBirthday.substring(4, 6) + "-" + ReadCardAPI.GetTwoCardInfo().szTwoIdBirthday.substring(6, 8));

            String startDate = ReadCardAPI.GetTwoCardInfo().szTwoIdValidityPeriodBegin.substring(0, 4) + "-" + ReadCardAPI.GetTwoCardInfo().szTwoIdValidityPeriodBegin.substring(4, 6) + "-" + ReadCardAPI.GetTwoCardInfo().szTwoIdValidityPeriodBegin.substring(6, 8);
            String endDate = "";
            int nlen = ReadCardAPI.GetTwoCardInfo().szTwoIdValidityPeriodEnd.indexOf("长期");
            if (nlen != -1) {
                endDate = "长期";
            } else {
                endDate = ReadCardAPI.GetTwoCardInfo().szTwoIdValidityPeriodEnd.substring(0, 4) + "-" + ReadCardAPI.GetTwoCardInfo().szTwoIdValidityPeriodEnd.substring(4, 6) + "-" + ReadCardAPI.GetTwoCardInfo().szTwoIdValidityPeriodEnd.substring(6, 8);
            }
            yxqTrue.setText(startDate + "~" + endDate);
            //process.setText("100");
        }
        if (msg.what == -9999)
        {
            String  szerr = ReadCardAPI.GetErrorInfo();
            setdialog(szerr);
        }
    }

    public Bitmap Bytes2Bimap(byte[] b) {
        if (b.length != 0) {
            return BitmapFactory.decodeByteArray(b, 0, b.length);
        } else {
            return null;
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (mAdapter != null) {
            mAdapter.enableForegroundDispatch(this, pi, new IntentFilter[]{tagDetected}, mTechLists);
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (mAdapter != null) {
            mAdapter.disableForegroundDispatch(this);
        }
    }

    @Override
    protected void onDestroy() {
        mHandler.removeCallbacksAndMessages(null);
        super.onDestroy();
    }

    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        progressBar.setProgress(0);
        startReadCard(intent);
    }

    @Override
    public void setUserInfo(String s)
    {
        //m_szUserInfo += s + "\r\n";
    }

    private void getAuthInfo()
    {
        clientAuthInfo authInfo = ReadCardAPI.GetAppKeyUseNum();
        if (authInfo == null) {
            return;
        }

        numtxt.setText(String.valueOf(authInfo.nNum));
        numtimetxt.setText(new String(authInfo.szDate));
    }

    @Override
    public void readProgress(int nprocess, String szinfo) {
        m_szUserInfo += szinfo + "\r\n";
        Message msg = Message.obtain();
        msg.what = 0;
        msg.arg1 = nprocess;
        mHandler.sendMessageDelayed(msg, 0);
    }

    private void startReadCard(final Intent intent){
        progressBar.setProgress(0);
        process.setText("");

        getAuthInfo();
        new Thread(new Runnable() {
            @Override
            public void run() {
                eCardType etype = ReadCardAPI.cardType(intent);
                if (etype == eCardType.eTwoGeneralCard)
                {
                    int     tt = 0;
                    tt = ReadCardAPI.NfcReadCard(intent);
                    if (tt == 41)
                    {
                        mHandler.sendEmptyMessageDelayed(-9999, 0);
                    }
                    if (tt == 90)
                    {
                        Message msg = Message.obtain();
                        msg.what = 1;
                        mHandler.sendMessageDelayed(msg, 0);
                    }
                }
            }
        }).start();
    }
}
