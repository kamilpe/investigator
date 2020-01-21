#pragma once

#include "Display.hpp"
#include <string>

class ICanvas
{
public:
    virtual void clear() = 0;
    virtual void setColor(const Display::Pair pair) = 0;
    virtual void print(int x, int y, const std::string& str) = 0;
    virtual void putc(int x, int y, const char c) = 0;
    virtual void lineh(int x, int y, int w) = 0;
    virtual void bar(int x, int y, int w) = 0;
    virtual void underline(const bool value) = 0;
    virtual void bordering() = 0;
    virtual void fillWith(const Display::Pair pair) = 0;

    ~ICanvas() = default;
};
