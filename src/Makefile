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