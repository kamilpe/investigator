#pragma once

#include "ListWindowBase.hpp"
#include "Pane.hpp"

using PanesWindowList = ListWindowBase<std::unique_ptr<Pane>>;

class PanesWindow : public PanesWindowList
{
public:
    static constexpr auto NameWidth = 53;
    static constexpr auto WindowWidth = 60;
    
    PanesWindow(IAppContext &context, const Items &items, const typename Items::const_iterator selected);
    void resize(const int w, const int h) override;
    void draw() override;
    
protected:
    void printLine(int x, int y, bool selected, typename Items::const_iterator &line) const override;
};

