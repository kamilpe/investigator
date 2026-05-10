#include "ListWindow.hpp"
#include <stdexcept>
#include <ncurses.h>

ListWindow::ListWindow(
    IAppContext &context,
    const Items &items,
    const std::size_t selectedIndex)
    : Window(context.display(), 0, 0, 0, 0)
    , selected_(items_.begin() + selectedIndex)
{
}

ListWindow::~ListWindow()
{
}

void ListWindow::up()
{
    if (selected_ > items_.begin())
        selected_--;
}

void ListWindow::down()
{
    if (selected_ < items_.end() - 1)
        selected_++;
}

const ListWindow::Items::const_iterator ListWindow::selected()
{
    return selected_;
}

const ListWindow::Items& ListWindow::items() const
{
    return items_;
}
