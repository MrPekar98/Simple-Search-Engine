#ifndef INDEX_HPP
#define INDEX_HPP

#include <set>
#include <map>

namespace Pekar
{
    template<typename KeyType, typename ItemType>
    class Index
    {
    public:
        virtual ~Index() {}
        virtual void load(const std::set<ItemType>& items) = 0;
        virtual void add(KeyType&& key, ItemType&& item) = 0;
        virtual void remove(KeyType&& key) = 0;
        virtual ItemType find(KeyType&& key) = 0;
    };
}

#endif