#include "QuestionWindow.hpp"

namespace
{
std::pair<int,int> dimensions(const std::string &text)
{
    int width = 0;
    int height = 0;
    auto lastLine = text.begin();
    for (auto i = text.begin(); i != text.end(); ++i)
    {
        if (*i == '\n')
        {
            width = std::max<int>(std::distance(lastLine, i), width);
            ++height;
            lastLine = i;
        }
    }

    if (lastLine == text.begin())
    {
        width = text.size();
    }

    return std::make_pair(width,height);
}
}

QuestionWindow::QuestionWindow(
    Display &display,
    const std::string question,
    const Answers answers,
    const int selected)
    : Window(display, 0, 0, 0, 0)
    , display_(display)
    , question_(question)
    , answers_(answers)
    , selected_(answers_.begin() + selected)
    , textWidth_(dimensions(question).first)
    , textHeight_(dimensions(question).second)
{
    resize(display_.width(), display_.height());
}

QuestionWindow::~QuestionWindow()
{
}

void QuestionWindow::resize(const int w, const int h)
{
    Window::resize(textWidth_+2,textHeight_+5);
    centerWindow();
}

void QuestionWindow::draw()
{
    fillWith(Display::Pair::Dialog);
    bordering();
    drawText();
    drawButtons();
}

void QuestionWindow::drawText()
{
    int px = 1;
    int py = 1;
    for (auto i = question_.begin(); i != question_.end(); ++i)
    {
        if (*i == '\n')
        {
            px=1;
            ++py;
            continue;
        }
        putc(px,py,*i);
        ++px;
    }
}

void QuestionWindow::drawButtons()
{
    const int y = height() - 2;
    int x = width();
    for (auto answerIt = answers_.begin(); answerIt != answers_.end(); ++answerIt)
    {
        if (answerIt == selected_)
        {
            setColor(Display::Pair::Highlight);
        }
        else
        {
            setColor(Display::Pair::Dialog);
        }
        x -= answerIt->size() + 2;
        print(x,y,*answerIt);
    }
}

void QuestionWindow::left()
{
    if (selected_+1 == answers_.end())
        return;

    ++selected_;
}

void QuestionWindow::right()
{
    if (selected_ == answers_.begin())
        return;

    --selected_;
}

const QuestionWindow::Answers& QuestionWindow::answers()
{
    return answers_;
}

QuestionWindow::Answers::const_iterator QuestionWindow::selected()
{
    return selected_;
}
