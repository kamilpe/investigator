#pragma once
#include "PanesContainer.hpp"
#include "Pane.hpp"
#include "LogViewport.hpp"
#include "Bookmark.hpp"

class IAppContext
{
public:
    ~IAppContext() = default;

    virtual Display& display() = 0;
    virtual Keyboard& keyboard() = 0;
    virtual Pane& pane() = 0;
    virtual PanesContainer& panes() = 0;
    virtual void setActive(Pane &pane) = 0;
    virtual LogViewport& window() = 0;
    virtual BookmarksContainer& bookmarks() = 0;
};
