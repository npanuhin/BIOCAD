//
// Created by ichigo on 29.10.2020.
//

#include <assert.h>
#include <functional>
#include <fstream>
#include <algorithm>

#include "../tests_maintenance.hpp"
#include "../../source/suffix_forest/suffix_forest.hpp"

#define SUFFIX_FOREST_TEST_DIR "./tests/suffix_forest/"

void getEntryIndexesTest();

int main() {
    printTestStarted("suffix forest");
    getEntryIndexesTest();
    printLastTestFinished();
    return 0;
}

void makeMockFastaFile(std::vector<int> *, const std::string &, const std::string &);

template<typename T>
bool areVectorsEqual(std::vector<T> *, std::vector<T> *);

void getEntryIndexesTest() {
    const std::string testSubstring = "acab", mockFilename = "suffix_forest_test_mock.fasta";
    auto entryTestSubstringEntryIndexes = new std::vector<int>({1, 5, 14, 7089, 12812, 12933, 17232, 21341});

    makeMockFastaFile(entryTestSubstringEntryIndexes, testSubstring, mockFilename);

    auto mockFastaFile = new FastaFileContent(SUFFIX_FOREST_TEST_DIR + mockFilename, false);
    auto suffixForest = new SuffixForest(mockFastaFile->sequences.at(0));
    suffixForest->build();

    std::vector<int> *suffixForestTestSubstringEntryIndexes = suffixForest->getEntryIndexes(testSubstring);

    assert(areVectorsEqual(entryTestSubstringEntryIndexes, suffixForestTestSubstringEntryIndexes));

    delete mockFastaFile;
    delete suffixForest;
}

std::string generateTestSequence(std::vector<int> *, const std::string &, std::string, int);

void generateMockFastaFile(std::string &, const std::string &);

void makeMockFastaFile(std::vector<int> *entrySubstringEntryIndexes,
                       const std::string &substring,
                       const std::string &mockFilename) {
    std::sort(entrySubstringEntryIndexes->begin(), entrySubstringEntryIndexes->end());
    std::string testSequence = generateTestSequence(entrySubstringEntryIndexes, substring,
                                                    "some_really_long_fill_string_for_faster_building_",
                                                    26000);
    generateMockFastaFile(testSequence, mockFilename);
}


template<typename T>
bool areVectorsEqual(std::vector<T> *first, std::vector<T> *second) {
    return first->size() == second->size() && std::equal(first->begin(), first->end(), second->begin());
}


std::string generateTestSequence(std::vector<int> *entrySubstringEntryIndexes,
                                 const std::string &substring,
                                 std::string fillString,
                                 int sequenceLength) {
    std::string testSequence = "";
    std::function<void(const std::string &, int)> addChars =
            [&testSequence](const std::string &fillString, int amount) {
                for (int i = 0, fillStringLength = fillString.length(); i < amount; i++) {
                    testSequence += fillString[i % fillStringLength];
                }
            };
    for (int iEntry = 0, lastEntry = 0; iEntry < entrySubstringEntryIndexes->size(); ++iEntry) {
        int nextSubstringEntryIndex = entrySubstringEntryIndexes->at(iEntry);
        addChars(fillString, nextSubstringEntryIndex - lastEntry);
        testSequence += substring;
        lastEntry = nextSubstringEntryIndex + substring.length();
    }
    addChars(fillString, sequenceLength - entrySubstringEntryIndexes->back() - substring.length());
    return testSequence;
}

void generateMockFastaFile(std::string &testSequence, const std::string &mockFilename) {
    std::string filePath = SUFFIX_FOREST_TEST_DIR + mockFilename;
    std::ofstream mockFastaFile(filePath);
    std::string const &fileDescription = "> Suffix forest test mock sequence";
    mockFastaFile << fileDescription << std::endl << testSequence;
    mockFastaFile.close();
}
