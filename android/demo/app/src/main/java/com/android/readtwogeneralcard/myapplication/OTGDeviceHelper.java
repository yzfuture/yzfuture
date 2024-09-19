package com.android.readtwogeneralcard.myapplication;

import static android.hardware.usb.UsbManager.ACTION_USB_DEVICE_ATTACHED;
import static android.hardware.usb.UsbManager.ACTION_USB_DEVICE_DETACHED;

import android.annotation.SuppressLint;
import android.app.Application;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbConstants;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbInterface;
import android.hardware.usb.UsbManager;
import android.os.Build;
import androidx.annotation.NonNull;
import android.util.Log;

import com.blankj.utilcode.util.Utils;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

/**
 * OTGHelper
 *
 * @Author: fanfan@751027@163.com
 * @Date: 2023/7/17 09:38
 * @Description:
 */
@SuppressLint({"StaticFieldLeak", "UnspecifiedRegisterReceiverFlag"})
public class OTGDeviceHelper {
    /*标准版读卡器*/
    private static final int ONLINE_DEVICE_PRODUCT_ID   = 20763;
    private static final  int ONLINE_DEVICE_VENDOR_ID   = 1306;
    /*离线版读卡器*/
    private static final int OFFLINE_DEVICE_PRODUCT_ID   = 50010;
    private static final int OFFLINE_DEVICE_VENDOR_ID    = 1024;
    private static final String ACTION_USB_PERMISSION = "com.android.usb.USB_PERMISSION";
    private static volatile OTGDeviceHelper INSTANCE = null;

    public static void init(@NonNull Application application){
        if (INSTANCE == null){
            synchronized (OTGDeviceHelper.class){
                if (INSTANCE == null){
                    INSTANCE = new OTGDeviceHelper(application);
                }
            }
        }
    }
    private final Context mContext;
    private final UsbManager mUsbManager;
    private final List<IOTGEnableChangedListener> mChangedListeners = new ArrayList<>();
    private OTGDeviceHelper(@NonNull Context context){
        this.mContext = context;
        this.mUsbManager = (UsbManager)context.getSystemService(Context.USB_SERVICE);
        OTGChangedReceiver mOtgChangedReceiver = new OTGChangedReceiver();
        //注册USB变化广播
        IntentFilter dFilter = new IntentFilter();
        //插入设备
        dFilter.addAction(ACTION_USB_DEVICE_ATTACHED);
        //拔出设备
        dFilter.addAction(ACTION_USB_DEVICE_DETACHED);
        //请求授权
        dFilter.addAction(ACTION_USB_PERMISSION);
//        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU){
//            context.registerReceiver(mOtgChangedReceiver, dFilter, Context.RECEIVER_NOT_EXPORTED);
//        }else {
//            context.registerReceiver(mOtgChangedReceiver, dFilter);
//        }
        context.registerReceiver(mOtgChangedReceiver, dFilter);

        trySupportDevice();
    }

    public static OTGDeviceHelper getInstance(){
        return INSTANCE;
    }

    /**
     * 判断是否有可用的读卡设备
     */
    public boolean readerDeviceEnable(){
        if (mUsbManager != null){
            HashMap<String, UsbDevice> devices = mUsbManager.getDeviceList();
            if (devices != null){
                for (UsbDevice device : devices.values()){
                    if (isReaderDevice(device) && mUsbManager.hasPermission(device)){
                        return true;
                    }
                }
            }
        }
        return false;
    }

    /**
     * 判断是否有可用的存储设备
     */
    public boolean storageDeviceEnable(){
        if (mUsbManager != null){
            HashMap<String, UsbDevice> devices = mUsbManager.getDeviceList();
            if (devices != null){
                for (UsbDevice device : devices.values()){
                    if (isStorageDevice(device) && mUsbManager.hasPermission(device)){
                        return true;
                    }
                }
            }
        }
        return false;
    }

    /**
     * 判断是否有可用的打印设备
     */
    public boolean printerDeviceEnable(){
        if (mUsbManager != null){
            HashMap<String, UsbDevice> devices = mUsbManager.getDeviceList();
            if (devices != null){
                for (UsbDevice device : devices.values()){
                    if (isPrinterDevice(device) && mUsbManager.hasPermission(device)){
                        return true;
                    }
                }
            }
        }
        return false;
    }

    public void addListener(@NonNull IOTGEnableChangedListener listener){
        if (!this.mChangedListeners.contains(listener)){
            this.mChangedListeners.add(listener);
        }
    }

    public void removeListener(@NonNull IOTGEnableChangedListener listener){
        this.mChangedListeners.remove(listener);
    }

    /**
     * 尝试启用支持的设备
     */
    private void trySupportDevice(){
        if (mUsbManager != null){
            HashMap<String, UsbDevice> devices = mUsbManager.getDeviceList();
            if (devices != null){
                for (UsbDevice device : devices.values()){
                    if (isSupportDevice(device)){
                        getDevicePermission(device);
                    }
                }
            }
        }
    }

    /**
     * 获取设备权限
     */
    private void getDevicePermission(UsbDevice device){
        if (device != null){
            PendingIntent pendingIntent;
            if (Build.VERSION.SDK_INT >= 34){
                Intent intent = new Intent(ACTION_USB_PERMISSION);
                intent.setPackage(Utils.getApp().getPackageName());
                pendingIntent = PendingIntent.getBroadcast(mContext,
                        0,
                        intent,
                        PendingIntent.FLAG_MUTABLE);
            }else if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S){
                pendingIntent = PendingIntent.getBroadcast(mContext,
                        0,
                        new Intent(ACTION_USB_PERMISSION),
                        PendingIntent.FLAG_MUTABLE);
            }else {
                pendingIntent = PendingIntent.getBroadcast(mContext,
                        0,
                        new Intent(ACTION_USB_PERMISSION),
                        0);
            }
            mUsbManager.requestPermission(device, pendingIntent);
        }
    }

    /**
     * 判断是否需要支持读卡器
     */
    private boolean isSupportDevice(UsbDevice device){
        return true;
//        return isStorageDevice(device) ||
//                isReaderDevice(device) ||
//                isPrinterDevice(device);
    }

    /**
     * 读卡器判断
     */
    private boolean isReaderDevice(UsbDevice device){
        return device != null && (
                (device.getProductId() == ONLINE_DEVICE_PRODUCT_ID && device.getVendorId() == ONLINE_DEVICE_VENDOR_ID) ||
                        (device.getProductId() == OFFLINE_DEVICE_PRODUCT_ID && device.getVendorId() == OFFLINE_DEVICE_VENDOR_ID)
        );
    }

    /**
     * U盘判断
     */
    private boolean isStorageDevice(UsbDevice device){
        if (device != null){
            for (int i = 0; i < device.getInterfaceCount(); i++) {
                UsbInterface usbInterface = device.getInterface(i);
                // 获取usb设备类型，判断当前连接的usb设备是否为存储设备（u盘或读卡器）
                int interfaceClass = usbInterface.getInterfaceClass();
                if (interfaceClass == UsbConstants.USB_CLASS_MASS_STORAGE) {
                    return true;
                }
            }
        }
        return false;
    }
    public static final int[] XY_PRINTER_ID = new int[]{1659, 1046, 7358, 1155, 8137, 1003, 11575, 1208, 22304, 26728};
    private boolean isPrinterDevice(UsbDevice device){
        if (device != null){
            for (int xyId : XY_PRINTER_ID){
                if (device.getVendorId() == xyId){
                    return true;
                }
            }
        }
        return false;
    }

    private void handlerChangedListener(){
        for (IOTGEnableChangedListener listener : mChangedListeners){
            listener.enableDeviceChanged();
        }
    }

    public interface IOTGEnableChangedListener {
        void enableDeviceChanged();
    }

    private class OTGChangedReceiver extends BroadcastReceiver{
        public OTGChangedReceiver() {
        }

        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (action != null){
                if (action.equals(ACTION_USB_DEVICE_ATTACHED)){
                    Log.e("fanfan--------------", "发现新设备");
                    UsbDevice mDevice = intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
                    if (isSupportDevice(mDevice)){
                        getDevicePermission(mDevice);
                    }
                } else if (action.equals(ACTION_USB_PERMISSION) || action.equals(ACTION_USB_DEVICE_DETACHED)){
                    Log.e("fanfan--------------", "请求权限完成");
                    handlerChangedListener();
                }
            }
        }
    }
}
