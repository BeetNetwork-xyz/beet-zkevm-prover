#ifndef DATABASE_KV_ASSOCIATIVE_CACHE_HPP
#define DATABASE_KV_ASSOCIATIVE_CACHE_HPP
#include <vector>
#include "goldilocks_base_field.hpp"
#include <nlohmann/json.hpp>
#include <mutex>
#include "zklog.hpp"
#include "zkmax.hpp"

using namespace std;
class DatabaseKVAssociativeCache
{
    private:
        recursive_mutex mlock;

        int log2IndexesSize;
        uint32_t indexesSize;
        int log2CacheSize;
        uint32_t cacheSize;

        uint32_t *indexes;
        Goldilocks::Element *keys;
        Goldilocks::Element *values;
        uint64_t * versions;
        uint32_t currentCacheIndex; 

        uint64_t attempts;
        uint64_t hits;
        string name;

        uint64_t indexesMask;
        uint64_t cacheMask;


    public:

        DatabaseKVAssociativeCache();
        DatabaseKVAssociativeCache(int log2IndexesSize_, int log2CacheSize_, string name_);
        ~DatabaseKVAssociativeCache();

        void postConstruct(int log2IndexesSize_, int log2CacheSize_, string name_);
        void addKeyValueVersion(Goldilocks::Element (&key)[4], const vector<Goldilocks::Element> &value, uint64_t version, bool update);
        bool findKey(const Goldilocks::Element (&key)[4], vector<Goldilocks::Element> &value, bool last = true, uint64_t version=0);
        inline bool enabled() const { return (log2IndexesSize > 0); };
        inline uint32_t getCacheSize()  const { return cacheSize; };
        inline uint32_t getIndexesSize() const { return indexesSize; };

    private:
        inline bool emptyCacheSlot(uint32_t cacheIndexRaw) const { 
            return (currentCacheIndex >= cacheIndexRaw &&  currentCacheIndex - cacheIndexRaw > cacheSize) ||
            (currentCacheIndex < cacheIndexRaw && UINT32_MAX - cacheIndexRaw + currentCacheIndex > cacheSize);
         };
        void forcedInsertion(uint32_t (&usedRawCacheIndexes)[10], int &iters);
};
#endif