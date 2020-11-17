//
// Created by ichigo on 18.10.2020.
//

#ifndef GMW_GLOBAL_APPROXIMATE_SLICE_MATCHER_HPP
#define GMW_GLOBAL_APPROXIMATE_SLICE_MATCHER_HPP

#include <map>

#include "../../suffix_forest/suffix_forest.hpp"
#include "../../utils/fasta.hpp"


class GlobalApproximateSliceMatcher {
public:
    GlobalApproximateSliceMatcher(FastaSequence *, FastaSequence *, int = 50);

    ~GlobalApproximateSliceMatcher();

    std::pair<std::vector<int> *, std::vector<int> *>
    getQueryAndTargetSubstringEntries(const std::string &);

    const std::string &getQuerySequenceDescription();

    const std::string &getTargetSequenceDescription();

    const std::string getSliceEntriesStringRepr();

private:
    void findSlicesEntries();

    SuffixForest *querySequenceSuffixForest;

    FastaSequence *targetSequence;

    std::map<const std::string, std::pair<std::vector<int> *, std::vector<int> *>> *slicesEntries;

    int thresholdMathLength;

    friend class ApproximateSliceMatchesFileFormat;
};

#endif //GMW_GLOBAL_APPROXIMATE_SLICE_MATCHER_HPP
