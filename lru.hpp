#include <iostream>
#include <list>
#include <unordered_map>
#include <utility>
#include <vector>

template<class key, class value>
class LRU_Cache {
private:
    void cleanLeastUsed()
    {
        //чистим то, что наименее используемое в кэше
        while (priorityList.size() > maxElements || currentSize > maxSizeBytes) {

            auto last = priorityList.back();
            currentSize -= size_bytes(last.first, last.second);
            itersTable.erase(last.first);
            priorityList.pop_back();
        }
    };

    size_t currentSize;
    size_t maxElements;
    size_t maxSizeBytes;

    std::list<std::pair<key, value>> priorityList;

    using listIterator = typename std::list<std::pair<key, value>>::iterator;
    std::unordered_map<key, listIterator> itersTable;

public:
    LRU_Cache(size_t size, size_t sizeBytes)
        : currentSize(0)
        , maxElements(size)
        , maxSizeBytes(sizeBytes)
    {
    }

    size_t size_bytes(const key &k, const value &v)
    {
        return k.size() + v.size() * sizeof(typename value::value_type);
    }

    //закэшировать элемент
    void store(const key &k, const value &v)
    {
        size_t new_item_size = size_bytes(k, v);
        if (new_item_size > maxSizeBytes) {
            std::cout << "!STORERR!" << std::endl;
            return;
        }

        //проверяем, есть ли с таким ключом элемент
        auto iter = itersTable.find(k);
        if (iter != itersTable.end()) {
            //если есть, стираем старое значение
            currentSize -= size_bytes(iter->second->first, iter->second->second);
            priorityList.erase(iter->second);
            itersTable.erase(iter);
        }

        priorityList.push_front({ k, v });
        itersTable[k] = priorityList.begin();
        currentSize += new_item_size;

        cleanLeastUsed();

        std::cout << "!STORED!" << std::endl;
    }

    //достать элемент из кэша
    void pull(const key &k)
    {
        auto iter = itersTable.find(k);
        if (iter == itersTable.end()) {
            std::cout << "!NOEMBED!" << std::endl;
        } else {
            auto [found_key, v] = *(iter->second);
            priorityList.erase(iter->second);
            priorityList.push_front({ found_key, v });
            itersTable[k] = priorityList.begin();

            for (const auto &num : v) {
                std::cout << num << " ";
            }
            std::cout << std::endl;
        }
    }

    size_t size() const
    {
        return priorityList.size();
    }

    void clear()
    {
        priorityList.clear();
        itersTable.clear();
        currentSize = 0;
    }
};
