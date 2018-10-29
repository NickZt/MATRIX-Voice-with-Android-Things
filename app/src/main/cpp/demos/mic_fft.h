//
// Created by n.maletskiy on 26.10.2018.
//

#include "../driver/matrixio_bus.h"
#include "../driver/everloop.h"

#ifndef THINGMATRIX_MIC_FFT_H
#define THINGMATRIX_MIC_FFT_H

#endif //THINGMATRIX_MIC_FFT_H
namespace matrix_hal {
    class MicFFTKissDemo {
    public:


        static void
        mic_fft_loop(MatrixIOBus bus, Everloop everloop, MicrophoneArray mics, int sampling_rate);
    };
}