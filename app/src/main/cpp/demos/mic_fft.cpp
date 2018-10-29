/*
 * Copyright 2017 <Admobilize>
 * MATRIX Labs  [http://creator.matrix.one]
 * This file is part of MATRIX Voice HAL for ESP32
 *
 * Author:
 *       Andrés Calderón <andres.calderon@admobilize.com>
 *
 * MATRIX Voice ESP32 HAL is free software: you can redistribute it
 * and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <cmath>


#include <cstring>

#include "../driver/bus.h"
#include "../driver/everloop.h"
#include "../driver/microphone_array.h"
#include "../driver/microphone_core.h"
#include "../fft/kiss_fft.h"
#include "../fft/qmath.h"
#include "mic_fft.h"

namespace hal = matrix_hal;

#define FFT_ORDER 128

void hal::MicFFTKissDemo::mic_fft_loop(matrix_hal::MatrixIOBus wb, matrix_hal::Everloop everloop,
                                       hal::MicrophoneArray mics, int sampling_rate) {

    // Microphone Core Init
    hal::MicrophoneCore mic_core(mics);
    mic_core.Setup(&wb);

    hal::EverloopImage image1d;

    kiss_fft_cpx cx_in[FFT_ORDER];
    kiss_fft_cpx cx_out[FFT_ORDER];
    kiss_fft_cfg cfg = kiss_fft_alloc(FFT_ORDER, 0, 0, 0);

    while (1) {
        mics.Read();

        for (uint32_t s = 0; s < FFT_ORDER; s++) {
            cx_in[s].r = float2q(fabs(mics.At(s, 0)) / 1024.0);
            cx_in[s].i = 0;
        }

        kiss_fft(cfg, cx_in, cx_out);

        for (uint32_t i = 0; i < image1d.leds.size(); i++) {
            float z = q2double(qmul((cx_out[i + 1].r), (cx_out[i + 1].r)) +
                               qmul((cx_out[i + 1].i), (cx_out[i + 1].i))) *
                      256;
            if (i < 6) {
                image1d.leds[i].Set(0, 0, z, 0);
            } else if (i < 12) {
                image1d.leds[i].Set(0, z, 0, 0);
            } else {
                image1d.leds[i].Set(z, 0, 0, 0);
            }
        }
        everloop.Write(&image1d);
    }
}
