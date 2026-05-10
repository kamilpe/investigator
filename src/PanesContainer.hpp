#pragma once

#include "Pane.hpp"
#include <vector>
#include <memory>

class PanesContainer
{
public:
    using Panes = std::vector<std::unique_ptr<Pane>>;
    using const_iterator = Panes::const_iterator;

    PanesContainer(std::unique_ptr<Pane> initialPane);
    Pane& addPane(std::unique_ptr<Pane>);
    void setCurrent(Pane &pane);
    Pane& current() const;
    void remove(Pane &pane);
    const Panes& allPanes() const;

private:
    Panes panes_;
    Pane* current_;
};
