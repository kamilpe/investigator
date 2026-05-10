#pragma once
#include "IAppContext.hpp"
#include "Toolkit/Window.hpp"

#include <memory>

class ListWindow : public Window
{
public:
    using Items = std::vector<std::string>;

    ListWindow(
        IAppContext &context,
        const Items &items,
        const std::size_t selectedIndex);
    ~ListWindow();

    void up();
    void down();
    const Items& items() const;
    const Items::const_iterator selected();

protected:
    const Items items_;
    Items::const_iterator selected_;
};
