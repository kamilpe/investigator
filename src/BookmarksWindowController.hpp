#pragma once

#include "ListWindow.hpp"
#include "IAppContext.hpp"
#include "Toolkit/IKeyboardInput.hpp"
#include <memory>

class BookmarksWindowController :  public IKeyboardInput
{
public:
    BookmarksWindowController(IAppContext &context);

    bool parseKey(const int key, Keyboard &keyboard) override;
    ListWindow& window();

private:
    IAppContext &context_;
    std::unique_ptr<ListWindow> listView_;
    std::unique_ptr<ListWindow> createView() const;
};
