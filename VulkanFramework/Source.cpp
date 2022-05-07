#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include "first_app.h"

int main() {

    First_App app;

    try
    {
        app.Run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}