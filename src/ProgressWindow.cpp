#include "ProgressWindow.hpp"

ProgressWindow::ProgressWindow(Display& display, const std::string &title)
    : Window(display, 0, 0, 0, 0)
    , display_(display)
    , title_(" " + title + " ")
    , cur_(0)
    , max_(0)
    , prog_(0)
{
    resize(display_.width(), display_.height());
}

ProgressWindow::~ProgressWindow()
{
}

void ProgressWindow::resize(const int w, const int h)
{
    Window::resize(w/3, 4);
    moveWindow(
        (display_.width()-width())/2,
        (display_.height()-height())-2);

    max_ = width() - 2;
    cur_ = 0;
}

void ProgressWindow::draw()
{
    fillWith(Display::Pair::NotifyWindow);
    bordering();
    print(2,0,title_);

    setColor(Display::Pair::ProgressBar);
    bar(2, 2, cur_);

    setColor(Display::Pair::NotifyWindow);
    print(width()/2-1, 1,
          std::to_string(static_cast<int>(prog_*100)) + "%");
}

void ProgressWindow::update(const Progress &prog)
{
    prog_ = prog;

    const int next = max_ * prog;
    if (cur_ != next)
    {
        cur_ = next;
        display_.refresh();
    }
}
