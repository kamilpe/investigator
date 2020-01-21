#pragma once

#include "LogViewport.hpp"
#include "Toolkit/IKeyboardInput.hpp"

class LogViewportController : public IKeyboardInput
{
public:
    LogViewportController(LogViewport&);

    bool parseKey(const int key, Keyboard& keyboard) override;

private:
    LogViewport &window_;
};
