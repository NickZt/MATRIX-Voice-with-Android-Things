//
// Created by n.maletskiy on 26.10.2018.
//

#include "../driver/matrixio_bus.h"
#include "../driver/everloop.h"

#ifndef THINGMATRIX_MIC_ENERGY_DIRECT_H
#define THINGMATRIX_MIC_ENERGY_DIRECT_H

#endif //THINGMATRIX_MIC_ENERGY_DIRECT_H
namespace matrix_hal {
    class MicEnergyDemo {
    public:

        //  static void setIseverloopRun(bool iseverloop);

        static int mic_energy_direct_main(MatrixIOBus bus, Everloop everloop, MicrophoneArray mics, int sampling_rate);
    };
}