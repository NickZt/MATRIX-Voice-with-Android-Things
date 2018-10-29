#include <jni.h>
#include <string>
#include "demos/everloop_demo.h"
#include "driver/everloop_image.h"
#include "driver/matrixio_bus.h"
#include "driver/everloop.h"
#include "driver/microphone_array.h"
#include "demos/mic_fft.h"
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
        }
        case 1: {
            hal::MicFFTKissDemo::mic_fft_loop(bus, everloop, mics,
                                              sampling_rate);
            break;
        }
        default: { ;
        }


    }
}