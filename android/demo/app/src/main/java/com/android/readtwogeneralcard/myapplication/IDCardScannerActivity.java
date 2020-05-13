package com.android.readtwogeneralcard.myapplication;

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
import android.os.Message;
import android.text.TextUtils;
import android.text.method.ScrollingMovementMethod;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.readTwoGeneralCard.ActiveCallBack;
import com.readTwoGeneralCard.OTGReadCardAPI;
import com.readTwoGeneralCard.PermissionUtil;
import com.readTwoGeneralCard.Serverinfo;
import com.readTwoGeneralCard.eCardType;
import com.recognition.BaseRecognitionActivity;
import com.util.GASystemCamera;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.FileNotFoundException;
import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.HashMap;

import butterknife.Bind;
import butterknife.ButterKnife;

import static java.lang.Thread.sleep;
import static com.android.readtwogeneralcard.myapplication.ResultActivity.EXTRA_INTENT_KEY_COMPARE_BITMAP_URL;
import static com.android.readtwogeneralcard.myapplication.ResultActivity.EXTRA_INTENT_KEY_ORIGIN_BITMAP;

public class IDCardScannerActivity extends BaseRecognitionActivity implements ActiveCallBack {
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
    @Bind(R.id.appkeyTxt)
    TextView appkeyTxt;
    @Bind(R.id.regeitBtn)
    Button regeitBtn;
    @Bind(R.id.modifyBtn)
    Button modifyBtn;
    @Bind(R.id.findBtn)
    Button findBtn;

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
    @Bind(R.id.showBtn)
    Button showBtn;
    @Bind(R.id.otgBtn)
    Button otgBtn;
    @Bind(R.id.faceDection)
    Button faceDection;

    @Bind(R.id.process)
    TextView process;
    @Bind(R.id.escapedTrue)
    TextView escapedTrue;

    private NfcAdapter      mAdapter = null;
    private PendingIntent   pi = null;
    private IntentFilter    tagDetected = null;
    private String[][]      mTechLists;
    private Intent          inintent = null;
    private final Handler   mHandler = new MyHandler(this);
    private OTGReadCardAPI  ReadCardAPI;
    private boolean         bTestServer = false;
    private boolean         bNFC = false;

    private String          m_szUserInfo = "";
    private boolean         m_bshow = false;
    private boolean         m_bAuthon = true;
    private boolean         m_berror = false;
    private String          m_szProcess = "";
    private SharedPreferences sharedPreferences = null;
    private String          m_szAppKey="";
    private Button          timeButton;
    private Bitmap          bkbmp = null;
    private updateApk       updateLp = null;
    private final static String ACTION ="android.hardware.usb.action.USB_STATE";
    private BroadcastReceiver usBroadcastReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            Toast.makeText(IDCardScannerActivity.this,"aciton ="+ action, Toast.LENGTH_SHORT).show();
            if (action.equals(ACTION)) {
                boolean connected = intent.getExtras().getBoolean("connected");
                Toast.makeText(IDCardScannerActivity.this,"aciton ="+ connected, Toast.LENGTH_SHORT).show();
                if (connected) {
                    UsbDevice       mUsbDevice = null;
                    PendingIntent mPendingIntent = PendingIntent.getBroadcast(context, 0, new Intent("com.android.example.USB_PERMISSION"), 0);
                    UsbManager mUsbManager = ((UsbManager) context.getSystemService("usb"));
                    HashMap<String, UsbDevice> deviceList = mUsbManager.getDeviceList();
                    if (!deviceList.isEmpty()) {
                        int       m_nVID = 1306;
                        int       m_nPID = 20763;
                        for (UsbDevice device : deviceList.values()) {
                            if ((m_nVID == device.getVendorId()) && (m_nPID == device.getProductId())) {
                                mUsbDevice = device;
                                break;
                            }
                        }
                    }
                    if (false == mUsbManager.hasPermission(mUsbDevice)) {
                        mUsbManager.requestPermission(mUsbDevice, mPendingIntent);
                    }
                }
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_idget);
        ButterKnife.bind(this);
        Resources res = getResources();

        IntentFilter filter = new IntentFilter();
        filter.addAction(ACTION);
        registerReceiver(usBroadcastReceiver, filter);

        sharedPreferences= getSharedPreferences("appConfig",MODE_PRIVATE);
        m_szAppKey = sharedPreferences.getString("AppKey", "");
        appkeyTxt.setTextIsSelectable(true);
        init();
        bkbmp = BitmapFactory.decodeResource(res, R.mipmap.logo);
        userInfo.setMovementMethod(ScrollingMovementMethod.getInstance());
        resetBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                resetText();
            }
        });
        faceDection.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                takePicture();
            }
        });
        showBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                LayoutInflater li = LayoutInflater.from(IDCardScannerActivity.this);
                View promptsView = li.inflate(R.layout.pinnumenter, null);

                AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(IDCardScannerActivity.this);
                alertDialogBuilder.setView(promptsView);
                final EditText userInput = (EditText) promptsView
                        .findViewById(R.id.editTextDialogUserInput);
                final TextView pinView = (TextView) promptsView.findViewById(R.id.pintextView1);
                pinView.setText("请输入显示码！");
                final TextView pinState = (TextView) promptsView.findViewById(R.id.pintextState);
                pinState.setText("");
                TextView can = (TextView)promptsView.findViewById(R.id.pin_can);
                TextView sure = (TextView)promptsView.findViewById(R.id.pin_sure);
                final String[] szEidResult = {null};
                final AlertDialog alertDialog = alertDialogBuilder.create();
                can.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        alertDialog.dismiss();
                    }
                });
                sure.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        if (userInput.getText().toString().equals("yzwl"))
                        {
                            setdialog("打开隐藏功能");
                            m_bshow = true;
                            m_bAuthon = false;
                            m_berror = true;
                        }
                        else
                        {
                            setdialog("关闭隐藏功能");
                            m_bshow = false;
                            m_bAuthon = false;
                            m_berror = false;
                        }
                        alertDialog.dismiss();
                    }
                });

                alertDialog.show();
            }
        });

        otgBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                mHandler.sendEmptyMessage(1005);
            }
        });
        regeitBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                regeditAppkey(true);
                appkeyTxt.setText(m_szAppKey);
            }
        });
        modifyBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                modifyAppkey();
                appkeyTxt.setText(m_szAppKey);
            }
        });
        findBtn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                regeditAppkey(false);
                appkeyTxt.setText(m_szAppKey);
            }
        });

        if (m_szAppKey.isEmpty())
        {
            regeditAppkey(true);
        }
        appkeyTxt.setText(m_szAppKey);

        int     nnum = ReadCardAPI.GetAppKeyUseNum(m_szAppKey);
        if (nnum <= 0) nnum = 0;
        numtxt.setText(String.valueOf(nnum));

        PermissionUtil.grantNeedPermission(this);

        updateLp = new updateApk();
        updateLp.update(this, this, mHandler);
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
        escapedTrue.setText("");
        idimg.setBackground(new BitmapDrawable(bkbmp));
    }

    private void modifyAppkey()
    {
        LayoutInflater li = LayoutInflater.from(IDCardScannerActivity.this);
        View promptsView = li.inflate(R.layout.modifyappkey_idget, null);

        AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(IDCardScannerActivity.this);
        alertDialogBuilder.setView(promptsView);
        final EditText    appkeytxt = (EditText) promptsView.findViewById(R.id.appkeytxt);
        Button modifybtn = (Button)promptsView.findViewById(R.id.modifybtn);
        final AlertDialog alertDialog = alertDialogBuilder.create();
        modifybtn.setOnClickListener(new Button.OnClickListener(){
            @Override
            public void onClick(View v) {
                String  szAppKeyTxt = appkeytxt.getText().toString();
                if (szAppKeyTxt.isEmpty())
                {
                    setdialog("新的appKey不能为空");
                }
                else
                {
                    m_szAppKey = szAppKeyTxt;
                    SharedPreferences.Editor editor = sharedPreferences.edit();
                    editor.putString("AppKey", m_szAppKey);
                    editor.commit();
                    int     nnum = ReadCardAPI.GetAppKeyUseNum(m_szAppKey);
                    if (nnum <= 0) nnum = 0;
                    numtxt.setText(String.valueOf(nnum));
                    appkeyTxt.setText(m_szAppKey);
                    setdialog("新appKey已经替换");
                    alertDialog.dismiss();
                }
            }
        });

        alertDialog.show();
    }

    private void regeditAppkey(final boolean bshowName)
    {
        LayoutInflater li = LayoutInflater.from(IDCardScannerActivity.this);
        View promptsView = li.inflate(R.layout.register_idget, null);

        AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(IDCardScannerActivity.this);
        alertDialogBuilder.setView(promptsView);
        final EditText    phonetxt = (EditText) promptsView.findViewById(R.id.phonetxt);
        final EditText    codetxt = (EditText) promptsView.findViewById(R.id.codetxt);
        final EditText    nametxt = (EditText) promptsView.findViewById(R.id.nametxt);
        timeButton = (Button)promptsView.findViewById(R.id.timebtn);
        Button      registerbtn = (Button)promptsView.findViewById(R.id.registerbtn);
        if (!bshowName)
        {
            nametxt.setVisibility(View.GONE);
            registerbtn.setText("找回");
        }
        else
        {
            nametxt.setVisibility(View.VISIBLE);
            registerbtn.setText("注册");
        }
        final AlertDialog alertDialog = alertDialogBuilder.create();
        final MyCountDownTimer myCountDownTimer = new MyCountDownTimer(60000,1000);

        timeButton.setOnClickListener(new Button.OnClickListener(){
            @Override
            public void onClick(View v) {
                String  szPhoneTxt = phonetxt.getText().toString();
                if (isMobileNO(szPhoneTxt))
                {
                    int     ncode = ReadCardAPI.GetVerificationCode(szPhoneTxt);
                    if (ncode == 0)
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

        registerbtn.setOnClickListener(new Button.OnClickListener(){

            @Override
            public void onClick(View v) {
                if (!bshowName)
                {
                    String  szPhoneTxt = phonetxt.getText().toString();
                    String  szCodeTxt = codetxt.getText().toString();
                    if (szPhoneTxt.isEmpty() || szCodeTxt.isEmpty())
                    {
                        setdialog("以上信息不能为空");
                    }
                    else
                    {
                        String  szretJson = ReadCardAPI.FindLogonAppKey(szPhoneTxt, szCodeTxt);
                        if (szretJson.isEmpty())
                        {
                            setdialog("找回appKey失败，请重获取验证码");
                        }
                        else
                        {
                            try {
                                JSONObject jsonObject=new JSONObject(szretJson);
                                int         nerror = jsonObject.getInt("ret");
                                if (nerror == 0)
                                {
                                    m_szAppKey = jsonObject.getString("appKey");
                                    SharedPreferences.Editor editor = sharedPreferences.edit();
                                    editor.putString("AppKey", m_szAppKey);
                                    editor.commit();
                                    int     nnum = ReadCardAPI.GetAppKeyUseNum(m_szAppKey);
                                    if (nnum <= 0) nnum = 0;
                                    numtxt.setText(String.valueOf(nnum));
                                    appkeyTxt.setText(m_szAppKey);
                                    setdialog("找回appKey成功，appKey已保存");
                                }
                                else
                                {
                                    setdialog("找回appKey失败，原因:" + jsonObject.getString("error"));
                                }
                            } catch (JSONException e) {
                                e.printStackTrace();
                                setdialog("找回appKey失败，原因:" + e.getMessage());
                            }
                            alertDialog.dismiss();
                        }
                    }
                }
                else
                {
                    String  szPhoneTxt = phonetxt.getText().toString();
                    String  szCodeTxt = codetxt.getText().toString();
                    String  szNameTxt = nametxt.getText().toString();
                    if (szPhoneTxt.isEmpty() || szCodeTxt.isEmpty() || szNameTxt.isEmpty())
                    {
                        setdialog("以上信息不能为空");
                    }
                    else
                    {
                        String  szretJson = ReadCardAPI.GetLogonAppKey(szPhoneTxt, szCodeTxt, szNameTxt);
                        if (szretJson.isEmpty())
                        {
                            setdialog("注册失败，请重获取验证码");
                        }
                        else
                        {
                            try {
                                JSONObject jsonObject=new JSONObject(szretJson);
                                int         nerror = jsonObject.getInt("ret");
                                if (nerror == 0)
                                {
                                    m_szAppKey = jsonObject.getString("appKey");
                                    SharedPreferences.Editor editor = sharedPreferences.edit();
                                    editor.putString("AppKey", m_szAppKey);
                                    editor.commit();
                                    int     nnum = ReadCardAPI.GetAppKeyUseNum(m_szAppKey);
                                    if (nnum <= 0) nnum = 0;
                                    numtxt.setText(String.valueOf(nnum));
                                    appkeyTxt.setText(m_szAppKey);
                                    setdialog("注册成功，appKey已保存，有100次免费使用机会");
                                }
                                else
                                {
                                    setdialog("注册失败，原因:" + jsonObject.getString("error"));
                                }
                            } catch (JSONException e) {
                                e.printStackTrace();
                                setdialog("注册失败，原因:" + e.getMessage());
                            }
                            alertDialog.dismiss();
                        }
                    }
                }
            }
        });

        alertDialog.show();
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

    @Override
    public void onActivityResultOK(int requestCode, int resultCode, Intent data) {
        startActivity(new Intent(this, ResultActivity.class)
                .putExtra(EXTRA_INTENT_KEY_ORIGIN_BITMAP, ReadCardAPI.GetTwoCardInfo().arrTwoIdPhoto == null ? null : Bytes2Bimap(ReadCardAPI.GetTwoCardInfo().arrTwoIdPhoto))
                .putExtra(EXTRA_INTENT_KEY_COMPARE_BITMAP_URL, GASystemCamera.getCurrentPhotoPath()));

    }

    @Override
    public void recognitionFinish(double dconfidence) {

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

    public void init() {
        ReadCardAPI = new OTGReadCardAPI(getApplicationContext(), this, bNFC);

        ArrayList<Serverinfo> twoCardServerlist = new ArrayList<Serverinfo>();
        twoCardServerlist.add(new Serverinfo("id.yzfuture.cn", 8848));
        ReadCardAPI.setServerInfo(twoCardServerlist, null, bTestServer);
        mAdapter = NfcAdapter.getDefaultAdapter(getApplicationContext());
        if (mAdapter != null) {
            init_NFC();
        } else {
            if (ReadCardAPI != null) {
                setdialog("本机不支持NFC功能！");
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
            if (msg.what == 1002)
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

    public void todo(Message msg) throws FileNotFoundException {
        int tt = 0;
        m_szProcess = "";
        process.setText("");
        m_szAppKey = sharedPreferences.getString("AppKey", "");
        int     nnum = ReadCardAPI.GetAppKeyUseNum(m_szAppKey);
        if (nnum <= 0) nnum = 0;
        numtxt.setText(String.valueOf(nnum));
        if (m_szAppKey.isEmpty() || nnum<=0)
        {
            regeditAppkey(true);
        }
        appkeyTxt.setText(m_szAppKey);
        if (!m_szAppKey.isEmpty() && nnum>0)
        {
            if (msg.what == 1 || msg.what==1005)
            {
                if (msg.what == 1005)
                {
                    inintent = null;
                }
                tt = ReadCardAPI.NfcReadCard(m_szAppKey, null, inintent, eCardType.eTwoGeneralCard, "", m_bAuthon);
            }
            if (m_bshow)
            {
                userInfo.setText(m_szUserInfo);
            }
            if (tt == 41)
            {
                if (m_berror)
                {
                    setdialog("读卡失败！" + ReadCardAPI.GetErrorInfo());
                }
                else
                {
                    int     nerr = ReadCardAPI.GetErrorCode();
                    if (nerr==-24997)
                    {
                        setdialog("此设备没有解码权限！");
                    }
                    else setdialog("读卡失败！");
                }

                resetText();
            }
            if (tt == 90)
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
                escapedTrue.setText("否");
                //process.setText("100");
                ReadCardAPI.release();
            }
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
        inintent = intent;
        mHandler.sendEmptyMessageDelayed(1, 0);
    }

    @Override
    public void setUserInfo(String s)
    {
        m_szUserInfo += s + "\r\n";
    }

    @Override
    public void DeviceOpenFailed(boolean b, boolean b1)
    {
        if (!b && !b1)
        {
            setdialog("请检查是否打开NFC开关或已插好USB读卡器，然后重新打开程序，否则无法进行后续操作！");
        }
    }

    @Override
    public void readProgress(int nprocess) {
        m_szProcess = Integer.toString((nprocess*100)/20);
        m_szUserInfo += "进度：" + m_szProcess + "\r\n";

        //process.setText(m_szProcess);
    }
}
