#include "BookmarksWindowController.hpp"
#include "InputWindowController.hpp"
#include "QuestionWindowController.hpp"
#include <ncurses.h>
#include <algorithm>

BookmarksWindowController::BookmarksWindowController(IAppContext &context, BookmarksWindow& window)
    : context_(context)
    , window_(window)
{
}

bool BookmarksWindowController::parseKey(const int key, Keyboard& keyboard)
{
    switch (key)
    {
    case KEY_UP:
        window_.up();
        break;
    case KEY_DOWN:
        window_.down();
        break;
    case 'r':
        rename();
        break;
    case 'd':
        remove();
        break;
    }
    return true;
}

void BookmarksWindowController::rename()
{
    auto selected = window_.selected();
    if (selected == window_.items().end()) {
        return;
    }
    
    InputWindow inputWindow{
        context_.display(),
        "New name:",
        selected->name,
        BookmarksWindow::NameWidth,
        BookmarksWindow::NameWidth+4};
    InputWindowController controller{inputWindow};
    context_.keyboard().parseKeys(controller);
}

void BookmarksWindowController::remove()
{
    auto selected = window_.selected();
    if (selected == window_.items().end()) {
        return;
    }
    
    QuestionWindow question{
        context_.display(),
            "Are you sure that you want to remove:\n* " + selected->name+ " *",
            {"Yes","Cancel"}};
    QuestionWindowController controller{question};
    context_.keyboard().parseKeys(controller);
}
