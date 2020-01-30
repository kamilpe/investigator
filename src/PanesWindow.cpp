#include "PanesWindow.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

namespace
{


std::string formatRow(const std::string& name, const std::string &linesCount)
{
    std::stringstream lines;
    lines << "  " << linesCount;
    const auto maxWidth = 3 + PanesWindow::NameWidth - lines.str().size();

    std::stringstream label;
    label << std::setw(static_cast<unsigned int>(maxWidth)) << std::left
          << name.substr(0, maxWidth)
          << lines.str();

    return label.str();
}

}

PanesWindow::PanesWindow(
                         IAppContext &context,
                         const Items &items,
                         const typename Items::const_iterator selected) : PanesWindowList(context, items, selected)
{
    resize(context.display().width(), context.display().height());
}


void PanesWindow::resize(const int w, const int h)
{
    Window::resize(WindowWidth, h-5);
    centerWindow();
}

void PanesWindow::printLine(int x, int y, bool selected, typename Items::const_iterator &line) const
{
    setColor(selected ? Display::Pair::Highlight : Display::Pair::Dialog);
    print(x, y, formatRow((*line)->name(), std::to_string((*line)->buffer().size())));
}

void PanesWindow::draw()
{
    fillWith(Display::Pair::Dialog);
    bordering();
    
    int xpos = 2;
    int ypos = 2;
    
    const auto header = formatRow("Buffers:","Lines: ");
    print(xpos, ypos, header);
    ypos+=1;
    lineh(xpos, ypos, static_cast<int>(header.size()));
    ypos+=1;
    
    drawList(xpos, ypos, height() - 4);
    setColor(Display::Pair::Dialog);
    
    print (2, height() - 2,  "[r] rename | [d] delete | [Enter] select | [ESC] cancel");
}

