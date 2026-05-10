#include "PanesWindowController.hpp"
#include "InputWindowController.hpp"
#include "QuestionWindow.hpp"
#include "QuestionWindowController.hpp"
#include <ncurses.h>
#include <iostream>
#include <iomanip>
#include <sstream>


namespace
{
constexpr auto NameWidth = 53;
constexpr auto WindowWidth = 60;

std::string formatRow(const std::string& name, const std::string &linesCount)
{
    std::stringstream lines;
    lines << "  " << linesCount;
    const auto maxWidth = 3 + NameWidth - lines.str().size();

    std::stringstream label;
    label << std::setw(maxWidth) << std::left
          << name.substr(0, maxWidth)
          << lines.str();

    return label.str();
}

}

PanesWindowController::PanesWindowController(IAppContext &context)
    : context_(context)
{
    listView_ = createView();
}

bool PanesWindowController::parseKey(const int key, Keyboard& keyboard)
{
    switch (key)
    {
    case KEY_UP:
        listView_->up();
        break;
    case KEY_DOWN:
        listView_->down();
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
    const auto index = std::distance(listView_->items().begin(), listView_->selected());
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
        listView_ = createView();
    }
}

void PanesWindowController::rename()
{
    auto &selectedPane = selected();

    InputWindow inputWindow{context_.display(), "New name:", selectedPane.name(), NameWidth, NameWidth + 4};
    InputWindowController controller{inputWindow};
    context_.keyboard().parseKeys(controller);

    if (controller.accepted())
    {
        selectedPane.setName(inputWindow.content());
        listView_ = createView();
    }
}

std::unique_ptr<ListWindow> PanesWindowController::createView() const
{
    ListWindow::Items items;
    std::size_t selectedIndex = 0;
    for (const auto &item : context_.panes().allPanes())
    {
        items.push_back(
            formatRow(
                item->name(),
                std::to_string(item->buffer().size())));

        if (item.get() == &context_.panes().current())
            selectedIndex = items.size() - 1;
    }

    return std::make_unique<ListWindow>(
        context_,
        "",
        formatRow("Buffers:","Lines: "),
        "[r] rename, [d] delete [Enter] select [ESC] cancel",
        items,
        selectedIndex,
        [](int w, int h){return std::make_pair(WindowWidth, h-5);});
}
