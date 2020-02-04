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

    Display *mainDisplay = nullptr;
    int returnValue = EXIT_SUCCESS;
    try
    {
        std::cout << "Loading logs..." << std::endl;
        LogBuffer log{args.files()};
        LogBufferView view{log};

        Display display;
        display.start();
        mainDisplay = &display;

        Keyboard keyboard{display};
        keyboard.start();

        MainController mainController{display, log, keyboard};
        keyboard.parseKeys(mainController);

    } catch (const std::exception& e) {
        printException(e);
        returnValue = EXIT_FAILURE;
    }

    if (mainDisplay)
        mainDisplay->stop();

    return returnValue;
}
