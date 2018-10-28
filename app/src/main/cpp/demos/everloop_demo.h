

#include "../driver/matrixio_bus.h"
#include "../driver/everloop.h"
namespace matrix_hal {
    class EverloopDemo {
    public:

        static void setIseverloopRun(bool iseverloop);

        static bool everloop_demo_main(MatrixIOBus bus, Everloop everloop);
    };
}