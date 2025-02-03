#sudo apt install qt6-base-dev

.PHONY: all
all: 
	rm -rf build
	mkdir -p build && \
	cd build && \
	cmake ../../src && \
	make
	./build/3DViewer