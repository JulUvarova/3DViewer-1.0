#sudo apt install qt6-base-dev

.PHONY: all
all: 
	rm -rf build
	mkdir -p build && \
	cd build && \
	cmake ../../src && \
	make
	# valgrind -q -s --leak-check=full --trace-children=yes --track-origins=yes  --log-file=./VALGRIND ./build/3DViewer
	./build/3DViewer

test:
	g++ -std=c++17 -o test_obj_data model/obj/test_obj_data.cc model/obj/obj_data.cc -lgtest -lgtest_main -pthread -I./model -I./include -DLOGGER_MAX_LOG_LEVEL_PRINTED=0
	./test_obj_data
	g++ -std=c++17 -o test_transform model/math/test_transform.cc -lgtest -lgtest_main -pthread -I./model -I./include -DLOGGER_MAX_LOG_LEVEL_PRINTED=0
	./test_transform

.PHONY: gcov_report
gcov_report:
# Run first test and capture its coverage data
	g++ --coverage -std=c++17 model/obj/test_obj_data.cc model/obj/obj_data.cc \
		-lgtest -lgtest_main -pthread -I./model -I./include \
		-DLOGGER_MAX_LOG_LEVEL_PRINTED=0 -o test_obj_data -lsubunit -lgcov
	./test_obj_data
	lcov --ignore-errors mismatch,gcov --no-external -t "test_obj_data" \
		-o ./test_obj_data.info -c -d .
	lcov --remove ./test_obj_data.info "range*" \
		--remove ./test_obj_data.info "Logger*" \
		-o ./test_obj_data_filtered.info

	# Remove gcda files before next test to avoid mixing data
	rm -f ./*.gcda

	# Run second test and capture its coverage data
	g++ --coverage -std=c++17 -o test_transform model/math/test_transform.cc \
		-lgtest -lgtest_main -pthread -I./model -I./include \
		-DLOGGER_MAX_LOG_LEVEL_PRINTED=0 -lsubunit -lgcov
	./test_transform
	lcov --ignore-errors mismatch,gcov --no-external -t "test_transform" \
		-o ./test_transform.info -c -d .

	# Merge the two filtered coverage info files into one
	lcov -a ./test_obj_data_filtered.info -a ./test_transform.info -o merged_coverage.info
	genhtml -o report merged_coverage.info

	# Cleanup
	rm -rf ./*.info ./*.gcda ./*.gcno