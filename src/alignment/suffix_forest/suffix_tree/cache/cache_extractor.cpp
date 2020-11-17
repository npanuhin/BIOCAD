#include "cache_extractor.hpp"
#include "cache_file_format.hpp"
#include <string> 
#include <iostream> 
#include <filesystem>
#include <fstream>

bool isDigit(std::string);

SuffixTreeCacheExtractor::SuffixTreeCacheExtractor(std::string cacheDirPath) 
    : subtrees(new std::vector<SuffixSubtreeCache *>()) {
    for (auto & p : std::filesystem::directory_iterator(cacheDirPath)) {
        if (isDigit(p.path().stem().string())) {
            SuffixSubtreeCache * newSubtreeCache = new SuffixSubtreeCache(cacheDirPath, std::stoi(p.path().filename()));
            this->subtrees->push_back(newSubtreeCache);
        }
    }
};

SuffixSubtreeCache::SuffixSubtreeCache(std::string cacheDirPath, int serialCachePartNum)
    : relativeFilePath(cacheDirPath + "/" + std::to_string(serialCachePartNum) + "." + CACHE_FILES_EXTENSION) {};

SuffixTree * SuffixSubtreeCache::translateSuffixTreeMask() {
    std::ofstream cacheFile(this->relativeFilePath);
    std::cout << "Is open : " << cacheFile.is_open() << std::endl;

    

    cacheFile.close();
    return nullptr;
}

bool isDigit(std::string s) {
    for (char c : s) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}
