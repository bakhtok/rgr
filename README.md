# RGR — Динамическая библиотека сортировок

**Ахмедов Бахтовар Боирович · вариант 2**
Сортировка вставками + пирамидальная сортировка · динамическая библиотека macOS

---

## Структура проекта

```
rgr/
├── sortlib.h / sortlib.c   — динамическая библиотека (libsort.dylib)
├── bitstruct.h / bitstruct.c — пользовательская структура atom (пример generic-сортировки)
├── main.c                  — демо-программа (getopt)
├── benchmark.c             — замеры производительности
├── plot.gp                 — gnuplot-скрипт (8 графиков)
├── Makefile
├── RESULTS.md              — анализ результатов
└── README.md
```

---

## Сборка и запуск

```bash
make          # собрать всё: libsort.dylib, main, benchmark
make run      # запустить демо (DYLD_LIBRARY_PATH=.)
make plots    # бенчмарк + графики → plots/*.png
make clean    # удалить собранные файлы
```

---

## Описание кода

### `sortlib.h` / `sortlib.c` — динамическая библиотека

Реализует два алгоритма с **обобщённым интерфейсом** (аналогично `qsort` из стандартной библиотеки):

```c
void insertionSort(void *base, size_t nmemb, size_t size,
                   int (*compar)(const void *, const void *));

void heapSort(void *base, size_t nmemb, size_t size,
              int (*compar)(const void *, const void *));
```

Параметры:
- `base` — указатель на начало массива
- `nmemb` — количество элементов
- `size` — размер одного элемента в байтах (`sizeof(int)`, `sizeof(atom)`, …)
- `compar` — функция-компаратор, возвращает <0 / 0 / >0

Благодаря `void*` и компаратору библиотека сортирует **любые типы данных** — в том числе пользовательские структуры (см. пример с `atom`).

**Ключевые детали реализации:**

`insertionSort` — один `malloc(size)` на всю сортировку (буфер для текущего ключа), сдвиг через `memcpy`.

`heapSort` — итеративный `siftDown` (не рекурсивный — нет риска переполнения стека), один `malloc(size)` на весь прогон (буфер для обмена).

Динамическая библиотека компилируется с флагом `-install_name @executable_path/libsort.dylib`, поэтому `./main` находит её без выставления `DYLD_LIBRARY_PATH`. Переменная окружения по-прежнему работает при запуске вручную:

```bash
DYLD_LIBRARY_PATH=. ./main
```

---

### `main.c` — демо-программа

Использует `getopt` для разбора аргументов командной строки:

| Флаг | Значение | По умолчанию |
|---|---|---|
| `-a insertion\|heap` | алгоритм | `insertion` |
| `-n N` | размер массива | `10` |
| `-t int\|atom` | тип данных | `int` |
| `-o random\|sorted\|reverse` | начальный порядок | `random` |
| `-h` | справка | — |

Примеры:

```bash
./main -a heap -n 20 -t int -o random
./main -a insertion -n 8 -t atom          # сортировка атомов по порядковому номеру Z
./main -h
```

Сортировка структуры `atom` демонстрирует, что библиотека работает с произвольными типами: передаётся компаратор `compareAtomsByZ`, сортировка идёт по полю Z, упакованному в битовое поле `uint16_t`.

---

### `benchmark.c` — замеры производительности

- Диапазон: n = 5 000 … 50 000, шаг 5 000 (10 точек)
- 3 типа входных данных: случайные, возрастание, убывание
- Каждая точка: 1 прогон разогрева + 3 измерения, среднее
- Таймер: `clock_gettime(CLOCK_MONOTONIC)` (наносекундная точность)
- Если время одного прогона превышает 900 с — измерения для данного алгоритма прекращаются
- После замеров автоматически генерирует **идеальные кривые** (`.dat`-файлы), откалиброванные по точке n = 50 000

Выходные файлы (`data/`):

```
insertion_random.dat   heap_random.dat
insertion_sorted.dat   heap_sorted.dat
insertion_reverse.dat  heap_reverse.dat
ideal_insertion_*.dat  ideal_heap_*.dat
```

---

### `bitstruct.h` / `bitstruct.c` — структура `atom`

Упакованная структура химического элемента таблицы Менделеева в 16 битах:

```
бит 15–9  порядковый номер Z (7 бит, 1–118)
бит 8–6   группа (3 бита, 1–8)
бит 5–3   период (3 бита, 1–7)
бит 2     металл/неметалл (1 бит)
бит 1–0   зарезервировано
```

Используется для демонстрации generic-сортировки: массив структур `atom` сортируется по Z через `compareAtomsByZ`.

---

## Описание Makefile

```makefile
all: libsort.dylib main benchmark
```
Цель по умолчанию — собирает все три артефакта.

```makefile
libsort.dylib: sortlib.c sortlib.h
    gcc-15 -dynamiclib -install_name @executable_path/libsort.dylib -o libsort.dylib sortlib.c
```
`-dynamiclib` — создаёт `.dylib` (macOS-аналог `.so`).  
`-install_name @executable_path/libsort.dylib` — прописывает путь к библиотеке относительно исполняемого файла, чтобы не нужен был `DYLD_LIBRARY_PATH`.

```makefile
main: main.c bitstruct.c sortlib.h bitstruct.h
    gcc-15 -o main main.c bitstruct.c -L. -lsort
```
`-L.` — искать библиотеки в текущей директории.  
`-lsort` — линковать с `libsort.dylib`.

```makefile
benchmark: benchmark.c sortlib.h
    gcc-15 -o benchmark benchmark.c -L. -lsort -lm
```
`-lm` — математическая библиотека (нужна для `log2()` при генерации идеальных кривых).

```makefile
run: main
    DYLD_LIBRARY_PATH=. ./main
```
Явно выставляет путь к динамической библиотеке через переменную окружения.

```makefile
plots: benchmark
    mkdir -p data plots
    DYLD_LIBRARY_PATH=. ./benchmark
    gnuplot plot.gp
```
Запускает бенчмарк, затем строит все графики.

```makefile
clean:
    rm -f libsort.dylib main benchmark
    rm -rf data plots
```
Удаляет все сгенерированные файлы, оставляя только исходники.

---

## Графики

| Файл | Содержимое |
|---|---|
| `plots/random.png` | Оба алгоритма, случайные данные |
| `plots/sorted.png` | Оба алгоритма, возрастание |
| `plots/reverse.png` | Оба алгоритма, убывание |
| `plots/insertion.png` | Insertion Sort — все три типа (log Y) |
| `plots/heap.png` | Heap Sort — все три типа |
| `plots/random_i.png` | Идеальные кривые O(n²) vs O(n log n), случайные |
| `plots/sorted_i.png` | Идеальные кривые O(n) vs O(n log n), возрастание |
| `plots/reverse_i.png` | Идеальные кривые O(n²) vs O(n log n), убывание |
