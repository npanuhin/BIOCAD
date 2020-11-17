#include "fasta.hpp"
#include <fstream>
#include <iostream>

FastaFileContent::FastaFileContent(std::string filename, bool isInDataDir)
        : filePath((isInDataDir ? std::string(SAMPLES_DIR) : "./") + filename) {
    std::string line;
    std::ifstream in(this->filePath);
    std::cout << "Name of fasta file extracting : " << this->filePath << std::endl;
    if (in.is_open()) {
        while (getline(in, line)) {
            if (line.find('>') != std::string::npos) {
                sequences.push_back(new FastaSequence(new std::string(line),
                                                      new std::string("")));
            } else {
                sequences.back()->source->append(line);
            }
        }
    } else {
        std::cerr << "Haven\'t found fasta file with path : " << this->filePath << std::endl;
    }
    in.close();
}