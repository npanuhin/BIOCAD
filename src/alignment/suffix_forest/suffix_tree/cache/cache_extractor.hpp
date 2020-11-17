#ifndef CACHE_EXTRACTOR
#define CACHE_EXTRACTOR

#include "../suffix_tree.hpp"
#include <string>
#include <vector>

class SuffixSubtreeCache;

class SuffixTreeCacheExtractor {
public:
    SuffixTreeCacheExtractor(std::string);
private:
    std::vector<SuffixSubtreeCache *> * subtrees;
};

class SuffixSubtreeCache {
public:
    SuffixSubtreeCache(std::string, int);

    SuffixTree * translateSuffixTreeMask();
private:
    std::string const relativeFilePath;
};

#endif
