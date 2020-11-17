#ifndef SUFFIX_TREE_BUILDER
#define SUFFIX_TREE_BUILDER


#include <string>
#include "../../../utils/fasta.hpp"
#include "vertex/suffix_tree_vertex.hpp"


class SuffixTreeBuilder {
public:
	SuffixTreeBuilder(std::string &, std::string);

	explicit SuffixTreeBuilder(std::string &);

	~SuffixTreeBuilder();

	SuffixTreeVertex * getRoot();
	
	std::string getSuffixTreeSubstring(int, int);

	std::string getVertexSubstring(SuffixTreeVertex *);

	int getTreeStringLength();

	void build();

	void printDebugView();

private:
	SuffixTreeVertex * root;
	const std::string suffixTreeString, description;

	friend class SuffixTreeCacher;
};


#endif
