#include "Statusbar.hpp"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iterator>

Statusbar::Statusbar(
    Display &display,
    IAppContext &context)
    : Window(display, 0, display.height()-1, display.width(), 1)
    , context_(context)
    , fullStatusText(width(), ' ')
{
}

void Statusbar::resize(const int w, const int h)
{
    Window::resize(w, 1);
    moveWindow(0, h-1);
    fullStatusText = std::string(w, ' ');
    draw();
}

void Statusbar::draw()
{
    std::fill(fullStatusText.begin(), fullStatusText.end(), ' ');

    std::size_t cursorPos = 0;
    std::size_t procentage = 0;
    
    const auto linesCount = context_.pane().buffer().size();
    const auto cursor = context_.logWindow().cursor();
    if (cursor)
    {
        cursorPos = std::distance(context_.pane().buffer().cbegin(), *cursor) + 1;
        procentage = cursorPos * 100 / linesCount;
    }

    constexpr auto STATUS_BUFFER_LEN = 256;
    char statusBuffer[STATUS_BUFFER_LEN];
    std::snprintf(statusBuffer, STATUS_BUFFER_LEN,
                  "press '?' for help | line: %lu of %lu | %lu%c ",
                  cursorPos, linesCount,
                  procentage, '%');

    const auto name = context_.pane().name();
    fullStatusText.replace(0, name.length(), name);
    
    const size_t lengthOfStatusBuffer = std::strlen(statusBuffer);
    fullStatusText.replace(
        fullStatusText.length() - lengthOfStatusBuffer,
        lengthOfStatusBuffer,
        statusBuffer);

    setColor(Display::Pair::Statusbar);
    print(0,0,fullStatusText);
}
