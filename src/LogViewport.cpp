#include "LogViewport.hpp"

#include <algorithm>
#include <iterator>
#include <regex>

namespace
{
    int getMaxIdFromView(const LogBufferView& bufferView)
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
        return std::to_string(num).length();
    }
}

LogViewport::LogViewport(
    Display& display,
    const LogBufferView& bufferView,
    const int shift)
    : Window(display, shift, 0, display.width() - shift, display.height()-1)
    , bufferView_(bufferView)
    , digitsCount_(getDigitsCount(getMaxIdFromView(bufferView)))
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

LogViewport::~LogViewport()
{
}

void LogViewport::resize(const int w, const int h)
{
    Window::resize(w, h-1);
    logPrinter_.changeWidth(width() - logLinePos_);
    draw();
    goTo(cursor_);
}

std::optional<LogBufferView::iterator> LogViewport::cursor() const
{
    if (cursor_ == bufferView_.cend())
        return {};
    return cursor_;
}

void LogViewport::draw()
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

void LogViewport::markCursor(const LogBufferView::iterator& it)
{
    underline(cursor_ == it);
}

bool LogViewport::cursorIsOutsieWindow() const
{
    return (cursor_ < firstLine_ || cursor_ > lastLine_);
}

int LogViewport::printLogLine(const int y, const LogBufferView::iterator& it)
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

void LogViewport::printLineNum(const int y, const LogBufferView::iterator& it)
{
    std::string numBuffer(digitsCount_, ' ');
    std::string numStr(std::to_string((*it)+1));

    setColor(Display::Pair::LineNum);

    numBuffer.replace(numBuffer.length() - numStr.length(), numStr.length(), numStr);
    print(0, y, numBuffer.c_str());
}

void LogViewport::fillLineNumSpace(const int y)
{
    std::string numBuffer(digitsCount_+1, ' ');
    setColor(Display::Pair::LineNum);
    print(0, y, numBuffer.c_str());
}

void LogViewport::up()
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

void LogViewport::alignViewportToCursor(const int fromTopCount)
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

void LogViewport::down()
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

void LogViewport::pageUp()
{
    if (bufferView_.cbegin() > firstLine_ - 1)
    {
        top();
        return;

    }
    cursor_ = firstLine_ - 1;
    alignViewportToCursor(height());
}

void LogViewport::pageDown()
{
    if (lastLine_ + 1 >= bufferView_.cend())
    {
        bottom();
        return;
    }
    viewport_ = lastLine_ + 1;
    cursor_ = viewport_;
}

void LogViewport::top()
{
    cursor_ = bufferView_.cbegin();
    viewport_ = bufferView_.cbegin();
}

void LogViewport::bottom()
{
    cursor_ = bufferView_.cend() - 1;
    alignViewportToCursor(height());
}

void LogViewport::goTo(const LogBufferView::iterator& iter)
{
    cursor_ = iter;
    if (cursorIsOutsieWindow())
    {
        alignViewportToCursor(height()/2);
    }
}

LogBufferView::iterator LogViewport::find(const std::string& expr)
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

const LogBufferView& LogViewport::buffer() const
{
    return bufferView_;
}

void LogViewport::highlight(const std::string& expr)
{
    if (expr.empty())
    {
        hlre_.reset();
        return;
    }
    hlre_ = std::regex(expr, std::regex::icase | std::regex::basic);
}
