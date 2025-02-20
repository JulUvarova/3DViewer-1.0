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

	