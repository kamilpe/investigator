#pragma once

#include "Display.hpp"

constexpr auto KEY_ACCEPT = 10;
constexpr auto KEY_ESCAPE = 27;

class IKeyboardInput;

class Keyboard
{
public:
    Keyboard(Display& display);

    void start();
    void parseKeys(IKeyboardInput& focused);

private:
    Display& display_;

    int getKey();
};
