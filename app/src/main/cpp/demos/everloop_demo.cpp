/*
 * Copyright 2016 <Admobilize>
 * MATRIX Labs  [http://creator.matrix.one]
 * This file is part of MATRIX Creator HAL
 *
 * MATRIX Creator HAL is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <unistd.h>
#include <iostream>
#include "../driver/bus.h"
#include "../driver/matrixio_bus.h"
#include "../driver/everloop_image.h"
#include "../driver/everloop.h"
#include "everloop_demo.h"

#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
#define ASSERT(cond, ...) if (!(cond)) { __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__); return false;}


namespace hal = matrix_hal;
bool iseverloopRun;


bool hal::EverloopDemo::everloop_demo_main(hal::MatrixIOBus bus, hal::Everloop everloop) {

    unsigned counter = 0;
    hal::EverloopImage image1d(bus.MatrixLeds());

    while (iseverloopRun) {
        for (hal::LedValue &led : image1d.leds) {
            led.red = 0;
            led.green = 0;
            led.blue = 0;
            led.white = 0;
        }
        image1d.leds[(counter / 2) % image1d.leds.size()].red = 20;
        image1d.leds[(counter / 7) % image1d.leds.size()].green = 30;
        image1d.leds[(counter / 11) % image1d.leds.size()].blue = 30;
        image1d.leds[image1d.leds.size() - 1 - (counter % image1d.leds.size())]
                .white = 10;

        everloop.Write(&image1d);
//        __android_log_print(ANDROID_LOG_DEBUG, "TODEL",
//                        "counter:->| %d|",
//                            counter);
        ++counter;
        usleep(20000);
    }

    return true;
}

void hal::EverloopDemo::setIseverloopRun(bool iseverloop) {
    iseverloopRun = iseverloop;
}

