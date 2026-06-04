#include "ui.h"
#include <iostream>

int main() {
    try {
        UserInterface ui;
        ui.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
