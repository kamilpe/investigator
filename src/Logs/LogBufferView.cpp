#include "LogBufferView.hpp"

#include <algorithm>
#include <iostream>

LogBufferView::LogBufferView(const LogBuffer& buffer)
    : buffer_(buffer), lines_(buffer.linesCount())
{
    lines_.reserve(buffer.linesCount());
    std::generate(lines_.begin(), lines_.end(), [i=0]() mutable {return i++;});
}

LogBufferView::LogBufferView(
    const LogBufferView& other,
    std::regex &reg,
    LogBufferView::GrepObservator progress)
    : buffer_(other.buffer_)
{
    decltype(other.size()) index = 0;
    for (auto it = other.cbegin(); it != other.cend(); ++it)
    {
        if (progress)
        {
            ++index;
            progress(static_cast<Progress>(index) / static_cast<Progress>(other.size()));
        }

        if (std::regex_search(other.value(it), reg))
        {
             lines_.push_back(*it);
        }
    }
}

LogBufferView::iterator LogBufferView::cbegin() const
{
    return lines_.cbegin();
}

LogBufferView::iterator LogBufferView::cend() const
{
    return lines_.cend();
}

std::size_t LogBufferView::size() const
{
    return lines_.size();
}

const std::vector<int>& LogBufferView::lines() const
{
    return lines_;
}

std::string LogBufferView::value(const iterator it) const
{
    return buffer_.line(*it);
}

LogBufferView::iterator LogBufferView::findClosestTo(int id) const
{
    if (id < 0)
    {
        return lines_.begin();
    }
    if (!lines_.empty() && id > lines_.back())
    {
        return lines_.cend() - 1;
    }

    auto it = std::find(lines_.begin(), lines_.end(), id);
    if (it != lines_.end())
    {
        return it;
    }
    
    it = std::lower_bound(lines_.begin(), lines_.end(), id);
    if (it == lines_.end())
    {
        return lines_.begin();
    }
    return it;
}
