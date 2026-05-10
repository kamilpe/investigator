#include "ArgParser.hpp"
#include "Logs/LogBuffer.hpp"
#include "Logs/LogBufferView.hpp"
#include "MainController.hpp"
#include "Toolkit/Display.hpp"
#include "Toolkit/Keyboard.hpp"
#include <iostream>

void printException(const std::exception& e)
{
    std::cout << "Error: " << e.what();
}

int main(int argv, char **argc)
{
    ArgParser args{argv, argc};
    if (args.errors())
    {
        std::cout << "Press enter to continue" << std::endl;
        std::cin.get();
    }

    if (args.files().empty())
    {
        std::cout << "No input files" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Loading..." << std::endl;

    LogBuffer log{args.files()};
    if (!log.initialized())
    {
        std::cout << "Unable to read input files" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Complete" << std::endl;

    LogBufferView view{log};
    Display display;
    try
    {
        display.start();
    }
    catch (const std::exception& e)
    {
        printException(e);
        return EXIT_FAILURE;
    }

    Keyboard keyboard{display};
    try
    {
        keyboard.start();
    }
    catch (const std::exception& e)
    {
        printException(e);
        return EXIT_FAILURE;
    }

    try
    {
        MainController mainController{display, log, keyboard};
        keyboard.parseKeys(mainController);
    }
    catch (const std::exception& e)
    {
        // TODO: report also graphical error
        std::cout << "Error: " << e.what();
    }
    display.stop();
    return EXIT_SUCCESS;
}
