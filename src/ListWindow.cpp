#include "ListWindow.hpp"
#include <stdexcept>
#include <ncurses.h>

ListWindow::ListWindow(
    IAppContext &context,
    const std::string &title,
    const std::string &header,
    const std::string &footer,
    const Items &items,
    const std::size_t selectedIndex,
    Placer placer)
    : Window(context.display(), 0, 0, 0, 0)
    , header_(header)
    , footer_(footer)
    , items_(items)
    , selected_(items_.begin() + selectedIndex)
    , placer_(placer)
{
    resize(context.display().width(), context.display().height());
}

ListWindow::~ListWindow()
{
}

void ListWindow::resize(const int w, const int h)
{
    const auto dim = placer_(w,h);
    Window::resize(dim.first, dim.second);
    centerWindow();
}

void ListWindow::draw()
{
    fillWith(Display::Pair::Dialog);
    bordering();
    print(3, 0, title_);

    int xpos = 2;
    int ypos = 2;

    print(xpos, ypos, header_);
    ++ypos;
    lineh(xpos,ypos, header_.size());
    ypos+=1;

    for (auto it = items_.begin();
         it != items_.end() && ypos < height();
         ++it)
    {
        if (it == selected_)
        {
            setColor(Display::Pair::Highlight);
        }
        else
        {
            setColor(Display::Pair::Dialog);
        }

        print(xpos, ypos, *it);
        ++ypos;
    }
    setColor(Display::Pair::Dialog);
    print (2, height() - 2, footer_);
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
