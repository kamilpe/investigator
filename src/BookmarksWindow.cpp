#include "BookmarksWindow.hpp"
#include <sstream>
#include <iomanip>

BookmarksWindow::BookmarksWindow(
    IAppContext &context,
    const Items &items,
    const typename Items::const_iterator selected)
    : BookmarksWindowList(context, items, selected)
{
    resize(context.display().width(), context.display().height());
}

void BookmarksWindow::resize(const int w, const int h)
{
    moveWindow(0,0);
    Window::resize(WindowWidth, h-1);
}


void BookmarksWindow::printLine(
    int x, int y, bool selected, typename Items::const_iterator &line) const
{
    if (focus_) {
        highlight(selected);
    } else {
        underline(selected);
    }

    std::stringstream label;
    label << std::setw(static_cast<unsigned int>(WindowWidth-2)) << std::left << line->name;
    print(x, y, label.str());
}

void BookmarksWindow::setFocus(bool focus)
{
    focus_ = focus;
}

void BookmarksWindow::draw()
{
    if (focus_) {
        fillWith(Display::Pair::YellowBorder);
    } else {
        fillWith(Display::Pair::BlueBorder);
    }
    bordering();
    setColor(Display::Pair::RegularLine);
    drawList(1, 1, height() - 3);
    setColor(Display::Pair::Dialog);
    
    setColor(Display::Pair::RegularLine);
    if (focus_) {
        print (2, height() - 2,  "[r] rename | [d] delete");
    } else {
        print (2, height() - 2,  "                       ");
    }
}
