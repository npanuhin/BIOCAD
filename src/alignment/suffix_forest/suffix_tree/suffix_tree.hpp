#ifndef SUFFIX_TREE
#define SUFFIX_TREE


#include <string>
#include "builder/suffix_tree_builder.hpp"
#include "builder/vertex/suffix_tree_vertex.hpp"
#include <vector>


class SuffixTree {
public:
    SuffixTree(std::string &, int);

    ~SuffixTree();

    std::vector<int> *getEntryIndexes(std::string);

private:
    SuffixTreeBuilder *builder;

    int globalSequenceEntryIndex;

    friend class SuffixTreeCacher;
};


#endif
