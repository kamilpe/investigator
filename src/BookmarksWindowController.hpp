#pragma once

#include "BookmarksWindow.hpp"
#include "Toolkit/IKeyboardInput.hpp"
#include <memory>

class BookmarksWindowController :  public IKeyboardInput
{
public:
    BookmarksWindowController(BookmarksWindow& window);

    bool parseKey(const int key, Keyboard &keyboard) override;
    void selectClosest(int id);

private:
    BookmarksWindow& listWindow_;
    void createView();
};
