all: libsort.dylib main benchmark

libsort.dylib: sortlib.c sortlib.h
	gcc-15 -dynamiclib -install_name @executable_path/libsort.dylib -o libsort.dylib sortlib.c

main: main.c bitstruct.c sortlib.h bitstruct.h
	gcc-15 -o main main.c bitstruct.c -L. -lsort

benchmark: benchmark.c sortlib.h
	gcc-15 -o benchmark benchmark.c -L. -lsort -lm

run: main
	DYLD_LIBRARY_PATH=. ./main

# Готовый показательный прогон: оба алгоритма, int + atom
demo: main
	@echo "=== Insertion Sort — случайные int ==="
	DYLD_LIBRARY_PATH=. ./main -a insertion -n 12 -t int -o random
	@echo ""
	@echo "=== Heap Sort — случайные int ==="
	DYLD_LIBRARY_PATH=. ./main -a heap -n 12 -t int -o random
	@echo ""
	@echo "=== Insertion Sort — атомы (по Z) ==="
	DYLD_LIBRARY_PATH=. ./main -a insertion -n 8 -t atom

# Ввод своих чисел: make sort N=7 ALG=heap
# или из файла:    make sort N=5 < nums.txt
N   ?= 10
ALG ?= heap
sort: main
	DYLD_LIBRARY_PATH=. ./main -a $(ALG) -n $(N) -o stdin

plots: benchmark
	mkdir -p data plots
	DYLD_LIBRARY_PATH=. ./benchmark
	gnuplot plot.gp

clean:
	rm -f libsort.dylib main benchmark
	rm -rf data plots
