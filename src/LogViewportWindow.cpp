#include "LogViewportWindow.hpp"

#include <algorithm>
#include <iterator>
#include <regex>

namespace
{
    LineId getMaxIdFromView(const LogBufferView& bufferView)
    {
        const auto it = std::max_element(bufferView.lines().begin(), bufferView.lines().end());
        if (it != bufferView.lines().end())
        {
            return *it;
        }
        else
        {
            return 0;
        }
    }

    int getDigitsCount(int num)
    {
        return static_cast<int>(std::to_string(num).length());
    }
}

LogViewportWindow::LogViewportWindow(
    Display& display,
    const LogBufferView& bufferView)
    : Window(display, 0, 0, display.width(), display.height()-1)
    , bufferView_(bufferView)
    , digitsCount_(getDigitsCount(static_cast<int>(getMaxIdFromView(bufferView))))
    , logLinePos_(digitsCount_ + 1)
    , logPrinter_(*this, width() - logLinePos_)
    , viewport_(bufferView.cbegin())
    , cursor_(bufferView.cbegin())
    , firstLine_(bufferView.cend())
    , lastLine_(bufferView.cend())
    , endLine_(bufferView.cend())
    , linesOnScreen_(0)
{
}

LogViewportWindow::~LogViewportWindow()
{
}

void LogViewportWindow::resize(const int w, const int h)
{
    Window::resize(w, h-1);
    logPrinter_.changeWidth(width() - logLinePos_);
    draw();
    goTo(cursor_);
}

std::optional<LogBufferView::iterator> LogViewportWindow::cursor() const
{
    if (cursor_ == bufferView_.cend())
        return {};
    return cursor_;
}

void LogViewportWindow::draw()
{
    if (bufferView_.size() == 0)
    {
        return;
    }

    firstLine_ = viewport_;
    linesOnScreen_ = 0;

    auto lineIt = firstLine_;
    int currentY = 0;

    while (currentY < height() &&
           lineIt != bufferView_.cend())
    {
        markCursor(lineIt);
        printLineNum(currentY, lineIt);
        putc(digitsCount_, currentY, ' ');
        const int lines = printLogLine(currentY, lineIt);
        for (int y2 = 1; y2 < lines; ++y2)
            fillLineNumSpace(currentY+y2);

        currentY += lines;
        linesOnScreen_ += lines;
        lastLine_ = lineIt;
        lineIt++;
    }
    endLine_ = lastLine_ + 1;
}

void LogViewportWindow::markCursor(const LogBufferView::iterator& it)
{
    if (focus_)
    {
        underline(false);
        Window::highlight(cursor_ == it);
    } else {
        underline(cursor_ == it);
        Window::highlight(false);
    }
}

bool LogViewportWindow::cursorIsOutsieWindow() const
{
    return (cursor_ < firstLine_ || cursor_ > lastLine_);
}

int LogViewportWindow::printLogLine(const int y, const LogBufferView::iterator& it)
{
    const auto& line = bufferView_.value(it);
    std::smatch match;
    if (hlre_.has_value())
    {
        try {
            std::regex_search(line, match, *hlre_);
        } catch (const std::regex_error& e) {
            /* do nothing*/
        };
    }
    return logPrinter_.print(logLinePos_, y, line, match);
}

void LogViewportWindow::printLineNum(const int y, const LogBufferView::iterator& it)
{
    std::string numBuffer(digitsCount_, ' ');
    std::string numStr(std::to_string((*it)+1));

    setColor(Display::Pair::LineNum);

    numBuffer.replace(numBuffer.length() - numStr.length(), numStr.length(), numStr);
    print(0, y, numBuffer.c_str());
}

void LogViewportWindow::fillLineNumSpace(const int y)
{
    std::string numBuffer(digitsCount_+1, ' ');
    setColor(Display::Pair::LineNum);
    print(0, y, numBuffer.c_str());
}

void LogViewportWindow::up()
{
    if (cursor_ > bufferView_.cbegin())
    {
        cursor_--;
        if (cursor_ < viewport_)
        {
            viewport_ = cursor_;
        }
    }
}

void LogViewportWindow::alignViewportToCursor(const int fromTopCount)
{
    auto newViewportIt = cursor_;
    for (int lines = fromTopCount;
         newViewportIt > bufferView_.cbegin() &&
             lines - logPrinter_.countLines(bufferView_.value(newViewportIt)) > 0;
         --newViewportIt)
    {
        lines -= logPrinter_.countLines(bufferView_.value(newViewportIt));
    }
    viewport_ = newViewportIt;
}

void LogViewportWindow::down()
{
    if (cursor_ + 1 < bufferView_.cend())
    {
        cursor_++;
        if (cursor_ > lastLine_)
        {
            alignViewportToCursor(height());
        }
    }
}

void LogViewportWindow::pageUp()
{
    if (bufferView_.cbegin() > firstLine_ - 1)
    {
        top();
        return;

    }
    cursor_ = firstLine_ - 1;
    alignViewportToCursor(height());
}

void LogViewportWindow::pageDown()
{
    if (lastLine_ + 1 >= bufferView_.cend())
    {
        bottom();
        return;
    }
    viewport_ = lastLine_ + 1;
    cursor_ = viewport_;
}

void LogViewportWindow::top()
{
    cursor_ = bufferView_.cbegin();
    viewport_ = bufferView_.cbegin();
}

void LogViewportWindow::bottom()
{
    cursor_ = bufferView_.cend() - 1;
    alignViewportToCursor(height());
}

void LogViewportWindow::goTo(const LogBufferView::iterator& iter)
{
    cursor_ = iter;
    if (cursorIsOutsieWindow())
    {
        alignViewportToCursor(height()/2);
    }
}

LogBufferView::iterator LogViewportWindow::find(const std::string& expr)
{
    std::regex regexp{expr, std::regex::icase | std::regex::basic};
    for (auto it = cursor_ + 1;
         it < bufferView_.cend();
         ++it)
    {
        const auto& line = bufferView_.value(it);
        try {
            std::smatch match;
            if (std::regex_search(line, match, regexp))
                return it;
        } catch (const std::regex_error& e) {
            /* do nothing*/
        };
    }
    return bufferView_.cend();
}

const LogBufferView& LogViewportWindow::buffer() const
{
    return bufferView_;
}

void LogViewportWindow::highlighting(const std::string& expr)
{
    if (expr.empty())
    {
        hlre_.reset();
        return;
    }
    hlre_ = std::regex(expr, std::regex::icase | std::regex::basic);
}

void LogViewportWindow::setFocus(bool focus)
{
    focus_ = focus;
}
