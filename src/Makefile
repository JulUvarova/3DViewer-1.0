# Dependencies: qt6-base-dev, gtest, lcov

# Define common compiler flags
CXX = g++
CXXFLAGS = -std=c++17 -I./model -I./include -DLOGGER_MAX_LOG_LEVEL_PRINTED=0
LDFLAGS = -lgtest -lgtest_main -pthread
GCOV_FLAGS = --coverage -lsubunit -lgcov

# Define source files
OBJ_DATA_SRC = model/obj/obj_data.cc
OBJ_DATA_TEST = model/obj/test_obj_data.cc
OBJ_DATA_TEST_BIN = test_obj_data
TRANSFORM_TEST = model/math/test_transform.cc
TRANSFORM_TEST_BIN = test_transform

# Build directory
BUILD_DIR = build

.PHONY: all clean test gcov_report

all: 
	mkdir -p $(BUILD_DIR) && \
	cd $(BUILD_DIR) && \
	cmake ../../src && \
	make 
	# valgrind -q -s --leak-check=full --trace-children=yes --track-origins=yes  --log-file=./VALGRIND ./build/3DViewer
	./$(BUILD_DIR)/3DViewer

# Use pattern rules for tests
test: test_obj_data test_transform

test_obj_data: $(OBJ_DATA_TEST) $(OBJ_DATA_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	./$@

test_transform: $(TRANSFORM_TEST)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	./$@

gcov_report: clean_coverage
	# Build and run obj_data test with coverage
	$(CXX) $(GCOV_FLAGS) $(CXXFLAGS) $(OBJ_DATA_TEST) $(OBJ_DATA_SRC) -o $(OBJ_DATA_TEST_BIN) $(LDFLAGS)
	./$(OBJ_DATA_TEST_BIN)
	lcov --ignore-errors mismatch,gcov --no-external -t "$(OBJ_DATA_TEST_BIN)" -o ./$(OBJ_DATA_TEST_BIN).info -c -d .
	lcov --remove ./$(OBJ_DATA_TEST_BIN).info "range*" --remove ./$(OBJ_DATA_TEST_BIN).info "Logger*" -o ./$(OBJ_DATA_TEST_BIN)_filtered.info

	# Build and run transform test with coverage
	$(CXX) $(GCOV_FLAGS) $(CXXFLAGS) $(TRANSFORM_TEST) -o $(TRANSFORM_TEST_BIN) $(LDFLAGS)
	./$(TRANSFORM_TEST_BIN)
	lcov --ignore-errors mismatch,gcov --no-external -t "$(TRANSFORM_TEST_BIN)" -o ./$(TRANSFORM_TEST_BIN).info -c -d .
	lcov --remove ./$(TRANSFORM_TEST_BIN).info "range*" --remove ./$(TRANSFORM_TEST_BIN).info "Logger*" -o ./$(TRANSFORM_TEST_BIN)_filtered.info

	# Merge coverage data and generate report
	#lcov -a ./$(TRANSFORM_TEST_BIN)_filtered.info -o merged_coverage.info
	lcov -a ./$(OBJ_DATA_TEST_BIN)_filtered.info -a ./$(TRANSFORM_TEST_BIN)_filtered.info -o merged_coverage.info
	genhtml -o report merged_coverage.info

clean:
	rm -rf $(BUILD_DIR) $(OBJ_DATA_TEST_BIN) $(TRANSFORM_TEST_BIN) report *.info

clean_coverage:
	rm -f *.gcda *.gcno *.info test_obj_data test_transform