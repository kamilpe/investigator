#pragma once

#include "IWindow.hpp"
#include <vector>

class Display
{
public:
    enum class Pair
    {
        RegularLine = 1,
        LineNum,
        LogLineWarning,
        LogLineError,
        Statusbar,
        Dialog,
        Dialog2,
        Input,
        Highlight,
        NotifyWindow,
        ProgressBar,
        BlueBorder,
        YellowBorder
    };

    enum class CursorMode
    {
        Invisible,
        Visible,
        VeryVisible
    };

    void start();
    void stop();
    void refresh();

    int width() const;
    int height() const;
    void resize();

    void toRegister(IWindow* window);
    void unregister(IWindow* window);

    void screenCursor(const CursorMode mode);

private:
    int width_;
    int height_;
    std::vector<IWindow*> windows_;

    void initColors() const;
};
