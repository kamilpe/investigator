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

MainController::MainController(Display& display, const LogBuffer& logBuffer)
    : display_(display)
    , logBuffer_(logBuffer)
    , panes_(std::make_unique<Pane>(logBuffer_.name(), logBuffer_))
    , window_(std::make_unique<LogViewport>(display_, panes_.current().buffer()))
    , controller_(std::make_unique<LogViewportController>(*window_.get()))
    , statusBar_(display, panes_)
{
}

bool MainController::parseKey(const int key, Keyboard& keyboard)
{
    switch (key)
    {
    case 'Q':
        return false;
    case KEY_ESCAPE:
        if (askForExit(keyboard))
            return false;
        break;
    case 'j':
    case 'G':
        gotoLine(keyboard);
        break;
    case 'g':
        grep(keyboard);
        break;
    case 'f':
        search(keyboard);
        break;
    case 'h':
        highlight(keyboard);
        break;
    case ' ':
        bookmark(keyboard);
        break;
    case '\t':
        showPanes(keyboard);
        break;
    case '?':
        showHelp(keyboard);
        break;
    default: return controller_->parseKey(key, keyboard);
    }
    return true;
}

bool MainController::askForExit(Keyboard &keyboard)
{
    QuestionWindow question{
        display_,
            "Do you want to exit the program?",
            {"Yes","No"}};
    QuestionWindowController controller{question};
    keyboard.parseKeys(controller);

    if (controller.accepted())
    {
        return (*question.selected() == "Yes");
    }
    return false;
}

void MainController::bookmark(Keyboard &keyboard)
{
    if (!window_->cursor())
    {
        QuestionWindow question{
            display_,
                "No line is selected, unable to create bookmark.",
                {"Ok"}};
        QuestionWindowController controller{question};
        keyboard.parseKeys(controller);
        return;
    }

    const std::string defaultName = "line " + std::to_string(**window_->cursor());
    InputWindow inputWindow{display_, "Bookmark name:", defaultName, 30};
    InputWindowController controller{inputWindow};
    keyboard.parseKeys(controller);

    if (controller.accepted())
    {

    }
}

void MainController::showHelp(Keyboard &keyboard)
{
    QuestionWindow question{display_, HelpText, {"Return"}};
    QuestionWindowController controller{question};
    keyboard.parseKeys(controller);
}


void MainController::showPanes(Keyboard &keyboard)
{
    PanesWindowController controller{
        display_,
        keyboard,
        panes_,
        std::bind(&MainController::setActive, this, std::placeholders::_1)};
    keyboard.parseKeys(controller);

    if (controller.accepted())
    {
        setActive(controller.selected());
    }
}

void MainController::gotoLine(Keyboard &keyboard)
{
    InputWindow inputWindow{display_, "Line number:", "", 30};
    InputWindowController controller{inputWindow};
    keyboard.parseKeys(controller);

    if (controller.accepted())
    {
        const auto lineId = std::atoi(inputWindow.content().c_str()) - 1;
        const auto targetIter = pane().buffer().findClosestTo(lineId);
        window_->goTo(targetIter);
    }
}

void MainController::search(Keyboard &keyboard)
{
    InputWindow inputWindow{display_, "Search for:", lastSearch_, 500, 100};
    InputWindowController controller{inputWindow};
    keyboard.parseKeys(controller);
    const auto it = window_->find(inputWindow.content());
    window_->goTo(it);
    window_->highlight(inputWindow.content());

    lastSearch_ = inputWindow.content();
}

void MainController::grep(Keyboard &keyboard)
{
    auto inputWindow = std::make_unique<InputWindow>(display_, "Grep:", "", 500, 100);
    InputWindowController controller{(*inputWindow.get())};
    keyboard.parseKeys(controller);

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

void MainController::highlight(Keyboard &keyboard)
{
    InputWindow inputWindow{display_, "Highlight:", lastHighlight_, 500, 100};
    InputWindowController controller{inputWindow};
    keyboard.parseKeys(controller);
    window_->highlight(inputWindow.content());

    lastHighlight_ = inputWindow.content();
}

void MainController::setActive(Pane &pane)
{
    const auto currentCursor = window_->cursor();

    panes_.setCurrent(pane);
    window_ = std::make_unique<LogViewport>(display_, panes_.current().buffer());
    controller_ = std::make_unique<LogViewportController>(*window_.get());

    if (currentCursor)
    {
        window_->goTo(
            pane.buffer().findClosestTo(
                (**currentCursor)));
    }
}

Pane& MainController::pane()
{
    return panes_.current();
}
