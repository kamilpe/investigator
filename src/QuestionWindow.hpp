#pragma once

#include "CursesToolkit/Window.hpp"

class QuestionWindow : public Window
{
public:
    using Answers = std::vector<std::string>;

    QuestionWindow(
        Display &display,
        const std::string question,
        const Answers answers,
        const int selected = 0);
    ~QuestionWindow();

    void draw() override;
    void resize(const int w, const int h) override;
    void left();
    void right();
    const Answers& answers();
    Answers::const_iterator selected();

private:
    Display& display_;
    const std::string question_;
    const Answers answers_;
    Answers::const_iterator selected_;

    const int textWidth_;
    const int textHeight_;

    void drawText();
    void drawButtons();
};
