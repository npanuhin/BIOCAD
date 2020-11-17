#include "../cache/cache_file_format.hpp"
#include "suffix_tree_builder.hpp"
#include "vertex/suffix_tree_vertex.hpp"
#include "subsidiary/subsidiary.hpp"
#include <string>
#include <iostream>


#define SPECIAL_END_CHAR "$"


std::string suffixTreeDebugView = "";


SuffixTreeBuilder::SuffixTreeBuilder(std::string &suffixTreeString, std::string description)
        : suffixTreeString(suffixTreeString + SPECIAL_END_CHAR), description(description),
          root(new SuffixTreeVertex(0, 0)) {}

SuffixTreeBuilder::SuffixTreeBuilder(std::string &suffixTreeString)
        : SuffixTreeBuilder(suffixTreeString, std::string(DEFAULT_SEQUENCE_DESCRIPTION)) {}

SuffixTreeBuilder::~SuffixTreeBuilder() {
    delete this->root;
}

SuffixTreeVertex *SuffixTreeBuilder::getRoot() {
    return this->root;
}

std::string SuffixTreeBuilder::getSuffixTreeSubstring(int entryIndex, int substringLength) {
    return this->suffixTreeString.substr(entryIndex, substringLength);
}

std::string SuffixTreeBuilder::getVertexSubstring(SuffixTreeVertex *vertex) {
    std::pair<int, int> entryIndexAndSubstringLength = vertex->getInfo();
    return this->getSuffixTreeSubstring(entryIndexAndSubstringLength.first, entryIndexAndSubstringLength.second);
}

int SuffixTreeBuilder::getTreeStringLength() {
    return this->suffixTreeString.length();
}

void saveRecursiveSuffixTreeVertexDebugView(SuffixTreeBuilder *, SuffixTreeVertex *, int);

std::string getFormattedSuffixTreeVertexDebugView(std::string, int);

void SuffixTreeBuilder::printDebugView() {
    suffixTreeDebugView = "";
    saveRecursiveSuffixTreeVertexDebugView(this, this->getRoot(), 0);
    std::cout << suffixTreeDebugView << std::endl;
}

void SuffixTreeBuilder::build() {
    std::string nextSuffix = "";
    SuffixTreeVertex *lastLeafBuiltVertex = const_cast<SuffixTreeVertex *>(this->getRoot());
    for (int i = 1, stringLength = this->suffixTreeString.length(); i <= stringLength; ++i) {
        nextSuffix.insert(0, this->getSuffixTreeSubstring(stringLength - i, 1));
        SuffixTreeVertex *nextSuffixLeaf = handleNextSuffixAndGetNewLeaf(this, lastLeafBuiltVertex, nextSuffix);
        lastLeafBuiltVertex = nextSuffixLeaf;
    }
}

void saveRecursiveSuffixTreeVertexDebugView(SuffixTreeBuilder *builder, SuffixTreeVertex *vertex, int deep) {
    std::string vertexSubstring = vertex->isRoot() ? "#ROOT#" : builder->getVertexSubstring(vertex);
    std::string view = getFormattedSuffixTreeVertexDebugView(vertexSubstring, deep);
    for (int i = 0; i < vertex->getChildren()->size(); ++i) {
        SuffixTreeVertex *child = vertex->getChildren()->at(i);
        saveRecursiveSuffixTreeVertexDebugView(builder, child, deep + 1);
    }
    suffixTreeDebugView.insert(0, view);
}

std::string getFormattedSuffixTreeVertexDebugView(std::string vertexSubstring, int deep) {
    std::string view = "";
    for (int i = 0; i < deep; ++i) {
        view += " | ";
    }
    view += "\n";
    for (int i = 0; i < deep - 1; ++i) {
        view += " | ";
    }
    view += deep > 0 ? " |- " : " ";
    view += vertexSubstring;
    view += " (deep : " + std::to_string(deep) + ")";
    view += "\n";
    return view;
}
