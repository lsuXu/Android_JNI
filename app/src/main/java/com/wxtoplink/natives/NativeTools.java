package com.wxtoplink.natives;

import android.media.Image;
import android.util.Log;

import java.nio.ByteBuffer;
import java.util.Date;

/**
 * Created by 12852 on 2018/11/21.
 */

public class NativeTools {

    static{
        System.loadLibrary("hello");
    }

    public native String getString();

    public native String getString(String string);

    public native String getFileMd5(String filePath);

    public byte[] formatYUV420_888ToNV21(Image image){
        long startTime = new Date().getTime();
        Image.Plane[] pa = image.getPlanes();
        ByteBuffer [] byteBuffers = new ByteBuffer[3];
        for(int i =0;i<3;i++){
            byteBuffers[i] = pa[i].getBuffer();
        }
        byte[] byteY =new byte[byteBuffers[0].capacity()];
        byte[] byteU = new byte[byteBuffers[1].capacity()];
        byte[] byteV = new byte[byteBuffers[2].capacity()];
        pa[0].getBuffer().get(byteY,0,byteBuffers[0].capacity());
        pa[1].getBuffer().get(byteU,0,byteBuffers[1].capacity());
        pa[2].getBuffer().get(byteV,0,byteBuffers[2].capacity());

        byte [] result = formatYUV420_888ToNV21(byteY,byteU,byteV);
        Log.i("Native format",String.format("user time :%s",new Date().getTime()-startTime));
        return result;
    }

    public native byte[] formatYUV420_888ToNV21(byte[] buffY,byte[] buffU,byte[] bufferV);

}
