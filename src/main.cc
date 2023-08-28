#include "shift_application.h"

int main(int argc, char* argv[]) {
    auto app = ShiftApplication::create(argc, argv);

    return app->run();
}
