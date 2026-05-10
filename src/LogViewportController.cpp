#include "LogViewportController.hpp"
#include <ncurses.h>


LogViewportController::LogViewportController(LogViewport& window)
    : window_(window)
{
}

bool LogViewportController::parseKey(const int key, Keyboard& keyboard)
{
    switch (key)
    {
    case KEY_UP: window_.up(); break;
    case KEY_DOWN: window_.down(); break;
    case KEY_PPAGE: window_.pageUp(); break;
    case KEY_NPAGE: window_.pageDown(); break;
    case KEY_HOME: window_.top(); break;
    case KEY_END: window_.bottom(); break;

    }
    return true;
}
