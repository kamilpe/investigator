#include "InputWindow.hpp"


InputWindow::InputWindow(
    Display& display,
    const std::string& title,
    const std::string& content,
    const unsigned maxlen,
    const unsigned width)
    : Window(display, 0, 0, 0, 0)
    , display_(display)
    , title_(" " + title + " ")
    , content_(content)
    , maxlen_(maxlen)
    , width_(width ? width : maxlen+4)
    , cursor_(content_.begin())
    , removeAtFirst_(true)
{
    resize(display.width(), display.height());
    display_.screenCursor(Display::CursorMode::Visible);
}

InputWindow::~InputWindow()
{
    display_.screenCursor(Display::CursorMode::Invisible);
}

void InputWindow::resize(const int w, const int h)
{
    Window::resize(width_, 3);
    centerWindow();
}

void InputWindow::draw()
{
    fillWith(Display::Pair::Dialog);
    bordering();
    print(3, 0, title_);

    setColor(Display::Pair::Input);
    const int inputX = 2;
    const int inputY = 1;
    print(inputX, inputY, std::string(width_ - 4, ' '));
    print(inputX, inputY, content_);

    moveCursor(inputX + std::distance(content_.begin(), cursor_), inputY);
}


void InputWindow::put(const char c)
{
    if (removeAtFirst_)
    {
        content_ = "";
        removeAtFirst_ = false;
    }

    if (maxlen_ && not(content_.length() < maxlen_))
    {
        return;
    }
    cursor_ = content_.insert(cursor_, c) + 1;
}

void InputWindow::left()
{
    removeAtFirst_ = false;
    if (cursor_ > content_.begin())
    {
        cursor_--;
    }
}

void InputWindow::right()
{
    removeAtFirst_ = false;
    if (cursor_ < content_.end())
    {
        cursor_++;
    }
}

void InputWindow::del()
{
    removeAtFirst_ = false;
    if (cursor_ < content_.end())
    {
        cursor_ = content_.erase(cursor_);
    }
}

void InputWindow::backspace()
{
    removeAtFirst_ = false;
    if (cursor_ > content_.begin())
    {
        cursor_ = content_.erase(cursor_ - 1);
    }
}

std::string InputWindow::content() const
{
    return content_;
}
