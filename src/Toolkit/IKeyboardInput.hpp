#pragma once

#include "Keyboard.hpp"

class IKeyboardInput
{
public:
    virtual bool parseKey(const int key, Keyboard& keyboard) = 0;
    virtual ~IKeyboardInput() = default;
};
