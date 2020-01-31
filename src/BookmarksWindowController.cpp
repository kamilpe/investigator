#include "BookmarksWindowController.hpp"
#include "InputWindowController.hpp"
#include <ncurses.h>
#include <algorithm>

BookmarksWindowController::BookmarksWindowController(BookmarksWindow& window)
    : listWindow_(window)
{
}

bool BookmarksWindowController::parseKey(const int key, Keyboard& keyboard)
{
    switch (key)
    {
    case KEY_UP:
        listWindow_.up();
        break;
    case KEY_DOWN:
        listWindow_.down();
        break;
    }
    return true;
}

void BookmarksWindowController::selectClosest(int id) // TODO: move to the Window, change to goTo(findClosest)
{
    const auto predicate = [id](const Bookmark& bookmark) {
        return bookmark.id <= id;
    };
    
    const auto& items = listWindow_.items();
    auto it = std::find_if(items.crbegin(), items.crend(), predicate);
    if (it == items.crend()) {
        listWindow_.select(items.cbegin());
        return;
    }
    listWindow_.select(it.base() - 1);
}
