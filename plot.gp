set terminal png size 1200,800
set grid
set xlabel "Array size (elements)"
set ylabel "Time (seconds)"
set key top left
set xrange [0:55000]
set xtics 10000

# --- per-input comparison (linear Y, обе кривые в своём масштабе) ---

set output "plots/random.png"
set title "Random data"
plot "data/insertion_random.dat" using 1:2 smooth csplines with lines lw 2.5 lc 1 title "Insertion Sort", \
     "data/insertion_random.dat" using 1:2 with points pt 7 ps 1.2 lc 1 notitle, \
     "data/heap_random.dat"      using 1:2 smooth csplines with lines lw 2.5 lc 2 title "Heap Sort", \
     "data/heap_random.dat"      using 1:2 with points pt 5 ps 1.2 lc 2 notitle

set output "plots/sorted.png"
set title "Sorted data (ascending)"
plot "data/insertion_sorted.dat" using 1:2 smooth csplines with lines lw 2.5 lc 1 title "Insertion Sort", \
     "data/insertion_sorted.dat" using 1:2 with points pt 7 ps 1.2 lc 1 notitle, \
     "data/heap_sorted.dat"      using 1:2 smooth csplines with lines lw 2.5 lc 2 title "Heap Sort", \
     "data/heap_sorted.dat"      using 1:2 with points pt 5 ps 1.2 lc 2 notitle

set output "plots/reverse.png"
set title "Sorted data (descending)"
plot "data/insertion_reverse.dat" using 1:2 smooth csplines with lines lw 2.5 lc 1 title "Insertion Sort", \
     "data/insertion_reverse.dat" using 1:2 with points pt 7 ps 1.2 lc 1 notitle, \
     "data/heap_reverse.dat"      using 1:2 smooth csplines with lines lw 2.5 lc 2 title "Heap Sort", \
     "data/heap_reverse.dat"      using 1:2 with points pt 5 ps 1.2 lc 2 notitle

# --- per-algorithm (каждый алгоритм — все три типа входных данных) ---

set logscale y
set output "plots/insertion.png"
set title "Insertion Sort — все типы входных данных (log Y)"
plot "data/insertion_random.dat"  using 1:2 smooth csplines with lines lw 2.5 lc 1 title "Random (O(n^2))", \
     "data/insertion_random.dat"  using 1:2 with points pt 7 ps 1.2 lc 1 notitle, \
     "data/insertion_reverse.dat" using 1:2 smooth csplines with lines lw 2.5 lc 3 title "Reverse (O(n^2))", \
     "data/insertion_reverse.dat" using 1:2 with points pt 9 ps 1.2 lc 3 notitle, \
     "data/insertion_sorted.dat"  using 1:2 smooth csplines with lines lw 2.5 lc 2 title "Sorted  (O(n))", \
     "data/insertion_sorted.dat"  using 1:2 with points pt 5 ps 1.2 lc 2 notitle

unset logscale y
set output "plots/heap.png"
set title "Heap Sort — все типы входных данных"
plot "data/heap_random.dat"  using 1:2 smooth csplines with lines lw 2.5 lc 1 title "Random", \
     "data/heap_random.dat"  using 1:2 with points pt 7 ps 1.2 lc 1 notitle, \
     "data/heap_reverse.dat" using 1:2 smooth csplines with lines lw 2.5 lc 3 title "Reverse", \
     "data/heap_reverse.dat" using 1:2 with points pt 9 ps 1.2 lc 3 notitle, \
     "data/heap_sorted.dat"  using 1:2 smooth csplines with lines lw 2.5 lc 2 title "Sorted", \
     "data/heap_sorted.dat"  using 1:2 with points pt 5 ps 1.2 lc 2 notitle

# --- ideal ---

set output "plots/random_i.png"
set title "Random data — ideal complexity"
plot "data/ideal_insertion_random.dat" using 1:2 smooth csplines with lines lw 2.5 lc 1 title "O(n^2)", \
     "data/ideal_insertion_random.dat" using 1:2 with points pt 7 ps 1.2 lc 1 notitle, \
     "data/ideal_heap_random.dat"      using 1:2 smooth csplines with lines lw 2.5 lc 2 title "O(n log n)", \
     "data/ideal_heap_random.dat"      using 1:2 with points pt 5 ps 1.2 lc 2 notitle

set output "plots/sorted_i.png"
set title "Sorted data (ascending) — ideal complexity"
plot "data/ideal_insertion_sorted.dat" using 1:2 smooth csplines with lines lw 2.5 lc 1 title "O(n)", \
     "data/ideal_insertion_sorted.dat" using 1:2 with points pt 7 ps 1.2 lc 1 notitle, \
     "data/ideal_heap_sorted.dat"      using 1:2 smooth csplines with lines lw 2.5 lc 2 title "O(n log n)", \
     "data/ideal_heap_sorted.dat"      using 1:2 with points pt 5 ps 1.2 lc 2 notitle

set output "plots/reverse_i.png"
set title "Sorted data (descending) — ideal complexity"
plot "data/ideal_insertion_reverse.dat" using 1:2 smooth csplines with lines lw 2.5 lc 1 title "O(n^2)", \
     "data/ideal_insertion_reverse.dat" using 1:2 with points pt 7 ps 1.2 lc 1 notitle, \
     "data/ideal_heap_reverse.dat"      using 1:2 smooth csplines with lines lw 2.5 lc 2 title "O(n log n)", \
     "data/ideal_heap_reverse.dat"      using 1:2 with points pt 5 ps 1.2 lc 2 notitle
