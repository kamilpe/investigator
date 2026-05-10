#include "Display.hpp"

#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <ncurses.h>

namespace
{
    enum Colors {
        COLOR_GRAY = 10,
        COLOR_DKGRAY,
        COLOR_LTGRAY,
        COLOR_DKBLUE
    };

    int constexpr c(int color)
    {
        return static_cast<int>(1000.0/256.0*static_cast<double>(color));
    }
}

void Display::start()
{
    initscr();
    if(!has_colors())
	{
        endwin();
		throw std::runtime_error("Your terminal does not support color");
    }
    start_color();
    initColors();
    curs_set(0);
    getmaxyx(stdscr, height_, width_);
    ::refresh();
}

void Display::stop()
{
    endwin();
}

void Display::refresh()
{
    for (auto* window : windows_)
    {
        if (window->getVisibility())
        {
            window->draw();
        }
    }
    for (auto* window : windows_)
    {
        if (window->getVisibility())
        {
            window->refresh();
        }
    }
    doupdate();
}

void Display::initColors() const
{
    init_color(COLOR_BLACK,  c(0),   c(0),   c(0));
    init_color(COLOR_GRAY,   c(128), c(128), c(128));
    init_color(COLOR_DKGRAY, c(64),  c(64),  c(64));
    init_color(COLOR_LTGRAY, c(162), c(162), c(162));
    init_color(COLOR_CYAN,   c(0),   c(128), c(124));
    init_color(COLOR_DKBLUE, c(0),   c(41),  c(92));
    init_pair(static_cast<short>(Pair::RegularLine),      COLOR_LTGRAY, COLOR_BLACK);
    init_pair(static_cast<short>(Pair::LogLineWarning), COLOR_YELLOW, COLOR_BLACK);
    init_pair(static_cast<short>(Pair::LogLineError),   COLOR_WHITE,  COLOR_RED);
    init_pair(static_cast<short>(Pair::LineNum),      COLOR_YELLOW, COLOR_BLACK);
    init_pair(static_cast<short>(Pair::Statusbar),    COLOR_WHITE,  COLOR_DKBLUE);
    init_pair(static_cast<short>(Pair::Dialog),       COLOR_WHITE,  COLOR_DKBLUE);
    init_pair(static_cast<short>(Pair::Input),        COLOR_YELLOW, COLOR_DKGRAY);
    init_pair(static_cast<short>(Pair::Highlight),    COLOR_DKGRAY, COLOR_YELLOW);
    init_pair(static_cast<short>(Pair::NotifyWindow), COLOR_BLACK, COLOR_LTGRAY);
    init_pair(static_cast<short>(Pair::ProgressBar),  COLOR_DKBLUE, COLOR_BLUE);
    init_pair(static_cast<short>(Pair::BlueBorder),     COLOR_DKBLUE, COLOR_BLACK);
    init_pair(static_cast<short>(Pair::YellowBorder),COLOR_YELLOW, COLOR_BLACK);
}

int Display::width() const
{
    return width_;
}

int Display::height() const
{
    return height_;
}

void Display::toRegister(IWindow* window)
{
    windows_.push_back(window);
}

void Display::unregister(IWindow* window)
{
    windows_.erase(std::find(windows_.begin(), windows_.end(), window));
}

void Display::resize()
{
    getmaxyx(stdscr, height_, width_);
    for (auto* window : windows_)
    {
        window->resize(width_, height_);
    }
    refresh();
}

void Display::screenCursor(const CursorMode mode)
{
    curs_set(static_cast<int>(mode));
}
