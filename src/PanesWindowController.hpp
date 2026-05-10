#pragma once

#include "ListWindowBase.hpp"
#include "PanesContainer.hpp"
#include "Toolkit/IKeyboardInput.hpp"
#include "PanesWindow.hpp"
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
    std::unique_ptr<PanesWindowList> listWindow_;
    bool accept_;
    void rename();
    void remove();
    void createView();
};
