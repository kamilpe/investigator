#pragma once
#include "IAppContext.hpp"
#include "Toolkit/Window.hpp"

#include <memory>

template<typename ItemType>
class ListWindowBase : public Window
{
public:
    using Items = std::vector<ItemType>;

    ListWindowBase(IAppContext &context,
               const Items &items,
               const typename Items::const_iterator selected) :
    Window(context.display(), 0, 0, 0, 0),
    items_(items),
    selected_(selected)
    {
    }
    
    ~ListWindowBase()
    {
    }
    
    void select(const typename Items::const_iterator selected)
    {
        selected_ = selected;
    }

    void up()
    {
        if (selected_ == items_.cend())
            return;
        
        if (selected_ > items_.cbegin())
            selected_--;
    }
    void down()
    {
        if (selected_ == items_.cend())
            return;
    
        if (selected_ < items_.end() - 1)
            selected_++;
    }
        
    const Items& items() const
    {
        return items_;
    }
    
    const typename Items::const_iterator selected() const {
        return selected_;
    }

protected:

    virtual void printLine(int x, int y, bool selected, typename Items::const_iterator &line) const = 0;
    
    void drawList(int x, int y, int count) const
    {
        for (auto it = items_.begin();
             it != items_.end() && count > 0;
             ++it)
        {
            printLine(x, y, (it == selected_), it);
            y++;
        }
    }

private:
    const Items& items_;
    typename Items::const_iterator selected_;
};
