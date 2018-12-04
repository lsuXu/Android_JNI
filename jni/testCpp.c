//
// Created by 12852 on 2018/11/21.
//
#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
#include <string.h>
#include <memory.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "md5.h"

#define READ_DATA_SIZE	1024
#define MD5_SIZE		16
#define MD5_STR_LEN		(MD5_SIZE * 2)

#ifndef _Included_com_wxtoplink_natives_NativeTools
#define _Included_com_wxtoplink_natives_NativeTools

//预先声明函数，java中的String转换为C语言中的String类型
char * Jstring2CStr(JNIEnv * env,jstring jstr)
    ;

int Compute_file_md5(const char *file_path, char *md5_str)
;

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_wxtoplink_natives_NativeTools
 * Method:    getString
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_wxtoplink_natives_NativeTools_getString__
  (JNIEnv * env, jobject obj)
  {
          char* cstr = "jni test1";

          jstring jstr = (*env)->NewStringUTF(env,cstr);

          return jstr ;
  };

/*
 * Class:     com_wxtoplink_natives_NativeTools
 * Method:    getString
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_wxtoplink_natives_NativeTools_getString__Ljava_lang_String_2
  (JNIEnv * env, jobject jobj, jstring str)
  {
      char* cstr = Jstring2CStr(env,str);

      jstring jstr = (*env)->NewStringUTF(env,cstr);

      return jstr;

  };


  /*
   * Class:     com_wxtoplink_natives_NativeTools
   * Method:    getFileMd5
   * Signature: (Ljava/lang/String;)Ljava/lang/String;
   */

  JNIEXPORT jstring JNICALL Java_com_wxtoplink_natives_NativeTools_getFileMd5
    (JNIEnv * env, jobject jobj, jstring filePath)
    {
        char * cPath = Jstring2CStr(env,filePath);

        char md5_str[MD5_STR_LEN + 1];

        int result ;

        result = Compute_file_md5(cPath,md5_str);

        if (0 == result)
        {
            printf("[file - %s] md5 value:\n", filePath);
            printf("%s\n", md5_str);

            return (*env)->NewStringUTF(env,md5_str);
        }

        return 0;

    };

/**
 * 将java的String类型传值转换为C的String类型传值
 * @param env
 * @param jstr
 * @return
 */
    char * Jstring2CStr(JNIEnv * env,jstring jstr)
    {
        char * ctn = NULL;

        //获取java 包下的String类
        jclass classString = (*env) ->FindClass(env,"java/lang/String");
        //设置编码
        jstring strCode = (*env)->NewStringUTF(env,"GB2312");

        //获取String的getBytes方法
        jmethodID strMethod = (*env)->GetMethodID(env,classString,"getBytes","(Ljava/lang/String;)[B");

        jbyteArray strByteArray = (jbyteArray) (*env) -> CallObjectMethod(env,jstr, strMethod, strCode);//对应于Java中的String.getByte("GB2312");

        jsize strSize = (*env)-> GetArrayLength(env,strByteArray);

        jbyte * ba = (*env)->GetByteArrayElements(env,strByteArray,JNI_FALSE);

        if(strSize > 0)
        {
            ctn = (char *) malloc(strSize +1);
            memcpy(ctn,ba,strSize);
            ctn[strSize] = '\0';
        }

        (*env)-> ReleaseByteArrayElements(env,strByteArray,ba,0);

        return ctn ;
    }

    /*
     * Class:     com_wxtoplink_natives_NativeTools
     * Method:    formatYUV420_888ToNV21
     * Signature: ([B[B[B)[B
     */
    JNIEXPORT jbyteArray JNICALL Java_com_wxtoplink_natives_NativeTools_formatYUV420_1888ToNV21
            (JNIEnv * env, jobject obj, jbyteArray arrayY, jbyteArray arrayU, jbyteArray arrayV)
    {
        int sizeY = (*env) ->GetArrayLength(env,arrayY);
        int sizeU = (*env) ->GetArrayLength(env,arrayU);
        int sizeV = (*env) ->GetArrayLength(env,arrayV);

        jbyte * byteU = (*env) ->GetByteArrayElements(env,arrayU,JNI_FALSE);
        jbyte * byteV = (*env) ->GetByteArrayElements(env,arrayV,JNI_FALSE);
        jbyte * byteY = (*env) ->GetByteArrayElements(env,arrayY,JNI_FALSE);

        //用于保存转码后的数据
        jbyteArray data = (*env) ->NewByteArray(env,sizeU+sizeV+sizeY);

        //FIXME ,error
        //把y的数据赋值到
//        (*env) ->SetByteArrayRegion(env,data,0,sizeY,arrayY);
//        memcpy(data +1,arrayY,sizeY);

        jbyte * dataByte = (*env) ->GetByteArrayElements(env,data,JNI_FALSE);

        for(int i = 0;i<sizeY;i++){
            dataByte[i] = byteY[i];
        }

        for(int i =0,acc = sizeY;i<sizeU;i++)
        {
            dataByte[acc] = byteV[i];
            dataByte[acc +1] = byteU[i];
            acc += 2 ;
        }

        return data;
    };


    /**
     * 计算文件的md5值
     * @param file_path 文件路径
     * @param md5_str  保存md5计算结果的指针
     * @return
     */
    int Compute_file_md5(const char *file_path, char *md5_str)
    {
        int i;
        int fd;
        int ret;
        unsigned char data[READ_DATA_SIZE];
        unsigned char md5_value[MD5_SIZE];
        MD5_CTX md5;

        fd = open(file_path, O_RDONLY);
        if (-1 == fd)
        {
            perror("open");
            return -1;
        }

        // init md5
        MD5Init(&md5);

        while (1)
        {
            ret = read(fd, data, READ_DATA_SIZE);
            if (-1 == ret)
            {
                perror("read");
                return -1;
            }

            MD5Update(&md5, data, ret);

            if (0 == ret || ret < READ_DATA_SIZE)
            {
                break;
            }
        }

        close(fd);

        MD5Final(&md5, md5_value);

        for(i = 0; i < MD5_SIZE; i++)
        {
            snprintf(md5_str + i*2, 2+1, "%02x", md5_value[i]);
        }
        md5_str[MD5_STR_LEN] = '\0'; // add end

        return 0;
    }

#ifdef __cplusplus
}
#endif
#endif