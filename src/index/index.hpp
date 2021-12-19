#ifndef INDEX_HPP
#define INDEX_HPP

#include <set>

namespace Pekar
{
    template<typename KeyType, typename ItemType>
    class Index
    {
    public:
        virtual ~Index() {}
        virtual std::set<KeyType> load(const std::set<ItemType>& items) = 0;
        virtual KeyType add(const ItemType& item) = 0;
        virtual void remove(const KeyType& key) = 0;
        virtual ItemType& find(const KeyType& key) = 0;
    };
}

#endif