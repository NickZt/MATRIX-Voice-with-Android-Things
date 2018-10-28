#include <jni.h>
#include <string>
#include "demos/everloop_demo.h"
#include "driver/everloop_image.h"
#include "driver/matrixio_bus.h"
#include "driver/everloop.h"
#include "driver/microphone_array.h"
#include <pio/peripheral_manager_client.h>
#include <android/log.h>

const char *TAG = "TODEL ";

//todo migrate to one bus class



namespace hal = matrix_hal;
//matrix_hal vars
hal::MatrixIOBus bus;
hal::Everloop everloop;
hal::MicrophoneArray mics;


//JNA vars
jobject store_listener;
static JavaVM *jvm = NULL;
jweak store_Wlistener;
jmethodID store_method = NULL;
jmethodID store_methodVAL = NULL;

JNIEnv *store_env;


int sampling_rate;
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
#define ASSERT(cond, ...) if (!(cond)) { __android_log_assert(#cond, TAG, __VA_ARGS__);}


extern "C"
JNIEXPORT void JNICALL
Java_ua_zt_mezon_thingstest_thingmatrix_MainActivity_nsubscribeListener(JNIEnv *env,
                                                                        jobject instance,
                                                                        jobject listener) {
    env->GetJavaVM(&jvm); //store jvm reference for later
    store_env = env;
    //ref to WEAKREF
    store_Wlistener = env->NewWeakGlobalRef(listener);

    jclass clazz = env->GetObjectClass(store_Wlistener);

    store_method = env->GetMethodID(clazz, "onMessage", "(Ljava/lang/String;)V");
    store_methodVAL = env->GetMethodID(clazz, "onEngineStateChanged", "(I)V");

    __android_log_print(ANDROID_LOG_VERBOSE, "GetEnv:", " Subscribe to Listener  OK \n");

    if (NULL == store_method) return;
}

extern "C"
JNIEXPORT void JNICALL
Java_ua_zt_mezon_thingstest_thingmatrix_MainActivity_ndismissListener(JNIEnv *env,
                                                                      jobject instance) {
    env->DeleteWeakGlobalRef(store_Wlistener);
    store_method = NULL;
}


void ss_string_callback(char *val) {
    JNIEnv *g_env;
    if (NULL == jvm) {
        __android_log_print(ANDROID_LOG_ERROR, "GetEnv:", " No VM  \n");
        return;
    }
    //  double check it's all ok

    JavaVMAttachArgs args;
    args.version = JNI_VERSION_1_6; // choose your JNI version
    args.name = NULL; // you might want to give the java thread a name
    args.group = NULL; // you might want to assign the java thread to a ThreadGroup

    //int getEnvStat=  jvm->AttachCurrentThread(&g_env, &args);

    int getEnvStat = jvm->GetEnv((void **) &g_env, JNI_VERSION_1_6);

    if (getEnvStat == JNI_EDETACHED) {
        __android_log_print(ANDROID_LOG_ERROR, "GetEnv:", " not attached\n");
        if (jvm->AttachCurrentThread(&g_env, &args) != 0) {
            __android_log_print(ANDROID_LOG_ERROR, "GetEnv:", "Failed to attach\n");
        }
    } else if (getEnvStat == JNI_OK) {
        //ee
    } else if (getEnvStat == JNI_EVERSION) {
        __android_log_print(ANDROID_LOG_ERROR, "GetEnv:", " version not supported\n");
    }

    jstring message = g_env->NewStringUTF(val);//+ val
    g_env->CallVoidMethod(store_Wlistener, store_method, message);


    if (g_env->ExceptionCheck()) {
        g_env->ExceptionDescribe();
    }

    if (getEnvStat == JNI_EDETACHED) {
        jvm->DetachCurrentThread();
    }
}

void ss_int_callback(int val) {
    JNIEnv *g_env;
    if (NULL == jvm) {
        __android_log_print(ANDROID_LOG_ERROR, "GetEnv:", " No VM  \n");
        return;
    }
    //  double check it's all ok

    JavaVMAttachArgs args;
    args.version = JNI_VERSION_1_6; // choose your JNI version
    args.name = NULL; // you might want to give the java thread a name
    args.group = NULL; // you might want to assign the java thread to a ThreadGroup

    //int getEnvStat=  jvm->AttachCurrentThread(&g_env, &args);

    int getEnvStat = jvm->GetEnv((void **) &g_env, JNI_VERSION_1_6);

    if (getEnvStat == JNI_EDETACHED) {
        __android_log_print(ANDROID_LOG_ERROR, "GetEnv:", " not attached i c \n");
        if (jvm->AttachCurrentThread(&g_env, &args) != 0) {
            __android_log_print(ANDROID_LOG_ERROR, "GetEnv:", "Failed to attach\n");
        }
    } else if (getEnvStat == JNI_OK) {
        //ee
    } else if (getEnvStat == JNI_EVERSION) {
        __android_log_print(ANDROID_LOG_ERROR, "GetEnv:", " version not supported\n");
    }

    // jstring message = g_env->NewStringUTF("Hello from C, code RED");//+ val
    // g_env->CallVoidMethod(store_Wlistener, store_method, message);

    g_env->CallVoidMethod(store_Wlistener, store_methodVAL, val);

    if (g_env->ExceptionCheck()) {
        g_env->ExceptionDescribe();
    }

    if (getEnvStat == JNI_EDETACHED) {
        jvm->DetachCurrentThread();
    }
}


extern "C"
JNIEXPORT jobjectArray JNICALL
Java_ua_zt_mezon_thingstest_thingmatrix_MainActivity_returnfftd(JNIEnv *env, jobject instance,
                                                                jobjectArray arr) {

    //if (magnitudeLeft != NULL) {
    int rows = env->GetArrayLength(arr);
    //   jobjectArray arrayRow = (jobjectArray) (env->GetObjectArrayElement(arr, 0));
    jfloatArray arrayRow = (jfloatArray) (env->GetObjectArrayElement(arr, 0));
    int columns = env->GetArrayLength(arrayRow);


//        __android_log_print(ANDROID_LOG_DEBUG, TAG, "array arrived, phase left(): %i, %i ", rows,
//                            columns);

    //while (frequencyPlayDomain->timeDomainToFrequencyDomain(magnitudeLeft, magnitudeRight, phaseLeft, phaseRight))


    const jfloat *magnitudeLeft = NULL;
    env->SetFloatArrayRegion(arrayRow, 0, columns, magnitudeLeft);

    //env->SetObjectArrayElement(arr, 0, (jobject) magnitudeLeft);

    arrayRow = (jfloatArray) (env->GetObjectArrayElement(arr, 1));
    const jfloat *magnitudeRight = NULL;
    env->SetFloatArrayRegion(arrayRow, 0, columns, magnitudeRight);


    arrayRow = (jfloatArray) (env->GetObjectArrayElement(arr, 2));
    const jfloat *phaseLeft = NULL;
    env->SetFloatArrayRegion(arrayRow, 0, columns, phaseLeft);

    arrayRow = (jfloatArray) (env->GetObjectArrayElement(arr, 3));
    const jfloat *phaseRight = NULL;
    env->SetFloatArrayRegion(arrayRow, 0, columns, phaseRight);

    arrayRow = (jfloatArray) (env->GetObjectArrayElement(arr, 4));
    const jfloat *stereoInputBufferFloat = nullptr;
    env->SetFloatArrayRegion(arrayRow, 0, columns, stereoInputBufferFloat);


//
//    for(int i = 0; i < rows; i++)
//    {
//        jfloatArray  currentRow = (jfloatArray)((env->GetObjectArrayElement(arr, i)));
//        for(int j = 0; j < columns; j++)
//        {
//         currentRow[j]=
//            //   (env->SetFloatArrayElements(currentRow, j));
//
//
//            // do stuff...
//        }
//    }

//    }
    return arr;
}


extern "C"
JNIEXPORT void JNICALL
Java_ua_zt_mezon_thingstest_thingmatrix_MainActivity_neffectEnable(JNIEnv *env, jobject instance,
                                                                   jboolean enable) {

    // TODO

}

extern "C"
JNIEXPORT void JNICALL
Java_ua_zt_mezon_thingstest_thingmatrix_MainActivity_nfftComplex(JNIEnv *env, jobject instance,
                                                                 jboolean cplx) {

    // TODO

}

extern "C"
JNIEXPORT void JNICALL
Java_ua_zt_mezon_thingstest_thingmatrix_MainActivity_ntestcallListener(JNIEnv *env,
                                                                       jobject obj) {

    jclass clazz = env->GetObjectClass(store_Wlistener);

    jmethodID method = env->GetMethodID(clazz, "onMessage", "(Ljava/lang/String;)V");
    if (NULL == method) return;
    jstring string = env->NewStringUTF("Hello");

    env->CallVoidMethod(store_listener, method, string);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_ua_zt_mezon_thingstest_thingmatrix_MainActivity_startEverloopN(JNIEnv *env, jobject instance,
                                                                    jboolean isEverloopStart) {
    hal::EverloopDemo::setIseverloopRun(isEverloopStart);
    if (isEverloopStart) {
        if (hal::EverloopDemo::everloop_demo_main(bus, everloop)) {
            std::string hello = "Ever Started";
            return env->NewStringUTF(hello.c_str());
        } else {
            std::string hello = "BUS ERROR";
            return env->NewStringUTF(hello.c_str());
        };
    } else {
        std::string hello = "Ever End";
        return env->NewStringUTF(hello.c_str());
    }

}

extern "C"
JNIEXPORT jboolean JNICALL
Java_ua_zt_mezon_thingstest_thingmatrix_MainActivity_matrixInit(JNIEnv *env, jobject instance,
                                                                jint samplingFreq) {


    if (!bus.Init()) {
        __android_log_print(ANDROID_LOG_DEBUG, "TODEL",
                            "BUS NOT INIT");
        return false;
    } else {
        __android_log_print(ANDROID_LOG_DEBUG, "TODEL",
                            "OK BUS  INIT");
    }
    sampling_rate = samplingFreq;


    everloop.Setup(&bus);
    __android_log_print(ANDROID_LOG_DEBUG, "TODEL",
                        " everloop.Setup(&bus);");


    mics.Setup(&bus);
    mics.SetSamplingRate(sampling_rate);

    mics.ShowConfiguration();


    return true;

}


extern "C"
JNIEXPORT void JNICALL
Java_ua_zt_mezon_thingstest_thingmatrix_MainActivity_neffectSelector(JNIEnv *env, jobject instance,
                                                                     jint eff_type, jint progress,
                                                                     jboolean isDemoloopStart,
                                                                     jint micsamplingFreq) {
    switch (eff_type) {
        case 0: {
            hal::EverloopDemo::setIseverloopRun(isDemoloopStart);
            if (isDemoloopStart) {
                hal::EverloopDemo::everloop_demo_main(bus, everloop);
                break;
            }

            default: {
                /*группа операторов*/;
            }
        }


    }
}