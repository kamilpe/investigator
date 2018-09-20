#include "InputWindowController.hpp"
#include <ncurses.h>

InputWindowController::InputWindowController(InputWindow& input)
    : input_(input)
{
}

bool InputWindowController::parseKey(const int key, Keyboard& keyboard)
{
    if (key != KEY_ENTER && std::isprint(key))
    {
        input_.put(key);
    }
    else
    {
        switch (key)
        {
        case KEY_LEFT:      input_.left(); break;
        case KEY_RIGHT:     input_.right(); break;
        case KEY_DC:        input_.del(); break;
        case KEY_BACKSPACE: input_.backspace(); break;
        case KEY_ESCAPE:    accept_ = false; return false;
        case KEY_ACCEPT:    accept_ = true; return false;
        }
    }

    return true;
}


bool InputWindowController::accepted() const
{
    if (input_.content().empty())
        return false;
    return accept_;
}
