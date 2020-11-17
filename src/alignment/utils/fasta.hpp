#ifndef GMW_FASTA_HPP
#define GMW_FASTA_HPP

#include <string>
#include <vector>

class FastaSequence;

class FastaFileContent {
public:
    FastaFileContent(std::string filename, bool isInDataDir = true);

    std::vector<FastaSequence *> sequences;

private:
    std::string filePath;
};

class FastaSequence {
public:
    FastaSequence(std::string *description, std::string *source)
            : description(description), source(source) {};

    std::string *description;
    std::string *source;
};

#endif
