/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_wxtoplink_natives_NativeTools */

#ifndef _Included_com_wxtoplink_natives_NativeTools
#define _Included_com_wxtoplink_natives_NativeTools
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_wxtoplink_natives_NativeTools
 * Method:    getString
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_wxtoplink_natives_NativeTools_getString__
  (JNIEnv *, jobject);

/*
 * Class:     com_wxtoplink_natives_NativeTools
 * Method:    getString
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_wxtoplink_natives_NativeTools_getString__Ljava_lang_String_2
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_wxtoplink_natives_NativeTools
 * Method:    getFileMd5
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_wxtoplink_natives_NativeTools_getFileMd5
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_wxtoplink_natives_NativeTools
 * Method:    formatYUV420_888ToNV21
 * Signature: ([B[B[B)[B
 */
JNIEXPORT jbyteArray JNICALL Java_com_wxtoplink_natives_NativeTools_formatYUV420_1888ToNV21
  (JNIEnv *, jobject, jbyteArray, jbyteArray, jbyteArray);

#ifdef __cplusplus
}
#endif
#endif
