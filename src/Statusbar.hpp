#pragma once
#include "PanesContainer.hpp"
#include "CursesToolkit/Window.hpp"

class Statusbar : Window
{
public:
    Statusbar(Display&, const PanesContainer&);

    void draw() override;
    void resize(const int w, const int h) override;

private:
    const PanesContainer& panes_;

    std::string fullStatusText;
};
