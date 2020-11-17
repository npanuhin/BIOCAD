#include <assert.h>
#include <algorithm>

#include "../tests_maintenance.hpp"
#include "../../alignment/suffix_forest/suffix_tree/suffix_tree.hpp"


void getEntryIndexesTest();

int main() {
    printTestStarted("suffix tree");
    getEntryIndexesTest();
    printLastTestFinished();
    return 0;
}

bool equals(std::vector<int> *, std::vector<int>);

void getEntryIndexesTest() {
    std::string suffixTreeString = "abacababdf";
    SuffixTree *tree = new SuffixTree(suffixTreeString, 0);

    auto firstNotEmptyEntries = tree->getEntryIndexes("a"),
            secondNotEmptyEntries = tree->getEntryIndexes("aca"),
            firstEmptyEntries = tree->getEntryIndexes("m"),
            secondEmptyEntries = tree->getEntryIndexes("abmm"),
            thirdEmptyEntries = tree->getEntryIndexes("bacabfff");

    assert(equals(firstNotEmptyEntries, {0, 2, 4, 6}));
    assert(equals(secondNotEmptyEntries, {2}));
    assert(equals(firstEmptyEntries, {}));
    assert(equals(secondEmptyEntries, {}));
    assert(equals(thirdEmptyEntries, {}));

    delete tree;
}

bool equals(std::vector<int> *query, std::vector<int> v) {
    if (query->size() == v.size()) {
        std::sort(query->begin(), query->end());
        std::sort(v.begin(), v.end());
        return std::equal(query->begin(), query->end(), v.begin());
    } else {
        return false;
    }
}