#include "suffix_tree.hpp"
#include "builder/subsidiary/subsidiary.hpp"

#include <string>
#include <functional>
#include <algorithm>


SuffixTree::SuffixTree(std::string &sequence, int globalSequenceEntryIndex)
        : globalSequenceEntryIndex(globalSequenceEntryIndex) {
    this->builder = new SuffixTreeBuilder(sequence);
    this->builder->build();
}

SuffixTree::~SuffixTree() {
    delete this->builder;
}

bool doSecondStringOverlays(const std::string &, const std::string &);

void getRecursiveDownAndCountLeaves(SuffixTreeVertex *vertex, std::vector<int> *entries, int prefixLen,
                                    int indexAdditive, int suffixTreeStringLength, int substringLength);

std::vector<int> *SuffixTree::getEntryIndexes(std::string substring) {
    int substringLength = substring.length(), entryIndexesAdditive = this->globalSequenceEntryIndex,
            suffixTreeStringLength = this->builder->getTreeStringLength();
    std::vector<int> *entries = new std::vector<int>();

    auto farthestVertexAndRemainingSuffix =
            goDownUntilSuffixSuperimposes(this->builder, this->builder->getRoot(), substring);

    const std::string remainingSuffix = farthestVertexAndRemainingSuffix.second;
    auto farthestVertex = farthestVertexAndRemainingSuffix.first;
    std::function<void(SuffixTreeVertex *, int)> fillEntries =
            [&entries, entryIndexesAdditive, suffixTreeStringLength, substringLength]
                    (SuffixTreeVertex *farthestVertex, int remainingSuffixLength) {
                getRecursiveDownAndCountLeaves(farthestVertex, entries, remainingSuffixLength,
                                               entryIndexesAdditive, suffixTreeStringLength, substringLength);
            };

    if (remainingSuffix.length() == 0) {
        fillEntries(farthestVertex, remainingSuffix.length());
    } else if (hasChildWithPrefixChar(this->builder, farthestVertex, remainingSuffix.at(0))) {
        auto childWithPrefixChar = getChildWithPrefixChar(this->builder, farthestVertex, remainingSuffix.at(0));
        const std::string childWithPrefixCharString = this->builder->getVertexSubstring(childWithPrefixChar);
        if (childWithPrefixChar != nullptr && doSecondStringOverlays(childWithPrefixCharString, remainingSuffix)) {
            fillEntries(getChildWithPrefixChar(this->builder, farthestVertex, remainingSuffix.at(0)),
                        childWithPrefixCharString.length() - remainingSuffix.length());
        }
    }

    std::sort(entries->begin(), entries->end());
    return entries;
}

bool doSecondStringOverlays(const std::string &first, const std::string &second) {
    return first.length() >= second.length() && first.substr(0, second.length()) == second;
}

void getRecursiveDownAndCountLeaves(SuffixTreeVertex *vertex, std::vector<int> *entries, int prefixLen,
                                    int indexAdditive, int suffixTreeStringLength, int substringLength) {
    if (vertex->getChildren()->size() > 0) {
        for (int i = 0; i < vertex->getChildren()->size(); ++i) {
            SuffixTreeVertex *child = vertex->getChildren()->at(i);
            getRecursiveDownAndCountLeaves(child, entries, prefixLen + child->getInfo().second,
                                           indexAdditive, suffixTreeStringLength, substringLength);
        }
    } else {
        entries->push_back(indexAdditive + suffixTreeStringLength - prefixLen - substringLength);
    }
}
