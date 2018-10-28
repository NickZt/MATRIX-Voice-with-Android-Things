/*
 * Copyright 2016-2018 <Admobilize>
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

#include "matrixio_bus.h"
#include <pio/peripheral_manager_client.h>
#include <android/log.h>
#include <errno.h>
#include <fcntl.h>

#include <linux/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include "bus_direct.h"
#include "creator_memory_map.h"


#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
#define ASSERT(cond, ...) if (!(cond)) { __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__); return false;}

namespace matrix_hal {

    struct hardware_address {
        uint8_t readnwrite : 1;
        uint16_t reg : 15;
    };

    MatrixIOBus::MatrixIOBus()
            : fpga_frequency_(18000000),
              matrix_name_(0),
              matrix_leds_(18),
              bus_driver_(NULL),
              direct_nkernel_(false) {}

    bool MatrixIOBus::Init() {
        if (bus_driver_) delete bus_driver_;
        Bus *bus_direct = new BusDirect();
        if (bus_direct->Init()) {
            bus_driver_ = bus_direct;
            direct_nkernel_ = true;
            // delete bus_kernel;
            LOGV(" TODEL OK bus_direct->Init() ");
        }

        Write(12, 13);
        if (!GetMatrixName()) {
            return false;
        }
//
//        if (!GetFPGAFrequency()) {
//            std::cerr << "can't get FPGA frequency" << std::endl;
//            return false;
//        }

        return true;
    }

    bool MatrixIOBus::Write(uint16_t add, unsigned char *data, int length) {
        return bus_driver_->Write(add, data, length);
    }

    bool MatrixIOBus::Read(uint16_t add, unsigned char *data, int length) {
        return bus_driver_->Read(add, data, length);
    }

    bool MatrixIOBus::Write(uint16_t add, uint16_t data) {
        return bus_driver_->Write(add, (unsigned char *) (&data), sizeof(data));
    }

    bool MatrixIOBus::Read(uint16_t add, uint16_t *data) {
        return bus_driver_->Read(add, (unsigned char *) data, sizeof(*data));
    }

    bool MatrixIOBus::GetMatrixName() {
        uint32_t data[2];
        if (!Read(kConfBaseAddress, (unsigned char *) &data, sizeof(data)))
            return false;
        matrix_name_ = data[0];
        if (matrix_name_ == kMatrixCreator)
            matrix_leds_ = kMatrixCreatorNLeds;
        else if (matrix_name_ == kMatrixVoice)
            matrix_leds_ = kMatrixVoiceNLeds;
        else {
            std::cerr << "MATRIX device has not been detected" << std::endl;
            return false;
        }
        matrix_version_ = data[1];
        return true;
    }

    bool MatrixIOBus::GetFPGAFrequency() {
        uint16_t values[2];
        if (!Read(kConfBaseAddress + 4, (unsigned char *) values, sizeof(values)))
            return false;
        fpga_frequency_ = (kFPGAClock * values[1]) / values[0];
        return true;
    }

};  // namespace matrix_hal
