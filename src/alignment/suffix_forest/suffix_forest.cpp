//
// Created by ichigo on 18.10.2020.
//

#include "suffix_forest.hpp"
#include <iostream>
#include <functional>

SuffixForest::SuffixForest(FastaSequence *fastaSequence)
        : sequence(fastaSequence), suffixTrees(new std::vector<SuffixTree *>()) {}

SuffixForest::~SuffixForest() {
    delete sequence;
    for (auto &suffixTree : *suffixTrees) {
        delete suffixTree;
    }
    delete suffixTrees;
}

void SuffixForest::build() {
    // Doesn't work with sequences with length less than
    // SUFFIX_TREE_STRING_MIN_LENGTH * 2
    // TODO remove debug prints
    int sequenceLength = this->sequence->source->length(),
            suffixTreesAmount = (sequenceLength - SUFFIX_TREE_STRING_OVERLAY_LENGTH) /
                                (SUFFIX_TREE_STRING_MIN_LENGTH - SUFFIX_TREE_STRING_OVERLAY_LENGTH),
            sliceLength = (sequenceLength + (suffixTreesAmount - 1) * SUFFIX_TREE_STRING_OVERLAY_LENGTH)
                          / suffixTreesAmount;
//    std::cout << "Total suffix forest string length : " << sequenceLength << std::endl;
    std::cout << "-- Going to build " << suffixTreesAmount << " suffix trees --" << std::endl;
    for (int sliceEntry = 0, i = 1; i <= suffixTreesAmount;
         sliceEntry += sliceLength - SUFFIX_TREE_STRING_OVERLAY_LENGTH, i++) {
        int currentSliceLength = i == suffixTreesAmount ? sequenceLength - sliceEntry : sliceLength;
//        std::cout << "| Suffix tree properties | " << std::endl
//                  << "+---- globalSequenceEntryIndex : " << sliceEntry << std::endl
//                  << "+---- sequenceSliceLength: " << currentSliceLength << std::endl;
        std::string sequenceSlice = this->sequence->source->substr(sliceEntry, currentSliceLength);
        auto *suffixTree = new SuffixTree(sequenceSlice, sliceEntry);
        this->suffixTrees->push_back(suffixTree);
        std::cout << "Built suffix tree num. " << i << ", left to build: " << (suffixTreesAmount - i) << std::endl;
    }
}

void supplement(std::vector<int> *, std::vector<int> *);

std::vector<int> *SuffixForest::getEntryIndexes(const std::string &substring) {
    auto *globalEntries = new std::vector<int>();
    for (auto suffixTree : *this->suffixTrees) {
        std::vector<int> *treeEntries = suffixTree->getEntryIndexes(substring);
        supplement(globalEntries, treeEntries);
    }
    std::sort(globalEntries->begin(), globalEntries->end());
    return globalEntries;
}

const std::string &SuffixForest::getSequenceDescription() {
    return *(this->sequence->description);
}

bool contains(std::vector<int> *, int);

void supplement(std::vector<int> *main, std::vector<int> *additional) {
    for (int element : *additional) {
        if (!contains(main, element)) {
            main->push_back(element);
        }
    }
}

bool contains(std::vector<int> *v, int val) {
    std::function<bool(int)> equalToVal = [&val](int element) { return element == val; };
    return std::any_of(v->begin(), v->end(), equalToVal);
}
