# RGR — Динамическая библиотека сортировок

**Ахмедов Бахтовар Боирович · вариант 2**  
Сортировка вставками + пирамидальная сортировка · macOS `.dylib`

---

## Сборка и запуск

```bash
make              # libsort.dylib + main + benchmark
make demo         # показательный прогон: оба алгоритма, int + atom
make sort N=7     # ввести 7 своих чисел с клавиатуры (ALG=heap по умолчанию)
make sort N=5 ALG=insertion < nums.txt   # из файла
make plots        # бенчмарк → data/*.dat → plots/*.png
make clean
```

---

## Интерфейс библиотеки

```c
void insertionSort(void *base, size_t nmemb, size_t size,
                   int (*compar)(const void *, const void *));
void heapSort     (void *base, size_t nmemb, size_t size,
                   int (*compar)(const void *, const void *));
```

Аналог `qsort` — сортирует любой тип данных через компаратор.  
`insertionSort`: буфер-ключ `malloc(size)`, сдвиг через `memcpy`.  
`heapSort`: итеративный `siftDown`, буфер-своп `malloc(size)`.

---

## Демо-программа (getopt)

```bash
./main [-a insertion|heap] [-n N] [-t int|atom] [-o random|sorted|reverse|stdin]
```

| Флаг | По умолчанию |
|---|---|
| `-a insertion\|heap` | `insertion` |
| `-n N` | `10` |
| `-t int\|atom` | `int` |
| `-o random\|sorted\|reverse\|stdin` | `random` |

Строка опций `"a:n:t:o:h"`: буква с `:` требует аргумент (`optarg`), без `:` — флаг.  
`getopt` возвращает `-1` в конце, `'?'` при неизвестном флаге.

```c
while ((opt = getopt(argc, argv, "a:n:t:o:h")) != -1) {
    switch (opt) {
        case 'a': alg   = optarg;       break;
        case 'n': n     = atoi(optarg); break;
        case 't': type  = optarg;       break;
        case 'o': order = optarg;       break;
        default:  printUsage(argv[0]); return opt == 'h' ? 0 : 1;
    }
}
```

---

## Структура `atom`

16-битная упаковка химического элемента:

```
бит 15–9  Z (7 бит, 1–118)
бит 8–6   группа (3 бита)
бит 5–3   период (3 бита)
бит 2     металл/неметалл
бит 1–0   зарезервировано
```

---

## Бенчмарк

- n = 5 000 … 50 000, шаг 5 000 · 3 типа входных данных · warmup + 3 прогона
- Таймер: `clock_gettime(CLOCK_MONOTONIC)`
- Генерирует 12 `.dat`-файлов: измеренные + идеальные кривые (O(n²) / O(n log n) / O(n))

---

## Динамическая библиотека: macOS vs Linux

### macOS (`.dylib`)

```bash
# Собрать библиотеку
gcc -dynamiclib \
    -install_name @executable_path/libsort.dylib \
    -o libsort.dylib sortlib.c

# Слинковать программу
gcc -o main main.c -L. -lsort

# Запустить (если install_name не вшит или путь не тот)
DYLD_LIBRARY_PATH=. ./main

# Проверить зависимости бинарника
otool -L ./main
```

| Флаг | Смысл |
|---|---|
| `-dynamiclib` | создать `.dylib` (аналог `-shared` на Linux) |
| `-install_name @executable_path/lib.dylib` | путь к `.dylib`, вшитый в саму библиотеку; `@executable_path` — папка с исполняемым файлом |
| `DYLD_LIBRARY_PATH` | переменная окружения для поиска `.dylib` в рантайме |
| `otool -L` | показать, какие `.dylib` требует бинарник |

---

### Linux (`.so`)

```bash
# Собрать библиотеку
gcc -shared -fPIC \
    -Wl,-soname,libsort.so.1 \
    -o libsort.so.1 sortlib.c
ln -s libsort.so.1 libsort.so   # симлинк для линковщика

# Слинковать программу
gcc -o main main.c -L. -lsort

# Запустить
LD_LIBRARY_PATH=. ./main

# Проверить зависимости
ldd ./main
```

| Флаг | Смысл |
|---|---|
| `-shared` | создать разделяемую библиотеку `.so` |
| `-fPIC` | Position Independent Code — обязателен для `.so` |
| `-Wl,-soname,libsort.so.1` | вшить soname (имя с версией) в саму библиотеку |
| `LD_LIBRARY_PATH` | переменная окружения для поиска `.so` в рантайме |
| `ldd` | показать зависимости (аналог `otool -L`) |

---

### Ключевые отличия

| | macOS | Linux |
|---|---|---|
| Расширение | `.dylib` | `.so` |
| Флаг компилятора | `-dynamiclib` | `-shared -fPIC` |
| Вшитый путь | `-install_name @executable_path/…` | `-Wl,-soname,…` (только имя, без пути) |
| Переменная окружения | `DYLD_LIBRARY_PATH` | `LD_LIBRARY_PATH` |
| Инспекция зависимостей | `otool -L` | `ldd` |
| `-fPIC` обязателен | нет (но не вредит) | да |

---

## Графики (`plots/`)

| Файл | Содержимое |
|---|---|
| `random.png` / `sorted.png` / `reverse.png` | Оба алгоритма по типу входа |
| `insertion.png` | Insertion: все три типа, log Y |
| `heap.png` | Heap: все три типа |
| `random_i.png` / `sorted_i.png` / `reverse_i.png` | Идеальные кривые |
