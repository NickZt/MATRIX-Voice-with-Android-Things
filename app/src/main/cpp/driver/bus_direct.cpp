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

#include "bus_direct.h"
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
#include <android/log.h>
#include <pio/peripheral_manager_client.h>
#include "creator_memory_map.h"

#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
#define ASSERT(cond, ...) if (!(cond)) { __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__); return false;}


namespace matrix_hal {

    struct hardware_address {
        uint8_t readnwrite : 1;
        uint16_t reg : 15;
    };

    BusDirect::BusDirect()
            : spi_fd_(-1), //zero norm not zero not norm
              spi_mode_(ASPI_MODE3),
              spi_bits_(8),
              spi_speed_(15000000),
              spi_delay_(0) {
        LOGV("TODEL OK BusDirect::BusDirect() ");
    }

    BusDirect::~BusDirect() {
        if (!spi_fd_) Close();
    }

    bool BusDirect::Init(char *device_name) {

        if (nullptr == BusDirect::getASpiDevice()) Close();

        if (device_name == nullptr)
            device_name_ = Spi_BUS;
        else
            device_name_ = device_name;
        LOGV("TODEL OK BusDirect::Init" );
        APeripheralManagerClient *client = APeripheralManagerClient_new();

        //  ASSERT(client, "failed to open peripheral manager client");
        int tmp = 0;
        char **tmplist = APeripheralManagerClient_listSpiBuses(client, &tmp);
        LOGV("TODEL OK open peripheral manager client %d", tmp);
        for (int i = 0; i < tmp; ++i) {
            LOGV("TODEL OK found peripheral manager Bus %s", tmplist[i]);
        }


        ASpiDevice *aSpiDevice;

        spi_fd_ = APeripheralManagerClient_openSpiDevice(client, Spi_BUS, &aSpiDevice);//device_name_

        ASSERT(spi_fd_ == 0, "failed to open SpiBUS: %s  %d ", device_name_, spi_fd_);

        LOGV(" OK open openResult %d", spi_fd_);

        spi_fd_ = ASpiDevice_setMode(aSpiDevice, spi_mode_);
        ASSERT(spi_fd_ == 0, "failed to setMode ASPI_MODE3 SpiBUS: %s", device_name_);


        spi_fd_ = ASpiDevice_setBitsPerWord(aSpiDevice, spi_bits_);
        ASSERT(spi_fd_ == 0, "failed to BitsPerWord SpiBUS: %s  spi_bits_: %d", device_name_,
               spi_bits_);

        spi_fd_ = ASpiDevice_setFrequency(aSpiDevice, spi_speed_);//18000000
        ASSERT(spi_fd_ == 0, "failed to setFrequency SpiBUS: %s  Frequency: %d", device_name_,
               spi_speed_);
        LOGV(" OK setFrequency SpiBUS: %s  Frequency: %d", device_name_, spi_speed_);

        spi_fd_ = ASpiDevice_setDelay(aSpiDevice, spi_delay_);
        ASSERT(spi_fd_ == 0, "failed to setDelay SpiBUS: %s  Delay: %d", device_name_,
               spi_delay_);
        LOGV(" OK set setDelay SpiBUS: %s  Delay: %d", device_name_,
             spi_delay_);

        //ASpiDevice_delete(aSpiDevice);
        BusDirect::setASpiDevice(aSpiDevice);
        LOGV("TODEL  OK set  BusDirect::setASpiDevice(aSpiDevice); ");
        //   std::unique_lock<std::mutex> lock(mutex_);
        return true;
    }

    bool BusDirect::SpiTransfer(unsigned char *send_buffer,
                                unsigned char *receive_buffer, unsigned int size) {

        ASSERT(ASpiDevice_transfer(BusDirect::getASpiDevice(), send_buffer, receive_buffer, size) ==
               0, "failed to transfer SpiBUS: %s", device_name_);


        return true;
    }

    bool BusDirect::Read(uint16_t add, unsigned char *data, int length) {
//        std::unique_lock<std::mutex> lock(mutex_);

        hardware_address *hw_addr = reinterpret_cast<hardware_address *>(tx_buffer_);
        hw_addr->reg = add;
        hw_addr->readnwrite = 1;

        if (SpiTransfer(tx_buffer_, rx_buffer_, length + 2)) {
            memcpy(data, &rx_buffer_[2], length);
            return true;
        }
        return false;
    }

    bool BusDirect::Write(uint16_t add, unsigned char *data, int length) {
//        std::unique_lock<std::mutex> lock(mutex_);

        hardware_address *hw_addr = reinterpret_cast<hardware_address *>(tx_buffer_);
        hw_addr->reg = add;
        hw_addr->readnwrite = 0;

        memcpy(&tx_buffer_[2], data, length);
        if (SpiTransfer(tx_buffer_, rx_buffer_, length + 2)) {
            return true;
        }
        return false;
    }

    void BusDirect::Close(void) { close(spi_fd_); }
};  // namespace matrix_hal
