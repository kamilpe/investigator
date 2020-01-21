#pragma once

#include "Toolkit/Window.hpp"

class ProgressWindow : public Window
{
public:
    using Progress = float;

    ProgressWindow(Display& display, const std::string &title);
    ~ProgressWindow();

    void draw() override;
    void resize(const int w, const int h) override;
    void update(const Progress &prog);

private:
    Display& display_;
    std::string title_;
    int cur_;
    int max_;
    Progress prog_;
};
