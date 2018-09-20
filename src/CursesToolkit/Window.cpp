#include "Window.hpp"

#include <stdarg.h>
#include <ncurses.h>
#include <stdexcept>

Window::Window(Display& display, const int x, const int y, const int w, const int h)
    : display_(display)
    , w_(w)
    , h_(h)
    , visibility_(true)
{
    WINDOW *win = newwin(h, w, y, x);
    if (!win)
    {
        throw std::runtime_error("Unable to create window");
    }
    wrefresh(win);
    winPtr_ = win;
    display_.toRegister(this);
}

Window::~Window()
{
    display_.unregister(this);
    delwin(static_cast<WINDOW*>(winPtr_));
}

int Window::x() const
{
    return x_;
}

int Window::y() const
{
    return y_;
}

int Window::width() const
{
    return w_;
}

int Window::height() const
{
    return h_;
}

void Window::setVisibility(bool visibility)
{
    visibility_ = visibility;
    clear();
}

bool Window::getVisibility()
{
    return visibility_;
}

void Window::fillWith(const Display::Pair pair)
{
    wbkgd(static_cast<WINDOW*>(winPtr_), COLOR_PAIR(static_cast<short>(pair)));
}

void Window::clear()
{
    wclear(static_cast<WINDOW*>(winPtr_));
}

void Window::resize(const int w, const int h)
{
    w_ = w;
    h_ = h;
    wresize(static_cast<WINDOW*>(winPtr_), h_, w_);
}

void Window::moveWindow(const int x, const int y)
{
    mvwin(static_cast<WINDOW*>(winPtr_), y, x);
}

void Window::centerWindow()
{
    moveWindow(
        (display_.width()-width())/2,
        (display_.height()-height())/2);
}

void Window::moveCursor(const int x, const int y)
{
    wmove(static_cast<WINDOW*>(winPtr_), y, x);
}

void Window::setColor(const Display::Pair pair)
{
    wattron(static_cast<WINDOW*>(winPtr_),
            COLOR_PAIR(static_cast<short>(pair)));
}

void Window::print(int x, int y, const std::string& str)
{
    mvwaddstr(static_cast<WINDOW*>(winPtr_), y, x, str.c_str());
}

void Window::putc(int x, int y, const char c)
{
    mvwaddch(static_cast<WINDOW*>(winPtr_), y, x, c);
}

void Window::lineh(int x, int y, int w)
{
    mvwhline(static_cast<WINDOW*>(winPtr_), y, x, ACS_HLINE, w);
}

void Window::bar(int x, int y, int w)
{
    mvwhline(static_cast<WINDOW*>(winPtr_), y, x, ACS_CKBOARD, w);
}

void Window::refresh()
{
    wrefresh(static_cast<WINDOW*>(winPtr_));
}

void Window::bordering()
{
    wborder(static_cast<WINDOW*>(winPtr_),
            0,0,0,0,0,0,0,0);
}

void Window::underline(const bool value)
{
    if (value)
    {
        wattron(static_cast<WINDOW*>(winPtr_), A_STANDOUT);
    }
    else
    {
        wattroff(static_cast<WINDOW*>(winPtr_), A_STANDOUT);
    }
}
