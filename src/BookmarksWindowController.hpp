#pragma once

#include "BookmarksWindow.hpp"
#include "Toolkit/IKeyboardInput.hpp"
#include <memory>

class BookmarksWindowController :  public IKeyboardInput
{
public:
    BookmarksWindowController(IAppContext &context, BookmarksWindow& window);

    bool parseKey(const int key, Keyboard &keyboard) override;

private:
    IAppContext &context_;
    BookmarksWindow& window_;
    void createView();
    void rename();
    void remove();
};
