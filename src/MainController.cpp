#include "MainController.hpp"
#include "QuestionWindowController.hpp"
#include "InputWindowController.hpp"
#include "PanesWindowController.hpp"
#include "ProgressWindow.hpp"
#include "Help.hpp"
#include <ncurses.h>
#include <functional>

MainController::MainController(Display& display, const LogBuffer& logBuffer, Keyboard &keyboard)
    : display_(display)
    , logBuffer_(logBuffer)
    , panes_(std::make_unique<Pane>(logBuffer_.name(), logBuffer_))
    , keyboard_(keyboard)
    , logViewportWindow_(std::make_unique<LogViewportWindow>(display_, panes_.current().buffer()))
    , logViewportController_(std::make_unique<LogViewportController>(*logViewportWindow_.get()))
    , statusBar_(display, *this)
{
    updateViewportWindowPlacement();
}

bool MainController::parseKey(const int key, Keyboard &keyboard)
{
    switch (key)
    {
    case 'Q':
        return false;
    case KEY_ESCAPE:
        if (askForExit())
            return false;
        break;
    case 'j':
    case 'G':
        gotoLine();
        break;
    case 'g':
        grep();
        break;
    case 's':
    case 'f':
        search();
        break;
    case 'h':
        highlight();
        break;
    case 'm':
        bookmark();
        break;
    case 't':
        toggleBookmarkPanel();
        break;
    case 'b':
        selectPane();
        break;
    case '?':
        showHelp();
        break;
    case '\t':
        if (!bookmarksController_ && bookmarks().size() > 0) {
            toggleBookmarkPanel();
        }
        switchFocus();
        break;
    default:
        auto retval = currentFocusInput().parseKey(key, keyboard);
        updateBookmarkAndViewport();
        return retval;
    }
    return true;
}

bool MainController::askForExit()
{
    QuestionWindow question{
        display_,
            "Do you want to exit the program?",
            {"Yes","No"}};
    QuestionWindowController controller{question};
    keyboard_.parseKeys(controller);

    if (controller.accepted())
    {
        return (*question.selected() == "Yes");
    }
    return false;
}

void MainController::bookmark()
{
    if (!logViewportWindow_->cursor())
    {
        QuestionWindow question{
            display_,
                "No line is selected, unable to create bookmark.",
                {"Ok"}};
        QuestionWindowController controller{question};
        keyboard_.parseKeys(controller);
        return;
    }

    const std::string defaultName = "line " + std::to_string(**logViewportWindow_->cursor() + 1);
    InputWindow inputWindow{display_, "Bookmark name:", defaultName, 30};
    InputWindowController controller{inputWindow};
    keyboard_.parseKeys(controller);

    if (controller.accepted())
    {
        bookmarks_.emplace_back(inputWindow.content(), **logWindow().cursor());
        updateBookmarkFromViewport();
    }
}

void MainController::updateBookmarkAndViewport()
{
    switch (currentFocus_) {
        case Focus::LogViewport:
            updateBookmarkFromViewport();
            break;
        case Focus::Bookmarks:
            updateViewportFromBookmark();
            break;
    }
}

void MainController::updateBookmarkFromViewport()
{
    if (!bookmarksController_) {
        return; // TODO: needed?
    }
    bookmarksController_->selectClosest(**logWindow().cursor());
}

void MainController::updateViewportFromBookmark()
{
    if (!bookmarksController_) {
        return; // TODO: needed?
    }
    
    if (bookmarksWindow_->selected() != bookmarksWindow_->items().cend())
    {
        const auto selectedBookmark = bookmarksWindow_->selected();
        logViewportWindow_->goTo(pane().buffer().findClosestTo(selectedBookmark->id));
    }
}

void MainController::showHelp()
{
    QuestionWindow question{display_, HelpText, {"Return"}};
    QuestionWindowController controller{question};
    keyboard_.parseKeys(controller);
}


void MainController::selectPane()
{
    PanesWindowController controller{*this};
    keyboard_.parseKeys(controller);

    if (controller.accepted())
    {
        setActive(controller.selected());
    }
}

void MainController::gotoLine()
{
    InputWindow inputWindow{display_, "Line number:", "", 30};
    InputWindowController controller{inputWindow};
    keyboard_.parseKeys(controller);

    if (controller.accepted())
    {
        const auto lineId = std::atoi(inputWindow.content().c_str()) - 1;
        const auto targetIter = pane().buffer().findClosestTo(lineId);
        logViewportWindow_->goTo(targetIter);
    }
}

void MainController::search()
{
    InputWindow inputWindow{display_, "Search for:", lastSearch_, 500, 100};
    InputWindowController controller{inputWindow};
    keyboard_.parseKeys(controller);
    const auto it = logViewportWindow_->find(inputWindow.content());
    logViewportWindow_->goTo(it);
    logViewportWindow_->highlighting(inputWindow.content());

    lastSearch_ = inputWindow.content();
}

void MainController::grep()
{
    auto inputWindow = std::make_unique<InputWindow>(display_, "Grep:", "", 500, 100);
    InputWindowController controller{(*inputWindow.get())};
    keyboard_.parseKeys(controller);

    if (controller.accepted())
    {
        const auto regularExpr = inputWindow->content();
        inputWindow.reset();

        std::regex reg{regularExpr,
                std::regex_constants::basic | std::regex_constants::icase};

        ProgressWindow progress{display_, "Grep in progress"};
        display_.refresh();

        auto progressFunc = std::bind(&ProgressWindow::update, &progress, std::placeholders::_1);
        LogBufferView bufferViewGrep{pane().buffer(), reg, progressFunc};

        auto& newPane = panes_.addPane(std::make_unique<Pane>(regularExpr, bufferViewGrep));
        setActive(newPane);
    }
}

void MainController::highlight()
{
    InputWindow inputWindow{display_, "Highlight:", lastHighlight_, 500, 100};
    InputWindowController controller{inputWindow};
    keyboard_.parseKeys(controller);
    logViewportWindow_->highlighting(inputWindow.content());

    lastHighlight_ = inputWindow.content();
}

void MainController::setActive(Pane &pane)
{
    auto newLogViewportWindow = std::make_unique<LogViewportWindow>(display_, pane.buffer());
    auto newlogViewportController = std::make_unique<LogViewportController>(*newLogViewportWindow.get());
   
    const auto currentCursor = logViewportWindow_->cursor();
    if (currentCursor)
    {
        newLogViewportWindow->goTo(newLogViewportWindow->buffer().findClosestTo((**currentCursor)));
    }
    
    panes_.setCurrent(pane);
    logViewportWindow_ = std::move(newLogViewportWindow);
    logViewportController_ = std::move(newlogViewportController);
    updateViewportWindowPlacement();
}

void MainController::toggleBookmarkPanel()
{
    if (!bookmarksController_)
    {
        bookmarksWindow_ = std::make_unique<BookmarksWindow>(*this, bookmarks(), bookmarks().begin());
        bookmarksController_ = std::make_unique<BookmarksWindowController>(*bookmarksWindow_.get());
    }
    else
    {
        bookmarksController_ = nullptr;
        bookmarksWindow_ = nullptr;
    }
    updateViewportWindowPlacement();
}

void MainController::updateViewportWindowPlacement()
{
    if (bookmarksController_)
    {
        logViewportWindow_->resize(display_.width() - BookmarksWindow::WindowWidth, display_.height());
        logViewportWindow_->moveWindow(BookmarksWindow::WindowWidth, 0);
    }
    else
    {
        logViewportWindow_->resize(display_.width(), display_.height());
        logViewportWindow_->moveWindow(0, 0);
    }
    logViewportWindow_->refresh();
}

void MainController::switchFocus()
{
    if (bookmarksController_)
    {
        if (currentFocus_ != Focus::Bookmarks)
        {
            currentFocus_ = Focus::Bookmarks;
            bookmarksWindow_->setFocus(true);
            logViewportWindow_->setFocus(false);
            return;
        }
        currentFocus_ = Focus::LogViewport;
        bookmarksWindow_->setFocus(false);
        logViewportWindow_->setFocus(true);
    }
}

IKeyboardInput& MainController::currentFocusInput()
{
    switch (currentFocus_)
    {
        case Focus::Bookmarks:
            if (bookmarksController_)
                return *bookmarksController_.get();
        case Focus::LogViewport:
            return *logViewportController_.get();
    }
}

Pane& MainController::pane()
{
    return panes_.current();
}

PanesContainer& MainController::panes()
{
    return panes_;
}

LogViewportWindow& MainController::logWindow()
{
    return *logViewportWindow_;
}

Display& MainController::display()
{
    return display_;
}

Keyboard& MainController::keyboard()
{
    return keyboard_;
}

BookmarksContainer& MainController::bookmarks()
{
    return bookmarks_;
}
