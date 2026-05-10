#include "QuestionWindowController.hpp"
#include <ncurses.h>

QuestionWindowController::QuestionWindowController(QuestionWindow &qwin)
    : qwin_(qwin)
{
}

bool QuestionWindowController::parseKey(const int key, Keyboard& keyboard)
{
    switch (key)
    {
    case KEY_LEFT:   qwin_.left(); break;
    case KEY_RIGHT:  qwin_.right(); break;
    case KEY_ACCEPT: accept_ = true; return false;
    case KEY_ESCAPE: accept_ = false; return false;
    }
    return true;
}

bool QuestionWindowController::accepted() const
{
    return accept_;
}
