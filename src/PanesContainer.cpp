#include "PanesContainer.hpp"
#include <algorithm>

PanesContainer::PanesContainer(std::unique_ptr<Pane> initialPane)
    : current_(initialPane.get())
{
    panes_.push_back(std::move(initialPane));
}

Pane& PanesContainer::addPane(std::unique_ptr<Pane> pane)
{
    panes_.push_back(std::move(pane));
    return *panes_.back();
}

Pane& PanesContainer::current() const
{
    return *current_;
}

PanesContainer::const_iterator PanesContainer::currentIterator() const
{
    return std::find_if(panes_.begin(), panes_.end(),
        [this](const auto &pane) { return pane.get() == current_; });
}

const PanesContainer::Panes& PanesContainer::allPanes() const
{
    return panes_;
}

void PanesContainer::setCurrent(Pane &pane)
{
    current_ = &pane;
}

void PanesContainer::remove(Pane &pane)
{
    if (&pane != panes_.front().get())
    {
        setCurrent(*panes_.front());
        panes_.erase(
            std::remove_if(
                panes_.begin(),
                panes_.end(),
                [&pane](const auto &it){return &pane == it.get();}),
            panes_.end());
    }
}
