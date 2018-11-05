

#include "../driver/matrixio_bus.h"
#include "../driver/everloop.h"
#include "../driver/microphone_array.h"
namespace matrix_hal {
    class DirectionOfArrivalDemo {
    public:

        //  static void setIseverloopRun(bool iseverloop);

        static int direction_of_arrivalmain(MatrixIOBus bus, Everloop everloop, MicrophoneArray mics,int sampling_rate);
    };
}