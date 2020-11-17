#ifndef CACHE_CONFIG
#define CACHE_CONFIG

#include <fstream>
#include <string>

#define DEFAULT_SEQUENCE_DESCRIPTION "anonymous"
#define CACHE_FILES_EXTENSION "stc"
#define CACHE_DIR "./cache"

class CacheFileFormat {
public:
    CacheFileFormat(std::string &fileHeader, std::string &fileSourceSequence, std::string &fileCachedSuffixTree);

    CacheFileFormat(std::string &cacheFileLocation);

    std::string toString();

private:
    std::string &header, &sourceSequence, cachedSuffixTree;
};

#endif
