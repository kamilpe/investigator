#pragma once

#include "LogViewportWindow.hpp"
#include "LogViewportController.hpp"
#include "Logs/LogBufferView.hpp"
#include "Toolkit/Display.hpp"

class Pane
{
public:
    Pane(const std::string&, const LogBuffer&);
    Pane(const std::string&, const LogBufferView&);
    Pane(const Pane&) = delete;

    const std::string& name() const;
    void setName(const std::string &name);
    LogBufferView& buffer();

private:
    std::string name_;
    LogBufferView buffer_;
};
