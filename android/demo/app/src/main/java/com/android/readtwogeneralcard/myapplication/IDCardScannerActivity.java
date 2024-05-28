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
import android.os.StrictMode;
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

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.FileNotFoundException;
import java.lang.ref.WeakReference;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

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
    @Bind(R.id.progressBar)
    ProgressBar progressBar;
    @Bind(R.id.deviceType)
    Switch deviceTypeBtn;
    @Bind(R.id.tryBtn)
    Button tryBtn;

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
    private OTGReadCardAPI  readCard;
    private boolean         bNFC = false;

    private String          m_szUserInfo = "";
    private Bitmap          bkbmp = null;

    public static final String SP_FILE = "config_file";
    public static final String SPKEY_APPKEY = "SPKEY_APPKEY";
    public static final String SPKEY_SECRET = "SPKEY_SECRET";
    public static final String SPKEY_USERDATA = "SPKEY_USERDATA";

    private String          m_szServerIP = "id.yzfuture.cn";
    private int             m_nServerPort = 443;
    private boolean         m_bonDevice = true;

    private Button          timeButton = null;

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
                    if (readCard != null)
                    {
                        readCard.setDeviceType(1);
                        m_bonDevice = false;
                    }
                }else{
                    if (readCard != null)
                    {
                        readCard.setDeviceType(0);
                        m_bonDevice = true;
                    }
                }
            }
        });
        tryBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                trialDevice();
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
    //倒计时函数
    private class MyCountDownTimer extends CountDownTimer {

        public MyCountDownTimer(long millisInFuture, long countDownInterval) {
            super(millisInFuture, countDownInterval);
        }

        //计时过程
        @Override
        public void onTick(long l) {
            //防止计时过程中重复点击
            timeButton.setClickable(false);
            timeButton.setText(l/1000+"秒");

        }

        //计时完毕的方法
        @Override
        public void onFinish() {
            //重新给Button设置文字
            timeButton.setText("重新获取");
            //设置可点击
            timeButton.setClickable(true);
        }
    }

    private boolean isMobileNO(String mobileNums)
    {
        /**
         * 判断字符串是否符合手机号码格式
         * 移动号段: 134,135,136,137,138,139,147,150,151,152,157,158,159,170,178,182,183,184,187,188
         * 联通号段: 130,131,132,145,155,156,170,171,175,176,185,186
         * 电信号段: 133,149,153,170,173,177,180,181,189
         * @param str
         * @return 待检测的字符串
         */
        String telRegex = "^((13[0-9])|(14[5,7,9])|(15[^4])|(18[0-9])|(17[0,1,3,5,6,7,8]))\\d{8}$";// "[1]"代表第1位为数字1，"[358]"代表第二位可以为3、5、8中的一个，"\\d{9}"代表后面是可以是0～9的数字，有9位。
        if (TextUtils.isEmpty(mobileNums))
            return false;
        else
            return mobileNums.matches(telRegex);
    }

    private void trialDevice()
    {
        LayoutInflater li = LayoutInflater.from(IDCardScannerActivity.this);
        View promptsView = li.inflate(R.layout.register_idget, null);

        AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(IDCardScannerActivity.this);
        alertDialogBuilder.setView(promptsView);
        final EditText    phonetxt = (EditText) promptsView.findViewById(R.id.phonetxt);
        final EditText    codetxt = (EditText) promptsView.findViewById(R.id.codetxt);
        final RegisterUser regit = new RegisterUser();
        timeButton = (Button)promptsView.findViewById(R.id.timebtn);
        Button      tryBtn = (Button)promptsView.findViewById(R.id.trybtn);
        final AlertDialog alertDialog = alertDialogBuilder.create();
        final MyCountDownTimer myCountDownTimer = new MyCountDownTimer(60000,1000);
        if (android.os.Build.VERSION.SDK_INT > 9) {
            StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
            StrictMode.setThreadPolicy(policy);
        }
        timeButton.setOnClickListener(new Button.OnClickListener(){
            @Override
            public void onClick(View v) {
                String  szPhoneTxt = phonetxt.getText().toString();
                if (isMobileNO(szPhoneTxt))
                {
                    boolean     bsucc = regit.getCode(szPhoneTxt);
                    if (bsucc)
                    {
                        myCountDownTimer.start();
                        setdialog("短信已发出，请注意查收");
                    }
                    else
                    {
                        setdialog("短信发送失败，请重试");
                    }
                }
                else
                {
                    setdialog("手机号码格式不对，请重新输入");
                }
            }
        });

        tryBtn.setOnClickListener(new Button.OnClickListener(){
            @Override
            public void onClick(View v) {
                String  szPhoneTxt = phonetxt.getText().toString();
                String  szCodeTxt = codetxt.getText().toString();
                if (szPhoneTxt.isEmpty() || szCodeTxt.isEmpty())
                {
                    setdialog("以上信息不能为空");
                }
                else
                {
                    RegisterUser.RetParam retParam = regit.registrUser(szPhoneTxt, szCodeTxt);
                    if (retParam == null)
                    {
                        setdialog("注册失败，请重获取验证码");
                    }
                    else
                    {
                        if (retParam.nRetCode == 1)
                        {
                            if (retParam.appArray.size() >= 1)
                            {
                                // *** 注意：这里有可能会有多个应用appkey返回，实际应用中可以列出一个列表让用户选择
                                //           由于demo只展示逻辑调用，所以默认用第一个appkey，如需其它appkey请用户自己修改
                                Map<String, String> appMap = retParam.appArray.get(0);
                                SPUtils.getInstance(SP_FILE).put(SPKEY_APPKEY, appMap.get("appKey"));
                                SPUtils.getInstance(SP_FILE).put(SPKEY_SECRET, appMap.get("appSecret"));
                                SPUtils.getInstance(SP_FILE).put(SPKEY_USERDATA, "");
                                appkeyTxt.setText(appMap.get("appKey"));
                                appSecretTxt.setText(appMap.get("appSecret"));
                                appUserDataTxt.setText("");

                                if (retParam.appArray.size() > 1)
                                {
                                    setdialog("找回成功，appKey已保存");
                                }
                                else
                                {
                                    setdialog("注册成功，用户名/密码：" + retParam.szUser + "/" + retParam.szPwd + ", 如需管理应用，请登录：https://login.aidoing.com.cn/");
                                }
                                final String      szAppKey = appkeyTxt.getText().toString();
                                final String      szAppSecret = appSecretTxt.getText().toString();
                                final String      szAppUserData = appUserDataTxt.getText().toString();
                                readCard.initReadCard(m_szServerIP, m_nServerPort, szAppKey, szAppSecret, szAppUserData);
                                getAuthInfo();
                            }
                            else
                            {
                                setdialog("创建失败，原因：账号已存在且应用列表为空，请登录后台自行管理应用：https://login.aidoing.com.cn/");
                            }
                        }
                        else
                        {
                            setdialog("注册失败，原因:" + retParam.szError);
                        }
                        alertDialog.dismiss();
                    }
                }
            }
        });

        alertDialog.show();
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

                if (readCard != null)
                {
                    final String      szAppKey = appkeyTxt.getText().toString();
                    final String      szAppSecret = appSecretTxt.getText().toString();
                    final String      szAppUserData = appUserDataTxt.getText().toString();
                    readCard.initReadCard(m_szServerIP, m_nServerPort, szAppKey, szAppSecret, szAppUserData);
                    getAuthInfo();
                }

                alertDialog.dismiss();
            }
        });

        alertDialog.show();
    }

    public void init() {
        readCard = new OTGReadCardAPI(getApplicationContext(), this);
        if (readCard != null)
        {
            final String      szAppKey = appkeyTxt.getText().toString();
            final String      szAppSecret = appSecretTxt.getText().toString();
            final String      szAppUserData = appUserDataTxt.getText().toString();
            readCard.initReadCard(m_szServerIP, m_nServerPort, szAppKey, szAppSecret, szAppUserData);
            getAuthInfo();
            mAdapter = NfcAdapter.getDefaultAdapter(getApplicationContext());
            if (mAdapter != null) {
                init_NFC();
            } else {
                if (readCard != null) {
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
            if (readCard.GetBCardInfo().type == 3) // 外国人永久居留身份证
            {
                if (readCard.GetBCardInfo().foreigner.photo != null) {
                    idimg.setBackground(new BitmapDrawable(Bytes2Bimap(readCard.GetBCardInfo().foreigner.photo)));
                }
                edtype.setText(readCard.GetBCardInfo().foreigner.type);
                edname.setText(readCard.GetBCardInfo().foreigner.name.trim());
                sexTrue.setText(readCard.GetBCardInfo().foreigner.sex.trim());
                gjTrue.setText(readCard.GetBCardInfo().foreigner.country.trim());
                mzTrue.setText(readCard.GetBCardInfo().foreigner.nation.trim());
                birthTrue.setText(readCard.GetBCardInfo().foreigner.birthday.trim());
                edenname.setText(readCard.GetBCardInfo().foreigner.enName.trim());
                edid.setText(readCard.GetBCardInfo().foreigner.no.trim());
                addTrue.setText("");
                jgTrue.setText(readCard.GetBCardInfo().foreigner.department.trim());

                String startDate = readCard.GetBCardInfo().foreigner.beginTime.substring(0, 4) + "-" + readCard.GetBCardInfo().foreigner.beginTime.substring(4, 6) + "-" + readCard.GetBCardInfo().foreigner.beginTime.substring(6, 8);
                String endDate = "";
                int nlen = readCard.GetBCardInfo().foreigner.endTime.indexOf("长期");
                if (nlen != -1) {
                    endDate = "长期";
                } else {
                    endDate = readCard.GetBCardInfo().foreigner.endTime.substring(0, 4) + "-" + readCard.GetBCardInfo().foreigner.endTime.substring(4, 6) + "-" + readCard.GetBCardInfo().foreigner.endTime.substring(6, 8);
                }
                yxqTrue.setText(startDate + "~" + endDate);
                signTrue.setText(readCard.GetBCardInfo().foreigner.signNum.trim());
                othernoTrue.setText(readCard.GetBCardInfo().foreigner.otherNo.trim());
            }
            else if (readCard.GetBCardInfo().type == 4) // 外国人永久居留身份证（旧版）
            {
                if (readCard.GetBCardInfo().foreignerOld.photo != null) {
                    idimg.setBackground(new BitmapDrawable(Bytes2Bimap(readCard.GetBCardInfo().foreignerOld.photo)));
                }
                edtype.setText(readCard.GetBCardInfo().foreignerOld.type);
                edname.setText(readCard.GetBCardInfo().foreignerOld.name.trim());
                sexTrue.setText(readCard.GetBCardInfo().foreignerOld.sex.trim());
                gjTrue.setText(readCard.GetBCardInfo().foreignerOld.country.trim());
                mzTrue.setText("");
                birthTrue.setText(readCard.GetBCardInfo().foreignerOld.birthday.trim());
                edenname.setText(readCard.GetBCardInfo().foreignerOld.enName.trim());
                edid.setText(readCard.GetBCardInfo().foreignerOld.no.trim());
                addTrue.setText("");
                jgTrue.setText(readCard.GetBCardInfo().foreignerOld.department.trim());

                String startDate = readCard.GetBCardInfo().foreignerOld.beginTime.substring(0, 4) + "-" + readCard.GetBCardInfo().foreignerOld.beginTime.substring(4, 6) + "-" + readCard.GetBCardInfo().foreignerOld.beginTime.substring(6, 8);
                String endDate = "";
                int nlen = readCard.GetBCardInfo().foreignerOld.endTime.indexOf("长期");
                if (nlen != -1) {
                    endDate = "长期";
                } else {
                    endDate = readCard.GetBCardInfo().foreignerOld.endTime.substring(0, 4) + "-" + readCard.GetBCardInfo().foreignerOld.endTime.substring(4, 6) + "-" + readCard.GetBCardInfo().foreignerOld.endTime.substring(6, 8);
                }
                yxqTrue.setText(startDate + "~" + endDate);
                signTrue.setText("");
                othernoTrue.setText("");
            }
            else
            {
                if (readCard.GetBCardInfo().sfz.photo != null) {
                    idimg.setBackground(new BitmapDrawable(Bytes2Bimap(readCard.GetBCardInfo().sfz.photo)));
                }
                edtype.setText(readCard.GetBCardInfo().sfz.type);
                if (readCard.GetBCardInfo().type == 1) {
                    othernoTrue.setText(readCard.GetBCardInfo().sfz.otherNo);
                    signTrue.setText(readCard.GetBCardInfo().sfz.signNum);
                } else if (readCard.GetBCardInfo().type == 2) {
                    othernoTrue.setText(readCard.GetBCardInfo().sfz.otherNo);
                    signTrue.setText(readCard.GetBCardInfo().sfz.signNum);
                } else{
                    othernoTrue.setText("");
                    signTrue.setText("");
                }
                edid.setText(readCard.GetBCardInfo().sfz.no.trim());
                edname.setText(readCard.GetBCardInfo().sfz.name.trim());
                addTrue.setText(readCard.GetBCardInfo().sfz.address.trim());
                sexTrue.setText(readCard.GetBCardInfo().sfz.sex.trim());
                mzTrue.setText(readCard.GetBCardInfo().sfz.nation.trim() + "族");
                gjTrue.setText("中国");
                jgTrue.setText(readCard.GetBCardInfo().sfz.department.trim());
                birthTrue.setText(readCard.GetBCardInfo().sfz.birthday.substring(0, 4) + "-" + readCard.GetBCardInfo().sfz.birthday.substring(4, 6) + "-" + readCard.GetBCardInfo().sfz.birthday.substring(6, 8));

                String startDate = readCard.GetBCardInfo().sfz.beginTime.substring(0, 4) + "-" + readCard.GetBCardInfo().sfz.beginTime.substring(4, 6) + "-" + readCard.GetBCardInfo().sfz.beginTime.substring(6, 8);
                String endDate = "";
                int nlen = readCard.GetBCardInfo().sfz.endTime.indexOf("长期");
                if (nlen != -1) {
                    endDate = "长期";
                } else {
                    endDate = readCard.GetBCardInfo().sfz.endTime.substring(0, 4) + "-" + readCard.GetBCardInfo().sfz.endTime.substring(4, 6) + "-" + readCard.GetBCardInfo().sfz.endTime.substring(6, 8);
                }
                yxqTrue.setText(startDate + "~" + endDate);
            }
        }
        if (msg.what == -9999)
        {
            String  szerr = readCard.GetErrorInfo();
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
        clientAuthInfo authInfo = readCard.GetAppKeyUseNum();
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
                int     tt = 0;
                if (m_bonDevice)
                {
                    readCard.setDeviceType(0);
                }
                else
                {
                    readCard.setDeviceType(1);
                }
                tt = readCard.ReadBCard(intent);
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
        }).start();
    }
}
