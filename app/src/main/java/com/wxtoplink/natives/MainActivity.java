package com.wxtoplink.natives;

import android.Manifest;
import android.content.pm.PackageManager;
import android.graphics.ImageFormat;
import android.media.Image;
import android.os.Environment;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.util.Size;

import com.wxtoplink.base.camera.CameraComponent;
import com.wxtoplink.base.camera.interfaces.PreviewDataCallBack;
import com.wxtoplink.java.ByteDataFormat;
import com.wxtoplink.java.EncryptionCheckUtil;

import java.io.File;
import java.util.Date;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "JNITest";

    private NativeTools nativeTools = new NativeTools();

    private CameraComponent cameraComponent;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        String fileFolder = Environment.getExternalStorageDirectory().getAbsolutePath() + "/ORC/Videos";
        String largeFile = Environment.getExternalStorageDirectory().getAbsolutePath() + "/React_HG.zip";

        javaGetMd5(largeFile,5);//使用Java语言读取md5

        cGetMd5(largeFile,5);//使用C语言JNI方式读取md5

        openCamera();//打开相机，使用java转码以及C转码效率测试
    }

    //使用Java方式获取md5
    private void javaGetMd5(String filePath, int countTime) {
        Log.i(TAG, String.format("Java get md5 start\n\n\n"));
        long startTimeJava = new Date().getTime();
        File file = new File(filePath);
        if (file.exists()) {
            for (; countTime > 0; countTime--) {
                if (file.isFile()) {
                    String md5 = EncryptionCheckUtil.md5sum(filePath);
                    Log.i(TAG, String.format("filePath:%s\nmd5:%s", filePath, md5));
                } else {
                    for (File childFile : file.listFiles()) {
                        String md5 = EncryptionCheckUtil.md5sum(childFile.getAbsolutePath());
                        Log.i("JNITest", String.format("filePath:%s\nmd5:%s", childFile.getName(), md5));
                    }
                }
            }
        } else {
            Log.i(TAG, String.format("file:%s is not exist", filePath));
        }

        Log.e(TAG, String.format("Java get md5 end\ntotal time:%s\n\n", new Date().getTime() - startTimeJava));
    }

    //使用C获取md5
    private void cGetMd5(String filePath, int countTime) {
        Log.i(TAG, String.format("C get md5 start\n\n\n"));
        long startTimeJava = new Date().getTime();
        File file = new File(filePath);
        if (file.exists()) {
            for (; countTime > 0; countTime--) {
                if (file.isFile()) {
                    String md5 = nativeTools.getFileMd5(filePath);
                    Log.i(TAG, String.format("filePath:%s\nmd5:%s", filePath, md5));
                } else {
                    for (File childFile : file.listFiles()) {
                        String md5 = nativeTools.getFileMd5(childFile.getAbsolutePath());
                        Log.i(TAG, String.format("filePath:%s\nmd5:%s", childFile.getName(), md5));
                    }
                }
            }
        } else {
            Log.i(TAG, String.format("file:%s is not exist", filePath));
        }

        Log.e(TAG, String.format("C get md5 end\ntotal time:%s\n\n", new Date().getTime() - startTimeJava));
    }

    private void openCamera() {
        if (cameraComponent == null) {
            cameraComponent = CameraComponent.getInstance();
            cameraComponent.setCameraType(CameraComponent.CAMERA_PREVIEW, this);
            cameraComponent.setMaxSize(new Size(800, 600));
            cameraComponent.setImageFormat(ImageFormat.YUV_420_888);
            cameraComponent.setPreviewDataCallBack(new PreviewDataCallBack() {
                @Override
                public void previewData(Image image) {
                    //Java转码
//                    byte [] data1 = ByteDataFormat.formatYUV420_888ToNV21(image);//平均20ms
                    //C语言转码
                    byte [] data2 = nativeTools.formatYUV420_888ToNV21(image);//平均10ms
                }

                @Override
                public void errorCallBack(Exception e) {

                }

                @Override
                public void surfaceSizeChanged(Size size) {

                }
            });
            if (ActivityCompat.checkSelfPermission(this, Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
                Log.e(TAG, String.format("camera is not permission"));

                return;
            }
            cameraComponent.startPreview();
        }
    }
}
