#include "Bookmark.hpp"
#include <algorithm>

void BookmarksContainer::insert(const Bookmark& bookmark)
{
    auto it = std::lower_bound(begin(), end(), bookmark.id,
        [](const Bookmark& b, LineId id) { return b.id < id; });
    std::vector<Bookmark>::insert(it, bookmark);
}
