//
// Created by ichigo on 02.11.2020.
//

#ifndef GMW_APPROXIMATE_SLICE_MATCHES_FILE_FORMAT_HPP
#define GMW_APPROXIMATE_SLICE_MATCHES_FILE_FORMAT_HPP

#include "approximate_slice_matcher.hpp"

#include <string>

#define FILES_OUTPUT_DIR "./output/alignment/"


class ApproximateSliceMatchesFileFormat {
public:
    ApproximateSliceMatchesFileFormat(const std::string &);

    ~ApproximateSliceMatchesFileFormat();

    void write(GlobalApproximateSliceMatcher *);

    GlobalApproximateSliceMatcher *extract();

private:
    const std::string filename;
};


#endif //GMW_APPROXIMATE_SLICE_MATCHES_FILE_FORMAT_HPP
