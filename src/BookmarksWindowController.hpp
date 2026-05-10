#pragma once

#include "BookmarksWindow.hpp"
#include "Toolkit/IKeyboardInput.hpp"
#include <memory>

class BookmarksWindowController :  public IKeyboardInput
{
public:
    BookmarksWindowController(IAppContext &context);

    bool parseKey(const int key, Keyboard &keyboard) override;
    void setFocus(bool focus);

private:
    IAppContext &context_;
    std::unique_ptr<BookmarksWindow> listWindow_;
    void createView();
};
