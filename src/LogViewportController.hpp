#pragma once

#include "LogViewportWindow.hpp"
#include "Toolkit/IKeyboardInput.hpp"

class LogViewportController : public IKeyboardInput
{
public:
    LogViewportController(LogViewportWindow&);

    bool parseKey(const int key, Keyboard& keyboard) override;

private:
    LogViewportWindow &window_;
};
