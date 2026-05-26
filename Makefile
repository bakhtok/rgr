all: libsort.dylib main benchmark

# ── Зависимости ───────────────────────────────────────────────────────────────
# Проверяет и устанавливает: Homebrew, gcc@15, gnuplot
deps:
	@command -v brew >/dev/null 2>&1 || { \
		echo "Installing Homebrew..."; \
		/bin/bash -c "$$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"; \
	}
	@brew list gcc@15 >/dev/null 2>&1 || { echo "Installing gcc@15..."; brew install gcc@15; }
	@brew list gnuplot >/dev/null 2>&1 || { echo "Installing gnuplot..."; brew install gnuplot; }
	@echo "All dependencies OK"

# ── Сборка ────────────────────────────────────────────────────────────────────
libsort.dylib: sortlib.c sortlib.h
	gcc-15 -dynamiclib -install_name @executable_path/libsort.dylib -o libsort.dylib sortlib.c

main: main.c bitstruct.c sortlib.h bitstruct.h
	gcc-15 -o main main.c bitstruct.c -L. -lsort

benchmark: benchmark.c sortlib.h
	gcc-15 -o benchmark benchmark.c -L. -lsort -lm

# ── Запуск ────────────────────────────────────────────────────────────────────
run: main
	DYLD_LIBRARY_PATH=. ./main

# Показательный прогон: оба алгоритма, int + atom
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
# Из файла:        make sort N=5 < nums.txt
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
