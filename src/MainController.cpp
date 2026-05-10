#include "MainController.hpp"
#include "QuestionWindowController.hpp"
#include "InputWindowController.hpp"
#include "PanesWindowController.hpp"
#include "ProgressWindow.hpp"
#include "Help.hpp"
#include <ncurses.h>
#include <functional>

namespace
{

}

MainController::MainController(Display& display, const LogBuffer& logBuffer, Keyboard &keyboard)
    : display_(display)
    , logBuffer_(logBuffer)
    , panes_(std::make_unique<Pane>(logBuffer_.name(), logBuffer_))
    , keyboard_(keyboard)
    , logViewportWindow_(std::make_unique<LogViewportWindow>(display_, panes_.current().buffer()))
    , logViewportController_(std::make_unique<LogViewportController>(*logViewportWindow_.get()))
    , statusBar_(display, *this)
    , currentFocusInput_(logViewportController_.get())
{
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
    case ' ':
        bookmark();
        break;
    case 'b':
        switchBookmarkPanel();
        break;
    case '\t':
        showPanes();
        break;
    case '?':
        showHelp();
        break;
    case KEY_LEFT:
    case KEY_RIGHT:
        switchFocus();
        break;
    default: return currentFocusInput_->parseKey(key, keyboard);
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
        bookmarks_.emplace_back(inputWindow.content(), **window().cursor());
        /*if (!bookmarksController_)
            switchBookmarkPanel();*/
    }
}

void MainController::showHelp()
{
    QuestionWindow question{display_, HelpText, {"Return"}};
    QuestionWindowController controller{question};
    keyboard_.parseKeys(controller);
}


void MainController::showPanes()
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
    logViewportWindow_->highlight(inputWindow.content());

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
    logViewportWindow_->highlight(inputWindow.content());

    lastHighlight_ = inputWindow.content();
}

void MainController::setActive(Pane &pane)
{
    const auto currentCursor = logViewportWindow_->cursor();

    panes_.setCurrent(pane);
    logViewportWindow_ = std::make_unique<LogViewportWindow>(display_, panes_.current().buffer());
    logViewportController_ = std::make_unique<LogViewportController>(*logViewportWindow_.get());

    if (currentCursor)
    {
        logViewportWindow_->goTo(
            pane.buffer().findClosestTo(
                (**currentCursor)));
    }
}

void MainController::switchBookmarkPanel()
{
    if (!bookmarksController_)
    {
        bookmarksController_ = std::make_unique<BookmarksWindowController>(*this);
        logViewportWindow_ = std::make_unique<LogViewportWindow>(
            display_,
            panes_.current().buffer(),
            BookmarksWindow::WindowWidth);
        logViewportController_ = std::make_unique<LogViewportController>(*logViewportWindow_.get());
    }
    else
    {
        bookmarksController_ = nullptr;
        logViewportWindow_ = std::make_unique<LogViewportWindow>(display_, panes_.current().buffer());
        logViewportController_ = std::make_unique<LogViewportController>(*logViewportWindow_.get());
    }
    switchFocus();
}

void MainController::switchFocus()
{
    if (bookmarksController_)
    {
        if (currentFocusInput_ != bookmarksController_.get())
        {
            currentFocusInput_ = bookmarksController_.get();
            bookmarksController_->setFocus(true);
            logViewportWindow_->setFocus(false);
            return;
        }
    }
    currentFocusInput_ = logViewportController_.get();
    bookmarksController_->setFocus(false);
    logViewportWindow_->setFocus(true);
}

Pane& MainController::pane()
{
    return panes_.current();
}

PanesContainer& MainController::panes()
{
    return panes_;
}

LogViewportWindow& MainController::window()
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
