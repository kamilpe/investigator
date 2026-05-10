#include "Statusbar.hpp"
#include <algorithm>
#include <cstdio>
#include <cstring>

Statusbar::Statusbar(
    Display& display,
    const PanesContainer& panes)
    : Window(display, 0, display.height()-1, display.width(), 1)
    , panes_(panes)
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

    const auto linesCount = panes_.current().buffer().size();

    std::size_t cursorPos = 0;
    std::size_t procentage = 0;
/*    const auto cursor = panes_.current().window().cursor();
    if (cursor)
    {
        cursorPos = (**cursor)+1;
        procentage = cursorPos * 100 / linesCount;
    }
*/
    constexpr auto STATUS_BUFFER_LEN = 256;
    char statusBuffer[STATUS_BUFFER_LEN];
    std::snprintf(statusBuffer, STATUS_BUFFER_LEN,
                  "press '?' for help | line: %lu of %lu | %lu%c ",
                  cursorPos, linesCount,
                  procentage, '%');

    const int lengthOfStatusBuffer = std::strlen(statusBuffer);
    fullStatusText.replace(
        fullStatusText.length() - lengthOfStatusBuffer,
        lengthOfStatusBuffer,
        statusBuffer);

    setColor(Display::Pair::Statusbar);
    print(0,0,fullStatusText);
}
