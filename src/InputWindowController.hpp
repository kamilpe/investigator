#pragma once

#include "InputWindow.hpp"
#include "CursesToolkit/IKeyboardInput.hpp"

class InputWindowController :  public IKeyboardInput
{
public:
    InputWindowController(InputWindow& input);
    bool parseKey(const int key, Keyboard& keyboard) override;

    bool accepted() const;
private:
    InputWindow& input_;

    bool accept_;
};
