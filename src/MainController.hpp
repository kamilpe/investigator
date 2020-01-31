#pragma once

#include "Toolkit/Display.hpp"
#include "Toolkit/IKeyboardInput.hpp"
#include "Logs/LogBuffer.hpp"
#include "PanesContainer.hpp"
#include "Statusbar.hpp"
#include "IAppContext.hpp"
#include "BookmarksWindowController.hpp"
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
    LogViewportWindow& window() override;
    BookmarksContainer& bookmarks() override;

private:
    Display& display_;
    const LogBuffer& logBuffer_;
    PanesContainer panes_;
    BookmarksContainer bookmarks_;
    Keyboard &keyboard_;
    std::unique_ptr<LogViewportWindow> logViewportWindow_;
    std::unique_ptr<LogViewportController> logViewportController_;
    std::unique_ptr<BookmarksWindowController> bookmarksController_;
    Statusbar statusBar_;

    std::string lastHighlight_;
    std::string lastSearch_;
    
    enum class Focus {
        Bookmarks,
        LogViewport
    } currentFocus_ = Focus::LogViewport;

    IKeyboardInput& currentFocusInput();
    bool askForExit();
    void gotoLine();
    void grep();
    void search();
    void highlight();
    void selectPane();
    void showHelp();
    void bookmark();
    void toggleBookmarkPanel();
    void switchFocus();
    void updateViewportWindowPosition();
};
