#include "subsidiary.hpp"
// TODO
// Delete this directive
#include <iostream>


SuffixTreeVertex *handleNextSuffixAndGetNewLeaf(SuffixTreeBuilder *builder, SuffixTreeVertex *previousLeafVertex,
                                                std::string &nextSuffix) {
    using std::pair;

    char suffixPrefixChar = nextSuffix.at(0);
    // std::cout << "NEW SUFFIX : " << nextSuffix << std::endl;

    pair<SuffixTreeVertex *, std::string> vertexWithPrefixLinkAndRestoredSuffix =
            goUpUntilFoundPrefixLink(builder, previousLeafVertex, suffixPrefixChar);
    SuffixTreeVertex *previousBranchVertexWithPrefixLink = vertexWithPrefixLinkAndRestoredSuffix.first;
    std::string suffixString = vertexWithPrefixLinkAndRestoredSuffix.second;

    // std::cout << "HERE1 : " << previousBranchVertexWithPrefixLink->isRoot() << std::endl;

    SuffixTreeVertex *nextBranchPrefixLinkedVertex =
            previousBranchVertexWithPrefixLink->isRoot() ? previousBranchVertexWithPrefixLink :
            previousBranchVertexWithPrefixLink->getPrefixLinkedVertex(nextSuffix.at(0));

    // std::cout << "HERE2 : " << suffixString << std::endl;
    pair<SuffixTreeVertex *, std::string> buildInPlaceVertexAndLeftoverSuffix =
            goDownUntilSuffixSuperimposes(builder, nextBranchPrefixLinkedVertex, suffixString);
    // std::cout << "HERE3" << std::endl;
    SuffixTreeVertex *buildInPlaceVertex = buildInPlaceVertexAndLeftoverSuffix.first;
    suffixString = buildInPlaceVertexAndLeftoverSuffix.second;
    // std::cout << "HERE4" << std::endl;

    SuffixTreeVertex *newLeaf = forkBranchAndGetNewLeaf(builder, buildInPlaceVertex, suffixString);

    // goUpTheBranchesAndAddPrefixLinksIfNecessary(builder, previousLeafVertex, newLeaf, suffixPrefixChar);

    return newLeaf;
}

std::pair<SuffixTreeVertex *, std::string> goUpUntilFoundPrefixLink(SuffixTreeBuilder *builder,
                                                                    SuffixTreeVertex *vertex, char prefixLinkedChar) {
    std::string restoredSuffixWhileGoUp = "";
    for (; !(vertex->isRoot()) && !(vertex->hasPrefixLinkedVertex(prefixLinkedChar)); vertex = vertex->getParent()) {
        restoredSuffixWhileGoUp = builder->getVertexSubstring(vertex) + restoredSuffixWhileGoUp;
    }
    if (vertex->isRoot()) {
        restoredSuffixWhileGoUp = prefixLinkedChar + restoredSuffixWhileGoUp;
    }
    return {vertex, restoredSuffixWhileGoUp};
}


std::pair<SuffixTreeVertex *, std::string>
goDownUntilSuffixSuperimposes(SuffixTreeBuilder *builder, SuffixTreeVertex *currentVertex, std::string &suffixString) {
    while (suffixString.length() > 0 && hasChildWithPrefixChar(builder, currentVertex, suffixString.at(0))) {
        SuffixTreeVertex *childTryToFollow = getChildWithPrefixChar(builder, currentVertex, suffixString.at(0));
        if (isVertexSubstringSuperimposesWithSuffix(builder, childTryToFollow, suffixString)) {
            std::string childSubstring = builder->getVertexSubstring(childTryToFollow);
            suffixString = suffixString.substr(
                    childSubstring.length(), suffixString.length() - childSubstring.length());
            currentVertex = childTryToFollow;
        } else {
            break;
        }
    }
    return {currentVertex, suffixString};
}

SuffixTreeVertex *getChildWithPrefixChar(SuffixTreeBuilder *builder, SuffixTreeVertex *parent, char prefixChar) {
    for (int i = 0; i < parent->getChildren()->size(); ++i) {
        SuffixTreeVertex *child = parent->getChildren()->at(i);
        std::string childSubstring = builder->getVertexSubstring(child);
        if (childSubstring.at(0) == prefixChar) {
            return child;
        }
    }
    return nullptr;
}

bool hasChildWithPrefixChar(SuffixTreeBuilder *builder, SuffixTreeVertex *parent, char prefixChar) {
    return getChildWithPrefixChar(builder, parent, prefixChar) != nullptr;
}

bool isVertexSubstringSuperimposesWithSuffix(SuffixTreeBuilder *builder, SuffixTreeVertex *vertex,
                                             std::string &suffixString) {
    std::string vertexSubstring = builder->getVertexSubstring(vertex);
    return suffixString.length() >= vertexSubstring.length() &&
           suffixString.substr(0, vertexSubstring.length()) == vertexSubstring;
}

SuffixTreeVertex *forkBranchAndGetNewLeaf(SuffixTreeBuilder *builder, SuffixTreeVertex *closestExistingVertex,
                                          std::string &suffixString) {
    char firstSuffixChar = suffixString.at(0);
    if (hasChildWithPrefixChar(builder, closestExistingVertex, firstSuffixChar)) {
        closestExistingVertex = splitBranchWhileSuffixSuperimposesAndGetSplitPlaceVertex(builder, closestExistingVertex,
                                                                                         suffixString);
        int closestExistingVertexSubstringLength = closestExistingVertex->getInfo().second;
        suffixString = suffixString.substr(closestExistingVertexSubstringLength,
                                           suffixString.length() - closestExistingVertexSubstringLength);
    }
    SuffixTreeVertex *newLeaf = addLeafToVertex(builder, closestExistingVertex, suffixString);
    return newLeaf;
}


SuffixTreeVertex *
splitBranchWhileSuffixSuperimposesAndGetSplitPlaceVertex(SuffixTreeBuilder *builder, SuffixTreeVertex *branchParent,
                                                         std::string &suffixString) {
    SuffixTreeVertex *branchChild = getChildWithPrefixChar(builder, branchParent, suffixString.at(0));
    std::string branchString = builder->getVertexSubstring(branchChild);
    int suffixAndBranchStringsOverlayLength = getOverlayLength(suffixString, branchString);

    int branchChildSubstringEntryIndex = branchChild->getInfo().first, branchChildSubstringLength = branchChild->getInfo().second;;
    SuffixTreeVertex *splitPlaceVertex = new SuffixTreeVertex(branchChildSubstringEntryIndex,
                                                              suffixAndBranchStringsOverlayLength);
    branchChild->updateInfo(branchChildSubstringEntryIndex + suffixAndBranchStringsOverlayLength,
                            branchChildSubstringLength - suffixAndBranchStringsOverlayLength);
    buildInIntermediary(branchParent, branchChild, splitPlaceVertex);
    return splitPlaceVertex;
}

void buildInIntermediary(SuffixTreeVertex *parent, SuffixTreeVertex *child, SuffixTreeVertex *intermediary) {
    parent->removeChildRelation(child);
    parent->addChildRelation(intermediary);
    intermediary->addChildRelation(child);
}

int getOverlayLength(std::string &first, std::string &second) {
    int overlayLength = 0;
    for (; overlayLength < first.length() && overlayLength < second.length()
           && first.at(overlayLength) == second.at(overlayLength); ++overlayLength);
    return overlayLength;
}

SuffixTreeVertex *
addLeafToVertex(SuffixTreeBuilder *builder, SuffixTreeVertex *newLeafParent, std::string &suffixString) {
    SuffixTreeVertex *newLeaf = createIndependentLeafVertex(builder, suffixString);
    newLeafParent->addChildRelation(newLeaf);
    return newLeaf;
}

SuffixTreeVertex *createIndependentLeafVertex(SuffixTreeBuilder *builder, std::string &suffixString) {
    int suffixEntryIndex = builder->getTreeStringLength() - suffixString.length();
    SuffixTreeVertex *independentVertex = new SuffixTreeVertex(suffixEntryIndex, suffixString.length());
    return independentVertex;
}


void goUpTheBranchesAndAddPrefixLinksIfNecessary(SuffixTreeBuilder *builder,
                                                 SuffixTreeVertex *oldLeaf, SuffixTreeVertex *newLeaf,
                                                 char prefixChar) {
    SuffixTreeVertex *newLeafBreakVertex;
    if (newLeaf->getParent() == nullptr || newLeaf->getParent()->getParent() == nullptr) {
        return;
    } else {
        newLeafBreakVertex = newLeaf->getParent()->getParent();
    }
    for (std::string oldBranchRestoredString = "", newBranchRestoredString = "";;) {
        if (newLeaf == newLeafBreakVertex) {
            return;
        } else if (oldBranchRestoredString.length() < newBranchRestoredString.length() ||
                   oldBranchRestoredString == "") {
            if (oldLeaf->isRoot()) {
                break;
            } else {
                oldBranchRestoredString.insert(0, builder->getVertexSubstring(oldLeaf));
                oldLeaf = oldLeaf->getParent();
            }
        } else if (oldBranchRestoredString.length() > newBranchRestoredString.length()) {
            newBranchRestoredString.insert(0, builder->getVertexSubstring(newLeaf));
            newLeaf = newLeaf->getParent();
        } else {
            if (oldBranchRestoredString == newBranchRestoredString &&
                oldLeaf->getPrefixLinkedVertex(prefixChar) == nullptr) {
                oldLeaf->addPrefixLinkedVertex(newLeaf, prefixChar);
                // std::cout << "ADDED" << std::endl;
                // std::cout << oldBranchRestoredString << " " << newBranchRestoredString << std::endl;
            }
            newBranchRestoredString.insert(0, builder->getVertexSubstring(newLeaf));
            newLeaf = newLeaf->getParent();
        }
    }
}