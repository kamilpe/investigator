#include "LogLinePrinter.hpp"
#include <cmath>

LogLinePrinter::LogLinePrinter(ICanvas& canvas, const int width)
    : canvas_(canvas)
    , width_(width)
    , currentColor_(Display::Pair::RegularLine)
    , warning_(" WRN/", std::regex::icase | std::regex::basic)
    , error_(" ERR/", std::regex::icase | std::regex::basic)
{
}

int LogLinePrinter::countLines(const std::string& content) const
{
    return content.empty() ? 1 :
        std::ceil(
            static_cast<double>(content.length()) /
            static_cast<double>(width_));
}

void LogLinePrinter::changeWidth(const int width)
{
    width_ = width;
}

int LogLinePrinter::print(
    const int x,
    const int y,
    const std::string& content,
    const std::smatch& match)
{
    currentColor_ = setupCurrentColor(content);

    const int lines = countLines(content);
    for (int l = 0; l<lines; ++l)
    {
        const auto currentY = y+l;
        if (match.size() > 0)
        {
            const auto firstCh = content.begin() + (width_ * l);
            const auto lastCh =
                firstCh + width_ < content.end() ? firstCh + width_ : content.end();

            printLine(x, currentY, firstCh, lastCh, match);
            printFill(x, currentY, static_cast<int>(std::distance(firstCh, lastCh)));
        }
        else
        {
            const auto line = content.substr(width_*l, width_);
            canvas_.setColor(currentColor_);
            canvas_.print(x, currentY, content.substr(width_*l, width_));
            printFill(x, currentY, static_cast<int>(line.length()));
        }
    }
    return lines;
}

Display::Pair LogLinePrinter::setupCurrentColor(const std::string& content)
{
    try
    {
        std::smatch match;
        if (std::regex_search(content, match, warning_))
        {
            return Display::Pair::LogLineWarning;
        }
        if (std::regex_search(content, match, error_))
        {
            return Display::Pair::LogLineError;
        }
    }
    catch(const std::regex_error& e)
    {
        // TODO: What to do here?
    }
    return Display::Pair::RegularLine;
}

void LogLinePrinter::printLine(
    const int x,
    const int y,
    const std::string::const_iterator& firstCh,
    const std::string::const_iterator& lastCh,
    const std::smatch& match)
{
    for (auto it = firstCh; it < lastCh; ++it)
    {
        canvas_.setColor(currentColor_);
        for (const auto& m : match)
        {
            if (it >= m.first && it < m.second)
            {
                canvas_.setColor(Display::Pair::Highlight);
            }
        }
        canvas_.putc(
            x + static_cast<int>(std::distance(firstCh, it)),
            y,
            *it);
    }
}

void LogLinePrinter::printFill(
    const int x,
    const int y,
    const int length)
{
    for (int x2 = length; x2 < width_; ++x2)
    {
        canvas_.putc(x+x2, y, ' ');
    }
}
