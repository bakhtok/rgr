OS := $(shell uname)

ifeq ($(OS), Darwin)
CC     = gcc-15
LIB    = libsort.dylib
LDFLAG = -dynamiclib -install_name @executable_path/libsort.dylib
RUN    = DYLD_LIBRARY_PATH=.
else
CC     = gcc
LIB    = libsort.so
LDFLAG = -shared -fPIC -Wl,-soname,libsort.so
RUN    = LD_LIBRARY_PATH=.
endif

N   ?= 10
ALG ?= heap

all: $(LIB) main benchmark

$(LIB): sortlib.c sortlib.h
	$(CC) $(LDFLAG) -o $(LIB) sortlib.c

main: main.c bitstruct.c sortlib.h bitstruct.h
	$(CC) -o main main.c bitstruct.c -L. -lsort

benchmark: benchmark.c sortlib.h
	$(CC) -o benchmark benchmark.c -L. -lsort

run: main
	$(RUN) ./main

demo: main
	@echo "=== Insertion Sort — случайные int ==="
	$(RUN) ./main -a insertion -n 12 -t int -o random
	@echo ""
	@echo "=== Heap Sort — случайные int ==="
	$(RUN) ./main -a heap -n 12 -t int -o random
	@echo ""
	@echo "=== Insertion Sort — атомы (по Z) ==="
	$(RUN) ./main -a insertion -n 8 -t atom

sort: main
	$(RUN) ./main -a $(ALG) -n $(N) -o stdin

plots: benchmark
	mkdir -p data plots
	$(RUN) ./benchmark
	gnuplot plot.gp

deps:
ifeq ($(OS), Darwin)
	@command -v brew >/dev/null 2>&1 || /bin/bash -c "$$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
	@brew list gcc@15 >/dev/null 2>&1 || brew install gcc@15
	@brew list gnuplot >/dev/null 2>&1 || brew install gnuplot
else
	sudo apt-get install -y gcc gnuplot
endif

clean:
	rm -f $(LIB) main benchmark
	rm -rf data plots
