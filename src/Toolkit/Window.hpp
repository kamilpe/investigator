#pragma once

#include "Display.hpp"
#include "IWindow.hpp"
#include "ICanvas.hpp"
#include <string>
#include <vector>
#include <utility>

class Window : public IWindow, protected ICanvas
{
public:
    Window(Display& display, const int x, const int y, const int w, const int h);
    ~Window();

    void refresh() override;
    void resize(const int w, const int h) override;
    void moveWindow(const int x, const int y) override;
    void centerWindow() override;
    void moveCursor(const int x, const int y) override;
    int x() const override;
    int y() const override;
    int width() const override;
    int height() const override;
    void setVisibility(bool visibility) override;
    bool getVisibility() override;

protected:
    Display& display_;

    void clear() override;
    void setColor(const Display::Pair pair) const override;
    void print(int x, int y, const std::string& str) const override;
    void putc(int x, int y, const char c) override;
    void lineh(int x, int y, int w) override;
    void bar(int x, int y, int w) override;
    void highlight(const bool value) const override;
    void underline(const bool value) const override;
    void bordering() override;
    void fillWith(const Display::Pair pair) override;

private:
    void *winPtr_ = nullptr;
    int x_, y_, w_, h_;
    bool visibility_;
};
