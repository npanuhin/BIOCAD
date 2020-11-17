#include <assert.h>

#include "../tests_maintenance.hpp"
#include "../../source/suffix_forest/suffix_tree/builder/vertex/suffix_tree_vertex.hpp"
#include "../../source/suffix_forest/suffix_tree/builder/suffix_tree_builder.hpp"
#include "../../source/suffix_forest/suffix_tree/builder/subsidiary/subsidiary.hpp"


void getVertexSubstringTest();

void buildInIntermediaryTest();

void createIndependentLeafVertexTest();

void getChildWithPrefixCharTest();

void isVertexSubstringSuperimposesWithSuffixTest();

void goUpUntilFoundPrefixLinkTest();

void goDownUntilSuffixSuperimposesTest();

void addLeafToVertexTest();

void goUpTheBranchesAndAddPrefixLinksIfNecessaryTest();

void splitBranchWhileSuffixSuperimposesAndGetSplitPlaceVertexTest();

void forkBranchAndGetNewLeafTest();

void handleNextSuffixAndGetNewLeafTest();

void buildTest();

int main() {
    printTestStarted("suffix tree builder");
    buildInIntermediaryTest();
    createIndependentLeafVertexTest();
    getChildWithPrefixCharTest();
    isVertexSubstringSuperimposesWithSuffixTest();
    goUpUntilFoundPrefixLinkTest();
    goDownUntilSuffixSuperimposesTest();
    addLeafToVertexTest();
    goUpTheBranchesAndAddPrefixLinksIfNecessaryTest();
    splitBranchWhileSuffixSuperimposesAndGetSplitPlaceVertexTest();
    forkBranchAndGetNewLeafTest();
    handleNextSuffixAndGetNewLeafTest();
    buildTest();
    printLastTestFinished();
    return 0;
}

SuffixTreeBuilder *createHelloWorldSuffixTreeBuilder();

SuffixTreeVertex *createEmptyVertex();

std::string getSuffixTreeBuilderString(SuffixTreeBuilder *);

void buildInIntermediaryTest() {
    SuffixTreeVertex *parent = createEmptyVertex(),
            *child = createEmptyVertex(),
            *intermediary = createEmptyVertex();

    buildInIntermediary(parent, child, intermediary);

    assert(child->getParent() == intermediary);
    assert(intermediary->getParent() == parent);
    assert(parent->getChildren()->at(0) == intermediary);
    assert(intermediary->getChildren()->at(0) == child);
    assert(parent->getChildren()->size() == 1);
    assert(intermediary->getChildren()->size() == 1);

    delete parent;
}

void createIndependentLeafVertexTest() {
    SuffixTreeBuilder *builder = createHelloWorldSuffixTreeBuilder();
    int suffixLength = 6;
    std::string treeString = builder->getSuffixTreeSubstring(0, builder->getTreeStringLength()),
            someSuffix = treeString.substr(treeString.length() - suffixLength, suffixLength);

    SuffixTreeVertex *leaf = createIndependentLeafVertex(builder, someSuffix);

    assert(builder->getVertexSubstring(leaf) == someSuffix);
    assert(leaf->getChildren()->size() == 0);
    assert(leaf->getParent() == nullptr);

    delete builder;
}

void getChildWithPrefixCharTest() {
    SuffixTreeBuilder *builder = createHelloWorldSuffixTreeBuilder();
    std::string suffixTreeString = getSuffixTreeBuilderString(builder);
    int firstCharEntryIndex = 0, secondCharEntryIndex = 1;
    char firstChar = suffixTreeString.at(firstCharEntryIndex),
            secondChar = suffixTreeString.at(secondCharEntryIndex);
    SuffixTreeVertex *firstCharChild = new SuffixTreeVertex(firstCharEntryIndex, 1),
            *secondCharChild = new SuffixTreeVertex(secondCharEntryIndex, 1);
    SuffixTreeVertex *root = const_cast<SuffixTreeVertex *>(builder->getRoot());

    root->addChildRelation(firstCharChild);
    root->addChildRelation(secondCharChild);

    assert(getChildWithPrefixChar(builder, root, firstChar) == firstCharChild);
    assert(getChildWithPrefixChar(builder, root, secondChar) == secondCharChild);

    delete builder;
}

void isVertexSubstringSuperimposesWithSuffixTest() {
    SuffixTreeBuilder *builder = createHelloWorldSuffixTreeBuilder();
    int vertexSubstringLength = 5;
    SuffixTreeVertex *vertex = new SuffixTreeVertex(0, vertexSubstringLength);
    builder->getRoot()->addChildRelation(vertex);

    std::string stringToSuperimpose = builder->getVertexSubstring(vertex).substr(0, vertexSubstringLength + 2);

    assert(isVertexSubstringSuperimposesWithSuffix(builder, vertex, stringToSuperimpose));

    delete builder;
}

void goUpUntilFoundPrefixLinkTest() {
    SuffixTreeBuilder *builder = createHelloWorldSuffixTreeBuilder();
    int entryIndex = 2, substringLength = 5;
    SuffixTreeVertex *firstBranchFirstDeepVertex = createEmptyVertex(),
            *firstBranchSecondDeepVertex = createEmptyVertex(),
            *firstBranchThirdDeepVertex = new SuffixTreeVertex(entryIndex, substringLength),
            *secondBranchFirstDeepVertex = createEmptyVertex(),
            *root = const_cast<SuffixTreeVertex *>(builder->getRoot());
    root->addChildRelation(firstBranchFirstDeepVertex);
    root->addChildRelation(secondBranchFirstDeepVertex);
    firstBranchFirstDeepVertex->addChildRelation(firstBranchSecondDeepVertex);
    firstBranchSecondDeepVertex->addChildRelation(firstBranchThirdDeepVertex);
    char prefixLinkChar = 'c';

    firstBranchSecondDeepVertex->addPrefixLinkedVertex(secondBranchFirstDeepVertex, prefixLinkChar);

    auto vertexWithPrefixLinkAndRestoredSuffix = goUpUntilFoundPrefixLink(
            builder, firstBranchThirdDeepVertex, prefixLinkChar);
    assert(vertexWithPrefixLinkAndRestoredSuffix.first == firstBranchSecondDeepVertex);
    assert(builder->getSuffixTreeSubstring(entryIndex, substringLength) ==
           vertexWithPrefixLinkAndRestoredSuffix.second);

    delete builder;
}

void goDownUntilSuffixSuperimposesTest() {
    SuffixTreeBuilder *builder = createHelloWorldSuffixTreeBuilder();
    int firstSubstringLength = 2, secondSubstringLength = 2, thirdSubstringLength = 3;
    SuffixTreeVertex *firstDeepVertex = new SuffixTreeVertex(0, firstSubstringLength),
            *secondDeepVertex = new SuffixTreeVertex(firstSubstringLength, secondSubstringLength),
            *thirdDeepVertex = new SuffixTreeVertex(firstSubstringLength + secondSubstringLength, thirdSubstringLength),
            *root = const_cast<SuffixTreeVertex *>(builder->getRoot());
    root->addChildRelation(firstDeepVertex);
    firstDeepVertex->addChildRelation(secondDeepVertex);
    secondDeepVertex->addChildRelation(thirdDeepVertex);

    int stringOverlayLength = firstSubstringLength + secondSubstringLength + 1;
    std::string suffixTreeString = getSuffixTreeBuilderString(builder),
            stringToOverlay = suffixTreeString.substr(0, stringOverlayLength);
    auto maxDeepVertexWhileOverlayingSubstringAndRemainingSuffix =
            goDownUntilSuffixSuperimposes(builder, root, stringToOverlay);

    assert(maxDeepVertexWhileOverlayingSubstringAndRemainingSuffix.first == secondDeepVertex);
    assert(maxDeepVertexWhileOverlayingSubstringAndRemainingSuffix.second ==
           suffixTreeString.substr(
                   stringOverlayLength - maxDeepVertexWhileOverlayingSubstringAndRemainingSuffix.second.length(),
                   maxDeepVertexWhileOverlayingSubstringAndRemainingSuffix.second.length()));

    delete builder;
}

void addLeafToVertexTest() {
    SuffixTreeBuilder *builder = createHelloWorldSuffixTreeBuilder();
    std::string suffixTreeString = getSuffixTreeBuilderString(builder);
    int suffixLength = 5, entryIndex = suffixTreeString.length() - suffixLength;
    SuffixTreeVertex *parent = createEmptyVertex();

    std::string suffixString = suffixTreeString.substr(entryIndex);
    SuffixTreeVertex *newLeaf = addLeafToVertex(builder, parent, suffixString);

    assert(parent->getChildren()->size() == 1);
    assert(parent->getChildren()->at(0) == newLeaf);
    assert(newLeaf->getParent() == parent);
    assert(newLeaf->getInfo().first == entryIndex && newLeaf->getInfo().second == suffixLength);

    delete builder;
}

void goUpTheBranchesAndAddPrefixLinksIfNecessaryTest() {
    SuffixTreeBuilder *builder = createHelloWorldSuffixTreeBuilder();
    SuffixTreeVertex *firstBranchFirstVertex = new SuffixTreeVertex(0, 1),
            *firstBranchSecondVertex = new SuffixTreeVertex(1, 3),
            *secondBranchFirstVertex = new SuffixTreeVertex(0, 1),
            *secondBranchSecondVertex = new SuffixTreeVertex(1, 2),
            *secondBranchThirdVertex = new SuffixTreeVertex(3, 1),
            *root = builder->getRoot();
    root->addChildRelation(firstBranchFirstVertex);
    firstBranchFirstVertex->addChildRelation(firstBranchSecondVertex);
    root->addChildRelation(secondBranchFirstVertex);
    secondBranchFirstVertex->addChildRelation(secondBranchSecondVertex);
    secondBranchSecondVertex->addChildRelation(secondBranchThirdVertex);

    char prefixChar = 'c';
    goUpTheBranchesAndAddPrefixLinksIfNecessary(builder, secondBranchThirdVertex, firstBranchSecondVertex, prefixChar);

    assert(secondBranchFirstVertex->getPrefixLinkedVertex(prefixChar) == firstBranchFirstVertex);
    assert(secondBranchSecondVertex->getPrefixLinkedVertex(prefixChar) == nullptr);
    assert(secondBranchThirdVertex->getPrefixLinkedVertex(prefixChar) == nullptr);
    assert(firstBranchFirstVertex->getPrefixLinkedVertex(prefixChar) == nullptr);
    assert(firstBranchSecondVertex->getPrefixLinkedVertex(prefixChar) == nullptr);

    delete builder;
}

void splitBranchWhileSuffixSuperimposesAndGetSplitPlaceVertexTest() {
    SuffixTreeBuilder *builder = createHelloWorldSuffixTreeBuilder();
    std::string suffixTreeString = getSuffixTreeBuilderString(builder);
    int substringEntryIndex = 0, substringLength = 5;
    SuffixTreeVertex *branchParent = createEmptyVertex(),
            *branchChild = new SuffixTreeVertex(substringEntryIndex, substringLength);
    branchParent->addChildRelation(branchChild);
    std::string stringToSuperimpose = builder->getVertexSubstring(branchChild).substr(substringEntryIndex,
                                                                                      substringLength - 2);

    SuffixTreeVertex *branchSplitPlace =
            splitBranchWhileSuffixSuperimposesAndGetSplitPlaceVertex(builder, branchParent, stringToSuperimpose);

    assert(branchParent->getChildren()->size() == 1);
    assert(branchParent->getChildren()->at(0) == branchSplitPlace);
    assert(branchSplitPlace->getParent() == branchParent);
    assert(branchSplitPlace->getChildren()->size() == 1);
    assert(branchSplitPlace->getChildren()->at(0) == branchChild);
    assert(branchChild->getParent() == branchSplitPlace);
    assert(branchChild->getChildren()->size() == 0);
    assert(builder->getVertexSubstring(branchSplitPlace) + builder->getVertexSubstring(branchChild) ==
           builder->getSuffixTreeSubstring(substringEntryIndex, substringLength));

    delete builder;
}

void forkBranchAndGetNewLeafTest() {
    SuffixTreeBuilder *builder = createHelloWorldSuffixTreeBuilder();
    std::string suffixTreeString = getSuffixTreeBuilderString(builder);
    int startLeafSubstringEntryIndex = 0, startLeafSubstringLength = 5;
    SuffixTreeVertex *startLeaf = new SuffixTreeVertex(0, startLeafSubstringLength),
            *root = builder->getRoot();
    root->addChildRelation(startLeaf);

    int forkLeafSubstringEntryIndex = 6, forkLeafSubstringLength = 3;
    std::string stringToSplitBranch = suffixTreeString.substr(startLeafSubstringEntryIndex,
                                                              startLeafSubstringLength - 2),
            stringToForkBranch = suffixTreeString.substr(forkLeafSubstringEntryIndex, forkLeafSubstringLength);

    auto splitLeaf = forkBranchAndGetNewLeaf(builder, root, stringToSplitBranch),
            forkLeaf = forkBranchAndGetNewLeaf(builder, root, stringToForkBranch),
            splitPlaceVertex = splitLeaf->getParent();

    assert(root->getChildren()->size() == 2);
    assert(root->getChildren()->at(0) == forkLeaf || root->getChildren()->at(1) == forkLeaf);
    assert(root->getChildren()->at(0) == splitPlaceVertex || root->getChildren()->at(1) == splitPlaceVertex);
    assert(forkLeaf->getParent() == root);
    assert(forkLeaf->getChildren()->size() == 0);
    assert(splitPlaceVertex->getParent() == root);
    assert(splitPlaceVertex->getChildren()->size() == 2);
    assert(splitPlaceVertex->getChildren()->at(0) == splitLeaf || splitPlaceVertex->getChildren()->at(1) == splitLeaf);
    assert(splitPlaceVertex->getChildren()->at(0) == startLeaf || splitPlaceVertex->getChildren()->at(1) == startLeaf);
    assert(splitLeaf->getParent() == splitPlaceVertex);
    assert(splitLeaf->getChildren()->size() == 0);
    assert(startLeaf->getParent() == splitPlaceVertex);
    assert(startLeaf->getChildren()->size() == 0);

    delete builder;
}

void handleNextSuffixAndGetNewLeafTest() {
    std::string builderString = "abacaba";
    SuffixTreeBuilder *builder = new SuffixTreeBuilder(builderString);
    std::string suffixTreeString = getSuffixTreeBuilderString(builder);
    int stringLength = suffixTreeString.length();
    SuffixTreeVertex *firstBuiltVertex = new SuffixTreeVertex(stringLength - 1, 1),
            *secondBuiltVertex = new SuffixTreeVertex(stringLength - 2, 2),
            *root = builder->getRoot();
    root->addChildRelation(firstBuiltVertex);
    root->addChildRelation(secondBuiltVertex);

    std::string thirdLeafSubstring = suffixTreeString.substr(stringLength - 3, 3),
            fourthLeafSubstring = suffixTreeString.substr(stringLength - 4, 4);
    auto thirdBuiltLeaf = handleNextSuffixAndGetNewLeaf(builder, secondBuiltVertex, thirdLeafSubstring);
    auto fourthBuiltLeaf = handleNextSuffixAndGetNewLeaf(builder, thirdBuiltLeaf, fourthLeafSubstring);

    assert(root->getChildren()->size() == 3);

    delete builder;
}

void buildTest() {
    // This test depends on suffix string input
    // Do not change builderString
    std::string builderString = "abaca";
    SuffixTreeBuilder *builder = new SuffixTreeBuilder(builderString);

    builder->build();
    SuffixTreeVertex *root = builder->getRoot(),
            *forkVertex = getChildWithPrefixChar(builder, root, 'a');

    /*
        Expected for input "acaba"

         #ROOT# (deep : 0)
        | 
        |- baca$ (deep : 1)
        | 
        |- a (deep : 1)
        |  | 
        |  |- baca$ (deep : 2)
        |  | 
        |  |- ca$ (deep : 2)
        |  | 
        |  |- $ (deep : 2)
        | 
        |- ca$ (deep : 1)
        | 
        |- $ (deep : 1)
    */

    assert(root->getChildren()->size() == 4);
    assert(builder->getVertexSubstring(getChildWithPrefixChar(builder, root, '$')) == "$");
    assert(builder->getVertexSubstring(getChildWithPrefixChar(builder, root, 'c')) == "ca$");
    assert(builder->getVertexSubstring(getChildWithPrefixChar(builder, root, 'b')) == "baca$");
    assert(builder->getVertexSubstring(getChildWithPrefixChar(builder, root, 'a')) == "a");
    assert(builder->getVertexSubstring(getChildWithPrefixChar(builder, forkVertex, 'b')) == "baca$");
    assert(builder->getVertexSubstring(getChildWithPrefixChar(builder, forkVertex, 'c')) == "ca$");
    assert(builder->getVertexSubstring(getChildWithPrefixChar(builder, forkVertex, '$')) == "$");

    delete builder;
}

SuffixTreeBuilder *createHelloWorldSuffixTreeBuilder() {
    std::string suffixTreeString = "hello_world";
    SuffixTreeBuilder *builder = new SuffixTreeBuilder(suffixTreeString);
    return builder;
}

SuffixTreeVertex *createEmptyVertex() {
    return new SuffixTreeVertex(0, 0);
}

std::string getSuffixTreeBuilderString(SuffixTreeBuilder *builder) {
    return builder->getSuffixTreeSubstring(0, builder->getTreeStringLength());
}
