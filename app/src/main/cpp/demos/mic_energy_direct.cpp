/*
 * Copyright 2016 <Admobilize>
 * All rights reserved.
 */
#include <unistd.h>
#include <iostream>
#include <string>
#include <valarray>

#include "../driver/bus_direct.h"
#include "../driver/matrixio_bus.h"
#include "../driver/everloop.h"
#include "../driver/direction_of_arrival.h"
#include "../driver/everloop_image.h"
#include "../driver/microphone_array.h"
#include "../driver/microphone_core.h"
#include "mic_energy_direct.h"

//DEFINE_bool(big_menu, true, "Include 'advanced' options in the menu listing");
//DEFINE_int32(sampling_frequency, 16000, "Sampling Frequency");

namespace hal = matrix_hal;

int hal::MicEnergyDemo::mic_energy_direct_main(hal::MatrixIOBus bus, hal::Everloop everloop,
                                               hal::MicrophoneArray mics,
                                               int sampling_rate) {




    // Microphone Core Init
    hal::MicrophoneCore mic_core(mics);
    mic_core.Setup(&bus);

    hal::EverloopImage image1d(bus.MatrixLeds());
    int j = 0;
    uint64_t instantE = 0;
    uint64_t avgEnergy = 0;
    std::valarray<uint64_t> localAverage(20);
    localAverage = 0;

    while (true) {
        mics.Read(); /* Reading 8-mics buffer from de FPGA */
        instantE = 0;
        for (uint32_t s = 0; s < mics.NumberOfSamples(); s++) {
            instantE = instantE + (mics.At(s, 0)) * (mics.At(s, 0));
        }

        localAverage[j % 20] = instantE;
        avgEnergy = 0;
        for (auto &data : localAverage) {
            avgEnergy = (avgEnergy + data);
        }

        avgEnergy = avgEnergy / 20;

        for (auto &led : image1d.leds) {
            led.red = avgEnergy >> 24;
        }
        everloop.Write(&image1d);

        j++;
    }

    return 0;
}
