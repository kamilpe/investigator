#include "Pane.hpp"

Pane::Pane(const std::string& name, const LogBuffer& logBuffer)
    : name_(name)
    , buffer_(logBuffer)
{
}

Pane::Pane(const std::string& name, const LogBufferView& logBufferView)
    : name_(name)
    , buffer_(logBufferView)
{
}

const std::string& Pane::name() const
{
    return name_;
}

void Pane::setName(const std::string &name)
{
    name_ = name;
}

LogBufferView& Pane::buffer()
{
    return buffer_;
}
