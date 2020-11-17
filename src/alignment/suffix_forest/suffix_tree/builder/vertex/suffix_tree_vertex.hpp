#ifndef SUFFIX_TREE_VERTEX
#define SUFFIX_TREE_VERTEX


#include <vector>
#include <string>


class SuffixTreeVertex {
public:
	SuffixTreeVertex(int, int);

	~SuffixTreeVertex();

	void updateInfo(int, int);

	std::pair<int, int> getInfo();

	std::vector<SuffixTreeVertex *> * getChildren();

	void addChildRelation(SuffixTreeVertex *);

	void removeChildRelation(SuffixTreeVertex *);
	
	void addPrefixLinkedVertex(SuffixTreeVertex *, char);

	SuffixTreeVertex * getPrefixLinkedVertex(char);

	bool hasPrefixLinkedVertex(char);

	SuffixTreeVertex * getParent();

	bool isRoot();

	bool isLeaf();

private:
	std::vector<SuffixTreeVertex *> * children;
	std::vector<SuffixTreeVertex *> * prefixLinkedVertexes;
	std::vector<char> * prefixLinkedChars;
	SuffixTreeVertex * parent;
	int suffixTreeStringEntryIndex, suffixTreeStringSubstringLength;
};


#endif