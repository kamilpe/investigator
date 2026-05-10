#pragma once

#include "Toolkit/ICanvas.hpp"
#include <string>
#include <regex>

class LogLinePrinter
{
public:
    LogLinePrinter(ICanvas& canvas, const int width);

    int countLines(const std::string& content) const;
    void changeWidth(const int width);
    int print(
        const int x,
        const int y,
        const std::string& content,
        const std::smatch& match);

private:
    ICanvas& canvas_;
    int width_;

    void printLine(
        const int x,
        const int y,
        const std::string::const_iterator& firstCh,
        const std::string::const_iterator& lastCh,
        const std::smatch& match);
    void printFill(
        const int x,
        const int y,
        const int length);
    Display::Pair setupCurrentColor(const std::string& content);

    Display::Pair currentColor_;
    std::regex warning_, error_;
};
