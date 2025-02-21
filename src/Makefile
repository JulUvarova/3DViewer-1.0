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

.PHONY: gcov_report
gcov_report:
#	@mkdir -p $(TEST_DIR)
	g++ --coverage -std=c++17 model/obj/test_obj_data.cc model/obj/obj_data.cc -lgtest -lgtest_main -pthread  -I./model -I./include -DLOGGER_MAX_LOG_LEVEL_PRINTED=0 -o test_obj_data -lsubunit  -lgcov
#	cd $(TEST_DIR)
	./test_obj_data
	lcov --ignore-errors mismatch,gcov --no-external  -t "s21_test" -o ./test_obj_data.info -c -d .
	lcov --remove ./test_obj_data.info "range*" --remove ./test_obj_data.info "Logger*"  -o ./test_obj_data_filtered.info
	genhtml -o report ./test_obj_data_filtered.info
	rm -rf tmp
	rm -rf ./*.info
	rm -rf ./*.gcda
	rm -rf ./*.gcno