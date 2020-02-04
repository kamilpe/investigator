#include "Bookmark.hpp"

void BookmarksContainer::insert(const Bookmark& bookmark)
{
    for (auto it = begin(); it < end(); ++it)
    {
        if (it->id >= bookmark.id) {
            std::vector<Bookmark>::insert(it, bookmark);
            return;
        }
    }
    push_back(bookmark);
}
