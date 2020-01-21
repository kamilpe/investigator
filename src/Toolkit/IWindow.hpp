#pragma once

class IWindow
{
public:
    virtual void resize(const int w, const int h) = 0;
    virtual void moveWindow(const int x, const int y) = 0;
    virtual void centerWindow() = 0;
    virtual void moveCursor(const int x, const int y) = 0;
    virtual void draw() = 0;
    virtual void refresh() = 0;
    virtual int x() const = 0;
    virtual int y() const = 0;
    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual void setVisibility(bool visibility) = 0;
    virtual bool getVisibility() = 0;

    virtual ~IWindow() = default;
};
