#pragma once
#include "PanesContainer.hpp"
#include "CursesToolkit/Window.hpp"

#include <memory>

class ListWindow : public Window
{
public:
    using Items = std::vector<std::string>;
    using Placer = std::function<std::pair<int,int>(int, int)>;

    ListWindow(
        Display &display,
        const std::string &title,
        const std::string &header,
        const std::string &footer,
        const Items &items,
        const std::size_t selectedIndex,
        Placer placer);
    ~ListWindow();

    void draw() override;
    void resize(const int w, const int h) override;
    void up();
    void down();
    const Items& items() const;
    const Items::const_iterator selected();

private:
    const std::string title_;
    const std::string header_;
    const std::string footer_;
    const Items items_;
    Items::const_iterator selected_;
    Placer placer_;
};
