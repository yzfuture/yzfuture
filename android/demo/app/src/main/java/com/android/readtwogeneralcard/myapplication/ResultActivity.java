package com.android.readtwogeneralcard.myapplication;

import android.app.Dialog;
import android.content.Intent;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.text.TextUtils;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import com.recognition.BaseRecognitionActivity;
import com.util.ImageUtil;

/**
 * Created by Administrator on 2018-06-07.
 *
 * @author kzaxil
 * @since 1.0.0
 */
public class ResultActivity extends BaseRecognitionActivity {
    public static final String EXTRA_INTENT_KEY_ORIGIN_BITMAP = "extra_intent_key_origin_bitmap";
    public static final String EXTRA_INTENT_KEY_ORIGIN_BITMAP_URL = "extra_intent_key_origin_bitmap_url";
    public static final String EXTRA_INTENT_KEY_COMPARE_BITMAP_URL = "extra_intent_key_compare_bitmap_url";

    private Bitmap bmpOrigin;
    private Bitmap bmpCompare;
    private ImageView ivOrigin;
    private ImageView ivCompare;
    private TextView tvResult;
    private ImageView ivResult;
    private Dialog mDialog;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.ac_result);
        setupView();
    }

    private void setupView() {
        ivOrigin = findViewById(R.id.iv_origin);
        ivCompare = findViewById(R.id.iv_compare);
        tvResult = findViewById(R.id.tv_result);
        ivResult = findViewById(R.id.iv_result);
    }

    @Override
    protected void onResume() {
        super.onResume();
        bmpOrigin = getIntent().getParcelableExtra(EXTRA_INTENT_KEY_ORIGIN_BITMAP);
        if (bmpOrigin == null) {
            bmpOrigin = ImageUtil.getPortraitBitmap(getIntent().getStringExtra(EXTRA_INTENT_KEY_ORIGIN_BITMAP_URL));
        }
        bmpCompare = ImageUtil.getPortraitBitmap(getIntent().getStringExtra(EXTRA_INTENT_KEY_COMPARE_BITMAP_URL));
        ivOrigin.setImageBitmap(bmpOrigin);
        ivCompare.setImageBitmap(bmpCompare);
        showDialog();
        recognition_submit(bmpOrigin, bmpCompare);
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (bmpOrigin != null) bmpOrigin.recycle();
        if (bmpCompare != null) bmpCompare.recycle();
        dismissDialog();
    }

    private void success(String desc) {
        tvResult.setText(TextUtils.isEmpty(desc) ? "成功" : desc);
        ivResult.setImageResource(R.drawable.ic_check_circle_black_48dp);
    }

    private void fail(String desc) {
        tvResult.setText(TextUtils.isEmpty(desc) ? "失败" : desc);
        ivResult.setImageResource(R.drawable.ic_cancel_black_48dp);
    }

    public void back(View view) {
        finish();
    }

    @Override
    public void onActivityResultOK(int requestCode, int resultCode, Intent data) {

    }

    private void showDialog() {
        if (mDialog != null && mDialog.isShowing()) return;
        mDialog = LoginDialogs.createLoadingDialog(this, "请稍候...", 1);
        mDialog.show();
    }

    private void dismissDialog() {
        if (mDialog == null) return;
        mDialog.dismiss();
        mDialog = null;
    }

    @Override
    public void recognitionFinish(double dconfidence) {
        dismissDialog();
        if (dconfidence <= 0) {
            fail("识别失败[" + dconfidence + "]");
            return;
        }
        else if (dconfidence > 51d)
        {
            success("识别成功[" + dconfidence + "]");
        }
        else
            {
            fail("相似度过低[" + dconfidence + "]");
        }
    }
}
