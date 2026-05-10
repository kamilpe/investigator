#pragma once

#include "Display.hpp"
#include <string>

class ICanvas
{
public:
    // make all const
    virtual void clear() = 0;
    virtual void setColor(const Display::Pair pair) const = 0;
    virtual void print(int x, int y, const std::string& str) const = 0;
    virtual void putc(int x, int y, const char c) = 0;
    virtual void lineh(int x, int y, int w) = 0;
    virtual void bar(int x, int y, int w) = 0;
    virtual void highlight(const bool value) const = 0;
    virtual void underline(const bool value) const = 0;
    virtual void bordering() = 0;
    virtual void fillWith(const Display::Pair pair) = 0;

    ~ICanvas() = default;
};
