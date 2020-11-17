#include "suffix_tree_vertex.hpp"


SuffixTreeVertex::SuffixTreeVertex(int entryIndex, int substringLength) 
	: parent(nullptr), suffixTreeStringEntryIndex(entryIndex), suffixTreeStringSubstringLength(substringLength) {
	this->children = new std::vector<SuffixTreeVertex *>();
	this->prefixLinkedVertexes = new std::vector<SuffixTreeVertex *>();
	this->prefixLinkedChars = new std::vector<char>();
}

SuffixTreeVertex::~SuffixTreeVertex() {
	for (int i = 0; i < this->children->size(); ++i) {
		delete this->children->at(i);
	}
}

void SuffixTreeVertex::updateInfo(int entryIndex, int substringLength) {
	this->suffixTreeStringEntryIndex = entryIndex; 
	this->suffixTreeStringSubstringLength = substringLength;
}

std::pair<int, int> SuffixTreeVertex::getInfo() {
	return std::pair<int, int>(this->suffixTreeStringEntryIndex, this->suffixTreeStringSubstringLength);
}

std::vector<SuffixTreeVertex *> * SuffixTreeVertex::getChildren() {
	return this->children;
}

void SuffixTreeVertex::addChildRelation(SuffixTreeVertex * childToAdd) {
	for (int i = 0; i < this->children->size(); ++i) {
		if (this->children->at(i) == childToAdd) {
			return;
		}
	}
	this->children->push_back(childToAdd);
	childToAdd->parent = this;
}

void SuffixTreeVertex::removeChildRelation(SuffixTreeVertex * childToRemove) {
	for (int i = 0; i < this->children->size(); ++i) {
		SuffixTreeVertex * child = this->children->at(i);
		if (child == childToRemove) {
			this->children->erase(this->children->begin() + i);
			child->parent = nullptr;
		}
	}
}

void SuffixTreeVertex::addPrefixLinkedVertex(SuffixTreeVertex * prefixLinkedVertex, char prefixLinkedChar) {
	this->prefixLinkedVertexes->push_back(prefixLinkedVertex);
	this->prefixLinkedChars->push_back(prefixLinkedChar);
}

SuffixTreeVertex * SuffixTreeVertex::getPrefixLinkedVertex(char prefixLinkedCharToSearch) {
	for (int i = 0; i < this->prefixLinkedChars->size(); ++i) {
		if (this->prefixLinkedChars->at(i) == prefixLinkedCharToSearch) {
			return this->prefixLinkedVertexes->at(i);
		}
	}
	return nullptr;
}

bool SuffixTreeVertex::hasPrefixLinkedVertex(char prefixLinkedCharToSearch) {
	return getPrefixLinkedVertex(prefixLinkedCharToSearch) != nullptr;
}
	
SuffixTreeVertex * SuffixTreeVertex::getParent() {
	return this->parent;
}


bool SuffixTreeVertex::isRoot() {
	return this->parent == nullptr;
}

bool SuffixTreeVertex::isLeaf() {
	return this->children->size() == 0;
}
