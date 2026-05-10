#pragma once
#include "CursesToolkit/Window.hpp"
#include "IAppContext.hpp"

class Statusbar : Window
{
public:
    Statusbar(Display&, IAppContext&);

    void draw() override;
    void resize(const int w, const int h) override;

private:
    IAppContext& context_;
    std::string fullStatusText;
};
