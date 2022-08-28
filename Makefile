all:
	g++ src/bin/main.cpp -std=c++11 -Iinclude -o server
	mkdir build
	mkdir build/bin
	mkdir build/bin/release
	mkdir -p received
	mv server build/bin/release
clean:
	rm -rf build *.so *.a *.o main
cleanFiles:
	rm -rf received