#pragma once

#include "ListWindowBase.hpp"
#include "IAppContext.hpp"
#include "Toolkit/IKeyboardInput.hpp"
#include <memory>

class BookmarksWindowController :  public IKeyboardInput
{
public:
    BookmarksWindowController(IAppContext &context);

    bool parseKey(const int key, Keyboard &keyboard) override;
    ListWindowBase<std::string>& window();

private:
    IAppContext &context_;
    std::unique_ptr<ListWindowBase<std::string>> listView_;
    std::unique_ptr<ListWindowBase<std::string>> createView() const;
};
