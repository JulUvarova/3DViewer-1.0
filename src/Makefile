# Dependencies: qt6-base-dev, gtest, lcov, doxygen
# Large .obj files: https://disk.yandex.ru/d/WUhyihtAWnTGpA

CXX = g++
CXXFLAGS = -std=c++17 -I./model -I./include -DLOGGER_MAX_LOG_LEVEL_PRINTED=0
LDFLAGS = -lgtest -lgtest_main -pthread
GCOV_FLAGS = --coverage -lsubunit -lgcov
CPPCHECK_FLAGS = --enable=all --suppress=missingIncludeSystem --language=c++ \
				 --quiet --suppress=unusedFunction --suppress=unusedStructMember \
				 --suppress=shadowFunction --suppress=missingInclude --suppress=unknownMacro \
				 --suppress=unmatchedSuppression --suppress=missingInclude --suppress=checkersReport

OBJ_DATA_SRC = model/obj/obj_data.cc
OBJ_DATA_TEST = model/obj/test_obj_data.cc
OBJ_DATA_TEST_BIN = test_obj_data
TRANSFORM_TEST = model/math/test_transform.cc
TRANSFORM_TEST_BIN = test_transform

BUILD_DIR = build
INSTALL_DIR = bin
DIST_DIR = dist
DIST_NAME = 3DViewer.tar.gz

.PHONY: all clean test gcov_report

#########################################
#------- Build and run 3DViewr ---------#
#########################################
all: 
	mkdir -p $(BUILD_DIR) && \
	cd $(BUILD_DIR) && \
	cmake ../../src && \
	make 
	./$(BUILD_DIR)/3DViewer


#########################################
#--------- Build and run Tests ---------#
#########################################
tests: test_obj_data test_transform

test_obj_data: $(OBJ_DATA_TEST) $(OBJ_DATA_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	./$@

test_transform: $(TRANSFORM_TEST)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	./$@

#########################################
#----------- Test coverage -------------#
#########################################
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

#########################################
#----------- Installation --------------#
#########################################
.PHONY: install uninstall
install: clean all
	 cd ../ && mkdir -p $(INSTALL_DIR) && mkdir -p $(INSTALL_DIR)/sample_models
	 cd ../ && cp -rf src/$(BUILD_DIR)/3DViewer $(INSTALL_DIR)/
	 cd ../ && cp -rf src/view/primitives/*.obj $(INSTALL_DIR)/sample_models/
uninstall:
	cd ../ && rm -rf $(INSTALL_DIR)

#########################################
#----- Create distributive archive -----#
#########################################
.PHONY: dist clean_dist
dist: clean_dist
	cd ../ && mkdir -p ${DIST_DIR}/src
	cd ../ && cp -rf src/controller ${DIST_DIR}/src/controller
	cd ../ && cp -rf src/include ${DIST_DIR}/src/include
	cd ../ && cp -rf src/model ${DIST_DIR}/src/model
	cd ../ && cp -rf src/view ${DIST_DIR}/src/view
	cd ../ && cp -rf src/*.cc ${DIST_DIR}/src/
	cd ../ && cp -rf src/CMakeLists.txt ${DIST_DIR}/src/
	cd ../ && cp -rf src/Makefile ${DIST_DIR}/src/
	cd ../ && cp -rf src/Doxyfile ${DIST_DIR}/src/
	cd ../ && tar -czvf $(DIST_NAME) ${DIST_DIR}
	cd ../ && rm -rf ${DIST_DIR}

clean_dist:
	cd ../ && rm -rf $(DIST_NAME)
	
#########################################
#----------- Documentation -------------#
#########################################
dvi: 
	doxygen
	open docs/html/index.html

clean_dvi:
	rm -rf ./docs

#########################################
#---------------- Checks ---------------#
#########################################
.PHONY: check
check:
	find . \( -path "./build" -o -path "./include" \) -prune -o -type f -regex '.*\.cc' -print | xargs cppcheck $(CPPCHECK_FLAGS)
	find . \( -path "./build" -o -path "./include" \) -prune -o -type f -regex '.*\.h' -print | xargs cppcheck $(CPPCHECK_FLAGS)
	find . \( -path "./build" -o -path "./include" \) -prune -o -type f -regex '.*\.cc' -print  -exec clang-format -style='{BasedOnStyle: Google}' -n {} \;
	find . \( -path "./build" -o -path "./include" \) -prune -o -type f -regex '.*\.h' -print  -exec clang-format -style='{BasedOnStyle: Google}' -n {} \;

.PHONY: check_fix
check_fix:
	find . \( -path "./build" -o -path "./include" \) -prune -o -type f -regex '.*\.cc' -print | xargs cppcheck $(CPPCHECK_FLAGS)
	find . \( -path "./build" -o -path "./include" \) -prune -o -type f -regex '.*\.h' -print | xargs cppcheck $(CPPCHECK_FLAGS)
	find . \( -path "./build" -o -path "./include" \) -prune -o -type f -regex '.*\.cc' -print  -exec clang-format -style='{BasedOnStyle: Google}' -i {} \;
	find . \( -path "./build" -o -path "./include" \) -prune -o -type f -regex '.*\.h' -print  -exec clang-format -style='{BasedOnStyle: Google}' -i {} \;

.PHONY: clean clean_bin clean_coverage clean_dist clean_dvi
clean_bin:
	rm -rf $(BUILD_DIR) $(OBJ_DATA_TEST_BIN) $(TRANSFORM_TEST_BIN) report *.info

clean_coverage:
	rm -rf coverage*
	rm -f *.gcda *.gcno *.info test_obj_data test_transform

clean: clean_bin clean_coverage clean_dist clean_dvi