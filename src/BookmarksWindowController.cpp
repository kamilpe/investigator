#include "BookmarksWindowController.hpp"
#include "InputWindowController.hpp"
#include <ncurses.h>


namespace
{
constexpr auto WindowWidth = 50;

class LeftListWindow : public ListWindowBase<std::string>
{
public:
    LeftListWindow(
        IAppContext &context,
        const Items &items,
        const typename Items::iterator selectedIndex)
        : ListWindowBase<std::string>(context, items, selectedIndex)
    {
        resize(context.display().width(), context.display().height());
    }

    void resize(const int w, const int h) override
    {
        moveWindow(0,0);
        Window::resize(WindowWidth, h-1);
    }

    void draw() override
    {
        fillWith(Display::Pair::Bookmark);
        bordering();

        int xpos = 1;
        int ypos = 1;

        for (auto it = items_.begin();
             it != items_.end() && ypos < height();
             ++it)
        {
            if (it == selected_)
            {
                setColor(Display::Pair::Highlight);
            }
            else
            {
                setColor(Display::Pair::Bookmark);
            }


            std::string label(*it);
            label.resize(WindowWidth-2);
            print(xpos, ypos, label);
            ++ypos;
        }
    }
};

}

BookmarksWindowController::BookmarksWindowController(IAppContext &context)
    : context_(context)
{
    listView_ = createView();
}

bool BookmarksWindowController::parseKey(const int key, Keyboard& keyboard)
{
    switch (key)
    {
    case KEY_UP:
        listView_->up();
        break;
    case KEY_DOWN:
        listView_->down();
        break;
    }
    return true;
}

std::unique_ptr<ListWindowBase<std::string>> BookmarksWindowController::createView() const
{
    ListWindowBase<std::string>::Items items;
    for (const auto &item : context_.bookmarks())
    {
        items.push_back(item.name);
    }

    return std::make_unique<LeftListWindow>(
        context_,
        items,
        items.begin());
}

ListWindowBase<std::string>& BookmarksWindowController::window()
{
    return *listView_;
}
