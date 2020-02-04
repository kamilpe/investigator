#pragma once

#include "ListWindowBase.hpp"
#include "Bookmark.hpp"

using BookmarksWindowList = ListWindowBase<Bookmark>;

class BookmarksWindow : public BookmarksWindowList
{
public:
    static constexpr auto NameWidth = 48;
    static constexpr auto WindowWidth = 50;

    BookmarksWindow(
        IAppContext &context,
        const Items &items,
        const typename Items::const_iterator selected);

    void resize(const int w, const int h) override;
    void draw() override;
    void setFocus(bool focus);
    void selectClosest(LineId id);

protected:
    void printLine(
        int x,
        int y,
        bool selected,
        typename Items::const_iterator &line) const override;

private:
    bool focus_ = false;
};
