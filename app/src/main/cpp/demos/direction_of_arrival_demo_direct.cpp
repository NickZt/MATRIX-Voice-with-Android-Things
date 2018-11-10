/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */

#include <stdint.h>
#include <fstream>
#include <valarray>
#include <android/log.h>

#include "../driver/bus_direct.h"
#include "../driver/matrixio_bus.h"
#include "../driver/everloop.h"
#include "../driver/microphone_array.h"
#include "../driver/microphone_core.h"
#include "../driver/direction_of_arrival.h"
#include "direction_of_arrival_demo_direct.h"

//DEFINE_bool(big_menu, true, "Include 'advanced' options in the menu listing");
//DEFINE_int32(sampling_frequency, 16000, "Sampling Frequency");

namespace hal = matrix_hal;

int led_offset[] = {23, 27, 32, 1, 6, 10, 14, 19};
int lut[] = {1, 2, 10, 200, 10, 2, 1};

int
hal::DirectionOfArrivalDemo::direction_of_arrivalmain(hal::MatrixIOBus bus, hal::Everloop everloop,
                                                      hal::MicrophoneArray mics,
                                                      int sampling_rate) {


   // sampling_rate = 16000;


    everloop.Setup(&bus);

    hal::EverloopImage image1d(bus.MatrixLeds());


    hal::MicrophoneCore mic_core(mics);
    mic_core.Setup(&bus);

    hal::DirectionOfArrival doa(mics);

    doa.Init();
    __android_log_print(ANDROID_LOG_DEBUG, "TODEL ",
                        "doa.Init(); ok of_arrivalmain mics_.NumberOfSamples(%d)",mics.NumberOfSamples() );
    float azimutal_angle;
    float polar_angle;
    int mic;

    while (true) {
        mics.Read(); /* Reading 8-mics buffer from de FPGA */

        doa.Calculate();

        azimutal_angle = doa.getAzimutalAngle() * 180 / M_PI;
        polar_angle = doa.getPolarAngle() * 180 / M_PI;
        mic = doa.getNearestMicrophone();

        __android_log_print(ANDROID_LOG_DEBUG, "TODEL ",
                            "azimutal angle:->| %f|, polar angle = %f, mic =%d",
                            azimutal_angle, polar_angle, mic);


        for (hal::LedValue &led : image1d.leds) {
            led.blue = 0;
        }

        for (int i = led_offset[mic] - 1, j = 0; i < led_offset[mic] + 1;
             ++i, ++j) {
            if (i < 0) {
                image1d.leds[image1d.leds.size() + i].blue = lut[j];
            } else {
                image1d.leds[i % image1d.leds.size()].blue = lut[j];
            }

            everloop.Write(&image1d);
        }
    }
    return 0;
}
