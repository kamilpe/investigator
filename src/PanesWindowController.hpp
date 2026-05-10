#pragma once

#include "ListWindow.hpp"
#include "IAppContext.hpp"
#include "Toolkit/IKeyboardInput.hpp"
#include <memory>

class PanesWindowController :  public IKeyboardInput
{
public:
    PanesWindowController(IAppContext &context);

    bool parseKey(const int key, Keyboard &keyboard) override;
    bool accepted() const;
    Pane& selected();

private:
    IAppContext &context_;
    std::unique_ptr<ListWindow> listView_;
    bool accept_;

    std::unique_ptr<ListWindow> createView() const;
    void rename();
    void remove();
};
