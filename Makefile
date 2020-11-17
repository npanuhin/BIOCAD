CC = g++ -std=gnu++0x -std=c++17
PYTHON_INTERPRETER = python3

SOURCE_DIR = source
TEST_DIR = tests
INFOGRAPHIC_DIR = infographic

MAIN_ALGO_FILE = $(SOURCE_DIR)/main.cpp
MAIN_BIN_FILE = main_algo.exe
TEST_DIR_EXE_FILES := $(shell find ./$(TEST_DIR)/* -name "*.exe")

SUFFIX_FOREST_TESTS_DIR := $(TEST_DIR)/suffix_forest
SLICE_MATCHER_TESTS_DIR := $(TEST_DIR)/slice_matcher

ALL_SOURCE_CPP_FILES := $(shell find ./$(SOURCE_DIR)/*/* -name "*.cpp")
ALL_SOURCE_HPP_FILES := $(shell find ./$(SOURCE_DIR) -name "*.hpp")

SUFFIX_TREE_TESTS := $(addprefix $(SUFFIX_FOREST_TESTS_DIR)/, $(addsuffix .exe, \
	$(shell find ./$(SUFFIX_FOREST_TESTS_DIR) -iname "*.cpp" -execdir basename {} .cpp ';')))
SLICE_MATCHER_TESTS := $(addprefix $(SLICE_MATCHER_TESTS_DIR)/, $(addsuffix .exe, \
	$(shell find ./$(SLICE_MATCHER_TESTS_DIR) -iname "*.cpp" -execdir basename {} .cpp ';')))


all: suffix_forest_tests slice_matcher_tests main_algo

all_tests: suffix_forest_tests slice_matcher_tests

suffix_forest_tests: $(SUFFIX_TREE_TESTS)
	@$(addsuffix $() &&, $(addprefix ./, $(SUFFIX_TREE_TESTS))) echo "Finished $@\n";

slice_matcher_tests: $(SLICE_MATCHER_TESTS)
	@$(addsuffix $() &&, $(addprefix ./, $(SLICE_MATCHER_TESTS))) echo "Finished $@\n";

main_algo : $(MAIN_BIN_FILE)
	@./$^ && echo "Finished $@\n" && $(PYTHON_INTERPRETER) $(INFOGRAPHIC_DIR)/slice_matches_plotting.py;

slice_matches_plot:
	@$(PYTHON_INTERPRETER) $(INFOGRAPHIC_DIR)/slice_matches_plotting.py

$(SUFFIX_FOREST_TESTS_DIR)/%.exe : $(SUFFIX_FOREST_TESTS_DIR)/%.cpp $(ALL_SOURCE_CPP_FILES)
	@$(CC) $^ -o $@

$(SLICE_MATCHER_TESTS_DIR)/%.exe : $(SLICE_MATCHER_TESTS_DIR)/%.cpp $(ALL_SOURCE_CPP_FILES)
	@$(CC) $^ -o $@

$(MAIN_BIN_FILE) : $(MAIN_ALGO_FILE) $(ALL_SOURCE_CPP_FILES) $(ALL_SOURCE_HPP_FILES)
	@$(CC) $^ -o $@

clean:
	@rm  $(MAIN_BIN_FILE);  $(addprefix rm , $(addsuffix ;, $(TEST_DIR_EXE_FILES)))
