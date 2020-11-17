#include <assert.h>
#include <iostream>

#include "../tests_maintenance.hpp"
#include "../../source/suffix_forest/suffix_tree/builder/vertex/suffix_tree_vertex.hpp"


void updateInfoTest();

void childAddTest();

void removeChildTest();

void addAndGetPrefixLinkedVertex();

void parentSetTest();

void parentSetTest();

void rootClassificationTest();

void leafClassificationTest();

int main() {
    printTestStarted("suffix tree vertex");
    updateInfoTest();
    childAddTest();
    removeChildTest();
    addAndGetPrefixLinkedVertex();
    parentSetTest();
    parentSetTest();
    rootClassificationTest();
    leafClassificationTest();
    printLastTestFinished();
    return 0;
}

SuffixTreeVertex *createEmptyVertex();

void updateInfoTest() {
    int entryIndex = 1, substringLength = 3;
    SuffixTreeVertex *vertex = new SuffixTreeVertex(entryIndex, substringLength);

    assert(vertex->getInfo().first == entryIndex && vertex->getInfo().second == substringLength);

    delete vertex;
}

void childAddTest() {
    SuffixTreeVertex *parent = createEmptyVertex(),
            *child = createEmptyVertex();

    parent->addChildRelation(child);

    assert(parent->getChildren()->size() == 1);
    assert(parent->getChildren()->at(0) == child);

    delete parent;
}

template<typename T>
bool isElementExist(std::vector<T *> *v, T *elem);

void removeChildTest() {
    SuffixTreeVertex *parent = createEmptyVertex(),
            *child1 = createEmptyVertex(),
            *child2 = createEmptyVertex(),
            *child3 = createEmptyVertex();

    parent->addChildRelation(child1);
    parent->addChildRelation(child2);
    parent->addChildRelation(child3);
    parent->removeChildRelation(child2);

    assert(parent->getChildren()->size() == 2);
    assert(isElementExist(parent->getChildren(), child1));
    assert(isElementExist(parent->getChildren(), child3));
    assert(!isElementExist(parent->getChildren(), child2));

    delete parent, child2;
}

void addAndGetPrefixLinkedVertex() {
    SuffixTreeVertex *independentVertex = createEmptyVertex(),
            *prefixLinkedVertex = createEmptyVertex();
    char prefixLink = 'c';

    independentVertex->addPrefixLinkedVertex(prefixLinkedVertex, prefixLink);

    assert(independentVertex->hasPrefixLinkedVertex(prefixLink));
    assert(independentVertex->getPrefixLinkedVertex(prefixLink) == prefixLinkedVertex);

    delete independentVertex, prefixLinkedVertex;
}

void parentSetTest() {
    SuffixTreeVertex *parent = createEmptyVertex(),
            *child = createEmptyVertex();

    parent->addChildRelation(child);

    assert(child->getParent() == parent);

    delete parent;
}

void rootClassificationTest() {
    SuffixTreeVertex *rootVertex = createEmptyVertex(),
            *leafVertex = createEmptyVertex();

    rootVertex->addChildRelation(leafVertex);

    assert(rootVertex->isRoot());
    assert(!leafVertex->isRoot());

    delete rootVertex;
}

void leafClassificationTest() {
    SuffixTreeVertex *rootVertex = createEmptyVertex(),
            *leafVertex = createEmptyVertex();

    rootVertex->addChildRelation(leafVertex);

    assert(leafVertex->isLeaf());
    assert(!leafVertex->isRoot());

    delete rootVertex, leafVertex;
}

template<typename T>
bool isElementExist(std::vector<T *> *v, T *elem) {
    for (int i = 0; i < v->size(); ++i) {
        if (v->at(i) == elem) {
            return true;
        }
    }
    return false;
}

SuffixTreeVertex *createEmptyVertex() {
    return new SuffixTreeVertex(0, 0);
}
