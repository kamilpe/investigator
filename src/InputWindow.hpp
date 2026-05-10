#pragma once

#include "Toolkit/Window.hpp"

class InputWindow : public Window
{
public:
    InputWindow(
        Display& display,
        const std::string& title,
        const std::string& content,
        const unsigned maxlen=0,
        const unsigned width=0);
    ~InputWindow();

    void draw() override;
    void resize(const int w, const int h) override;

    void put(const char c);
    void left();
    void right();
    void del();
    void backspace();
    const std::string& content() const;
    void setCursor(const std::string::const_iterator& cursor);

private:
    Display& display_;
    const std::string title_;
    std::string content_;
    const unsigned maxlen_;
    const int width_;
    std::string::iterator cursor_;
    bool removeAtFirst_;
};
