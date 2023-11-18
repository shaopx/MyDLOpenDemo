#include <jni.h>
#include <string>
#include <dlfcn.h>
#include <android/log.h>
#include <iostream>
#include "fake_dlfcn.h"
#include "HookHelper.h"

// 定义函数指针类型，与DumpJavaStack函数签名匹配
//http://aospxref.com/android-13.0.0_r3/xref/art/runtime/thread.cc#1196
typedef void (*DumpJavaStackFunc)(void *,std::ostream& os, bool param1, bool param2);
#define DUMP_JAVA_STACK_SIG "_ZNK3art6Thread13DumpJavaStackERNSt3__113basic_ostreamIcNS1_11char_traitsIcEEEEbb"

#define JNIHOOK_CLASS "com/spx/mydlopendemo/MainActivity"

#define TAG_NAME    "my_native"
#define log_info(fmt, args...) __android_log_print(ANDROID_LOG_INFO, TAG_NAME, (const char *) fmt, ##args)

//extern "C" JNIEXPORT jstring JNICALL
_jstring * native_stringFromJNI(
        JNIEnv *env, jclass, jlong self /* this */) {
    std::string hello = "Hello from C++";
    void *handle = mydlopen("libart.so", RTLD_NOW);

    if (handle == nullptr) {
        // 处理打开库失败的情况
        return env->NewStringUTF("Failed to open library");
    }

    DumpJavaStackFunc originFunc = reinterpret_cast<DumpJavaStackFunc>(mydlsym(handle, DUMP_JAVA_STACK_SIG));
    if (originFunc == nullptr) {
        log_info("这里为空", 1);
        return env->NewStringUTF("定位DumpJavaStack失败");
    }
    std::ostringstream oss;
    struct ctx *ctx = (struct ctx *) handle;
    oss <<"libart.so加载在位置:"<< ctx->load_addr<<"\r\n";
    oss <<"当前线程堆栈:\r\n";
    // 调用原始函数，将输出写入 buffer
    originFunc(reinterpret_cast<void *>(self), oss, true, false);

    return env->NewStringUTF(oss.str().c_str());
}

static JNINativeMethod dexposedMethods[] = {
        {"stringFromJNI",              "(J)Ljava/lang/String;",                          (void *) native_stringFromJNI},
};

static int registerNativeMethods(JNIEnv *env, const char *className,
                                 JNINativeMethod *gMethods, int numMethods) {
    jclass clazz = env->FindClass(className);
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    if (!registerNativeMethods(env, JNIHOOK_CLASS, dexposedMethods,
                               sizeof(dexposedMethods) / sizeof(dexposedMethods[0]))) {
        return -1;
    }
    return JNI_VERSION_1_6;
}
