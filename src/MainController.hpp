#pragma once

#include "CursesToolkit/Display.hpp"
#include "CursesToolkit/IKeyboardInput.hpp"
#include "Logs/LogBuffer.hpp"
#include "PanesContainer.hpp"
#include "Statusbar.hpp"
#include "IAppContext.hpp"
#include <string>

class MainController : public IKeyboardInput, public IAppContext
{
public:
    MainController(Display&, const LogBuffer&, Keyboard &keyboard);

    bool parseKey(const int key, Keyboard &keyboard) override;
    Display& display() override;
    Keyboard& keyboard() override;
    Pane& pane() override;
    PanesContainer& panes() override;
    void setActive(Pane &pane) override;
    LogViewport& window() override;

private:
    Display& display_;
    const LogBuffer& logBuffer_;
    PanesContainer panes_;
    Keyboard &keyboard_;
    std::unique_ptr<LogViewport> window_;
    std::unique_ptr<LogViewportController> controller_;
    Statusbar statusBar_;

    std::string lastHighlight_;
    std::string lastSearch_;

    bool askForExit();
    void gotoLine();
    void grep();
    void search();
    void highlight();
    void showPanes();
    void showHelp();
    void bookmark();
};
