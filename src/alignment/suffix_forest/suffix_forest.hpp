//
// Created by ichigo on 18.10.2020.
//

#ifndef GMW_SUFFIX_FOREST_HPP
#define GMW_SUFFIX_FOREST_HPP

#include <vector>

#include "../utils/fasta.hpp"
#include "suffix_tree/suffix_tree.hpp"

#define SUFFIX_TREE_STRING_MIN_LENGTH 10000
#define SUFFIX_TREE_STRING_OVERLAY_LENGTH 500

class SuffixForest {
public:
    explicit SuffixForest(FastaSequence *);

    ~SuffixForest();

    void build();

    std::vector<int> *getEntryIndexes(const std::string &);

    const std::string &getSequenceDescription();

private:
    FastaSequence *sequence;

    std::vector<SuffixTree *> *suffixTrees;
};

#endif //GMW_SUFFIX_FOREST_HPP
