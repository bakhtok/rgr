all: libsort.dylib main benchmark

libsort.dylib: sortlib.c sortlib.h
	gcc-15 -dynamiclib -install_name @executable_path/libsort.dylib -o libsort.dylib sortlib.c

main: main.c bitstruct.c sortlib.h bitstruct.h
	gcc-15 -o main main.c bitstruct.c -L. -lsort

benchmark: benchmark.c sortlib.h
	gcc-15 -o benchmark benchmark.c -L. -lsort -lm

run: main
	DYLD_LIBRARY_PATH=. ./main

plots: benchmark
	mkdir -p data plots
	DYLD_LIBRARY_PATH=. ./benchmark
	gnuplot plot.gp

clean:
	rm -f libsort.dylib main benchmark
	rm -rf data plots
