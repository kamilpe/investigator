#pragma once

#include "LogLinePrinter.hpp"
#include "Logs/LogBufferView.hpp"
#include "Toolkit/Display.hpp"
#include "Toolkit/Window.hpp"

#include <regex>
#include <optional>

class LogViewport : public Window
{
public:
    LogViewport(Display& display, const LogBufferView& bufferView, const int shift = 0);
    ~LogViewport();

    void draw() override;
    void resize(const int w, const int h) override;
    void up();
    void down();
    void pageUp();
    void pageDown();
    void top();
    void bottom();
    void goTo(const LogBufferView::iterator& iter);
    std::optional<LogBufferView::iterator> cursor() const;
    LogBufferView::iterator find(const std::string& expr);
    void highlight(const std::string& expr);
    const LogBufferView& buffer() const;

private:
    const LogBufferView& bufferView_;
    const int digitsCount_;
    const int logLinePos_;
    LogLinePrinter logPrinter_;
    LogBufferView::iterator viewport_;
    LogBufferView::iterator cursor_;
    LogBufferView::iterator firstLine_;
    LogBufferView::iterator lastLine_;
    LogBufferView::iterator endLine_;
    unsigned linesOnScreen_;
    std::optional<std::regex> hlre_;

    void markCursor(const LogBufferView::iterator& it);
    int printLogLine(const int y, const LogBufferView::iterator& it);
    void printLineNum(const int y, const LogBufferView::iterator& it);
    void fillLineNumSpace(const int y);
    bool cursorIsOutsieWindow() const;
    void alignViewportToCursor(const int fromTopCount);
};
