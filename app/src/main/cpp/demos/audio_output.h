//
// Created by NickZt on 30.10.2018.
//

#ifndef THINGSMATRIXCLEARGIT_AUDIO_OUTPUT_H
#define THINGSMATRIXCLEARGIT_AUDIO_OUTPUT_H

#endif //THINGSMATRIXCLEARGIT_AUDIO_OUTPUT_H

#include "../driver/matrixio_bus.h"
#include "../driver/everloop.h"

namespace matrix_hal {
    class AudioOutputDemo {
    public:

        static void setIsaudiooutputRun(bool iseverloop);

        static int audio_output_main(int argc, const char *agrv, MatrixIOBus bus, Everloop everloop);
    };
}