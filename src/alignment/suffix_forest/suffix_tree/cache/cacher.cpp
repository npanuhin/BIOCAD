#include "cacher.hpp"
#include "cache_file_format.hpp"

#include <iostream>
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <functional>

void createDirectoryIfNotExist(std::string);

std::ofstream createCacheFileStream(std::string, int);

void writeHeader(std::ofstream &, std::string const &, std::string const &);

void writeTextRepr(std::ofstream &, SuffixTreeBuilder *);

void SuffixTreeCacher::cache(SuffixTree *suffixTree, std::string globalSampleName, int cachePartSerialNumber) {
    createDirectoryIfNotExist(globalSampleName);
    std::ofstream cacheFileStream = createCacheFileStream(globalSampleName, cachePartSerialNumber);
    writeHeader(cacheFileStream, suffixTree->builder->description, suffixTree->builder->suffixTreeString);
    writeTextRepr(cacheFileStream, suffixTree->builder);
    cacheFileStream.close();
}

void createDirectoryIfNotExist(std::string cacheSubdir) {
    std::string const cacheDir = CACHE_DIR + std::string("/") + cacheSubdir;
    if (mkdir(cacheDir.c_str(), 0777) == -1) {
        std::cerr << "Error :  " << strerror(errno) << std::endl;
    } else {
        std::cout << "Directory created" << std::endl;
    }
}

std::ofstream createCacheFileStream(std::string cacheSubdir, int cachePartSerialNumber) {
    std::string const filePath = CACHE_DIR + std::string("/") + cacheSubdir + std::string("/") +
                                 std::to_string(cachePartSerialNumber) + std::string(".") + CACHE_FILES_EXTENSION;
    std::ofstream outfile(filePath);
    return outfile;
}

void writeHeader(std::ofstream &fileStream, std::string const &description, std::string const &sequenceSource) {
    fileStream << description << std::endl;
    fileStream << sequenceSource << std::endl;
}

std::string getFormattedVertexTextRepr(SuffixTreeVertex *);

void writeTextRepr(std::ofstream &fileStream, SuffixTreeBuilder *builder) {
    std::function<void(SuffixTreeVertex *)> recursiveTreeWritingToFile;
    recursiveTreeWritingToFile = [&fileStream, &recursiveTreeWritingToFile](SuffixTreeVertex *vertex) {
        std::string vertexTextRepr = getFormattedVertexTextRepr(vertex);
        fileStream << vertexTextRepr;
        for (int i = 0; i < vertex->getChildren()->size(); ++i) {
            SuffixTreeVertex *child = vertex->getChildren()->at(i);
            recursiveTreeWritingToFile(child);
        }
    };
    recursiveTreeWritingToFile(builder->getRoot());
    fileStream << std::endl;
}

std::string getFormattedVertexTextRepr(SuffixTreeVertex *vertex) {
    std::pair<int, int> entryIndexAndSubstringLength = vertex->getInfo();
    int childrenAmount = vertex->getChildren()->size();
    return std::to_string(entryIndexAndSubstringLength.first) + "," +
           std::to_string(entryIndexAndSubstringLength.second) + "," +
           std::to_string(childrenAmount) + ";";
}
