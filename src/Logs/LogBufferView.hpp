#pragma once

#include "LogBuffer.hpp"
#include <functional>
#include <regex>

class LogBufferView
{
public:
    using iterator = std::vector<LineId>::const_iterator;
    using Progress = float;
    using GrepObservator = std::function<void(const Progress& progress)>;

    LogBufferView(const LogBuffer& buffer);
    LogBufferView(
        const LogBufferView& other,
        std::regex &reg,
        GrepObservator progress = nullptr);

    LogBufferView::iterator cbegin() const;
    LogBufferView::iterator cend() const;
    std::size_t size() const;
    const std::vector<LineId>& lines() const;
    std::string value(const iterator it) const; // TODO: const std::string& ?
    iterator findClosestTo(LineId id) const;

private:
    const LogBuffer& buffer_;
    std::vector<LineId> lines_;
};
