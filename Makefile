main: nin
	ctest

nin: cmk
	cd build && ninja

cmk: build
	cd build && cmake -G Ninja ..


build:
	mkdir build

clean:
	rm build
