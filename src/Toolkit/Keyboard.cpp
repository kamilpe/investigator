#include "Keyboard.hpp"
#include "IKeyboardInput.hpp"
#include <ncurses.h>

Keyboard::Keyboard(Display& display)
    : display_(display)
{
}

void Keyboard::start()
{
    cbreak();
    nl();
    noecho();
    keypad(stdscr, TRUE);
    ESCDELAY = 0;
}


void Keyboard::parseKeys(IKeyboardInput& focused)
{
    bool running = true;
    int key;
    do
    {
        display_.refresh();
        key = getKey();
        switch (key)
        {
        case KEY_RESIZE:
            display_.resize();
            break;
        default:
            running = focused.parseKey(key, *this);
            break;
        }
    } while (running);
}

int Keyboard::getKey()
{
    return getch();
}
