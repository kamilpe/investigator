#include "PanesWindowController.hpp"
#include "InputWindowController.hpp"
#include "QuestionWindowController.hpp"
#include <ncurses.h>

PanesWindowController::PanesWindowController(IAppContext &context)
    : context_(context)
{
    createView();
}

bool PanesWindowController::parseKey(const int key, Keyboard& keyboard)
{
    switch (key)
    {
    case KEY_UP:
        listWindow_->up();
        break;
    case KEY_DOWN:
        listWindow_->down();
        break;
    case KEY_ACCEPT:
        accept_ = true;
        return false;
    case '\t':
    case KEY_ESCAPE:
        accept_ = false;
        return false;
    case 'R':
    case 'r':
        rename();
        break;
    case 'D':
    case 'd':
        remove();
        break;
    }
    return true;
}


bool PanesWindowController::accepted() const
{
    return accept_;
}

Pane& PanesWindowController::selected()
{
    const auto index = std::distance(listWindow_->items().begin(), listWindow_->selected());
    return (*context_.panes().allPanes().at(index));
}

void PanesWindowController::remove()
{
    if (context_.panes().allPanes().size() == 1)
        return;

    QuestionWindow question{
        context_.display(),
            "Are you sure that you want to remove:\n* " + selected().name() + " *",
            {"Yes","Cancel"}};
    QuestionWindowController controller{question};
    context_.keyboard().parseKeys(controller);

    if (controller.accepted() && *question.selected() == "Yes")
    {
        context_.panes().remove(selected());
        context_.setActive(context_.panes().current());
        createView();
    }
}

void PanesWindowController::rename()
{
    auto &selectedPane = selected();

    InputWindow inputWindow{
        context_.display(),
        "New name:",
        selectedPane.name(),
        PanesWindow::NameWidth,
        PanesWindow::NameWidth + 4};
    inputWindow.setCursor(inputWindow.content().end());

    InputWindowController controller{inputWindow};
    context_.keyboard().parseKeys(controller);

    if (controller.accepted())
    {
        selectedPane.setName(inputWindow.content());
        createView();
    }
}

void PanesWindowController::createView()
{
    listWindow_ = std::make_unique<PanesWindow>(
        context_, context_.panes().allPanes(), context_.panes().currentIterator());
}
