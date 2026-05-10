#pragma once

#include "ListWindow.hpp"
#include "CursesToolkit/IKeyboardInput.hpp"
#include <memory>

class PanesWindowController :  public IKeyboardInput
{
public:
    using UpdateActive = std::function<void(Pane&)>;
    PanesWindowController(
        Display &display,
        Keyboard &keyboard,
        PanesContainer &panes,
        UpdateActive updateActive);

    bool parseKey(const int key, Keyboard &keyboard) override;
    bool accepted() const;
    Pane& selected();

private:
    Display &display_;
    Keyboard &keyboard_;
    PanesContainer &panes_;
    UpdateActive updateActive_;
    std::unique_ptr<ListWindow> listView_;
    bool accept_;

    std::unique_ptr<ListWindow> createView() const;
    void rename();
    void remove();
};
