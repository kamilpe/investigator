#pragma once

#include "CursesToolkit/Display.hpp"
#include "CursesToolkit/IKeyboardInput.hpp"
#include "Logs/LogBuffer.hpp"
#include "PanesContainer.hpp"
#include "Statusbar.hpp"
#include <string>

class MainController : public IKeyboardInput
{
public:
    MainController(Display&, const LogBuffer&);
    bool parseKey(const int key, Keyboard& keyboard) override;

private:
    Display& display_;
    const LogBuffer& logBuffer_;
    PanesContainer panes_;
    std::unique_ptr<LogViewport> window_;
    std::unique_ptr<LogViewportController> controller_;
    Statusbar statusBar_;

    std::string lastHighlight_;
    std::string lastSearch_;

    Pane& pane();
    bool askForExit(Keyboard&);
    void gotoLine(Keyboard&);
    void grep(Keyboard&);
    void search(Keyboard&);
    void highlight(Keyboard&);
    void showPanes(Keyboard&);
    void showHelp(Keyboard&);
    void bookmark(Keyboard&);
    void setActive(Pane &pane);
};
