LOCAL_PATH := $(call my-dir)
    include $(CLEAR_VARS)
    #编译生成的文件的类库叫什么名字
    LOCAL_MODULE    := hello
    #要编译的c文件
    LOCAL_SRC_FILES := testCpp.c \
    md5.c
    include $(BUILD_SHARED_LIBRARY)
    APP_ABI := armeabi armeabi-v7a x86
