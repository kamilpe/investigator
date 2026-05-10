#include "BookmarksWindowController.hpp"
#include "InputWindowController.hpp"
#include <ncurses.h>

BookmarksWindowController::BookmarksWindowController(IAppContext &context)
    : context_(context)
{
    createView();
}

bool BookmarksWindowController::parseKey(const int key, Keyboard& keyboard)
{
    switch (key)
    {
    case KEY_UP:
        listWindow_->up();
        break;
    case KEY_DOWN:
        listWindow_->down();
        break;
    }
    return true;
}

void BookmarksWindowController::setFocus(bool focus)
{
    listWindow_->setFocus(focus);
}

void BookmarksWindowController::createView()
{
    listWindow_ = std::make_unique<BookmarksWindow>(
        context_, context_.bookmarks(), context_.bookmarks().begin());
}
