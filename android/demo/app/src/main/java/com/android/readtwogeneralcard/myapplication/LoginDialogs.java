package com.android.readtwogeneralcard.myapplication;

import android.app.Dialog;
import android.content.Context;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.android.readtwogeneralcard.myapplication.R;

/**
 * Created by 65 on 2015/6/19.
 */
//自定义Dialog
public class LoginDialogs {
    /**
     * 得到自定义的progressDialog
     *
     * @param context
     * @param msg
     * @return
     */
    public static Dialog createLoadingDialog(Context context, String msg, int cut) {
        LayoutInflater inflater = LayoutInflater.from(context);
        View v = inflater.inflate(R.layout.login_dialog, null);// 得到加载view
        LinearLayout layout = v.findViewById(R.id.dialog_view);// 加载布局
        // main.xml中的ImageView
        ImageView spaceshipImage = v.findViewById(R.id.img);
        TextView tipTextView = v.findViewById(R.id.tipTextView);// 提示文字

        if (cut == 1) {//正在进行
            spaceshipImage.setImageResource(R.mipmap.jz);
            // 加载动画
            Animation hyperspaceJumpAnimation = AnimationUtils.loadAnimation(
                    context, R.anim.loading_animation);
            // 使用ImageView显示动画
            spaceshipImage.startAnimation(hyperspaceJumpAnimation);
        } else if (cut == 2) {//失败
            spaceshipImage.setImageResource(R.mipmap.cw);
        } else if (cut == 3) {//成功
            spaceshipImage.setImageResource(R.mipmap.cg);
        }
        tipTextView.setText(msg);// 设置加载信息
        Dialog loadingDialog = new Dialog(context, R.style.loading_dialog);// 创建自定义样式dialog
        loadingDialog.setCanceledOnTouchOutside(false);//点击屏幕不消失
        loadingDialog.setCancelable(false);// 可以用“返回键”取消
        loadingDialog.setContentView(layout, new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.MATCH_PARENT,
                LinearLayout.LayoutParams.MATCH_PARENT));// 设置布局
        Window dialogWindow = loadingDialog.getWindow();
        WindowManager.LayoutParams lp = dialogWindow.getAttributes();
        dialogWindow.setGravity(Gravity.BOTTOM);
        return loadingDialog;

    }
}
