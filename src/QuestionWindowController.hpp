#pragma once

#include "CursesToolkit/IKeyboardInput.hpp"
#include "QuestionWindow.hpp"

class QuestionWindowController :  public IKeyboardInput
{
public:
    QuestionWindowController(QuestionWindow &qwin);

    bool parseKey(const int key, Keyboard &keyboard) override;
    bool accepted() const;

private:
    QuestionWindow &qwin_;
    bool accept_;


};
