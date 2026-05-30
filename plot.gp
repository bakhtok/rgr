set terminal png size 1200,800
set grid
set xlabel "Array size (elements)"
set key top left
set xrange [0:55000]
set xtics 10000
set yrange [0:*]

# --- Swaps ---

set ylabel "Swaps"

set output "plots/swaps_random.png"
set title "Swaps — Random data"
plot "data/insertion_random.dat" using 1:2 smooth csplines with lines lw 2.5 lc 1 title "Insertion Sort", \
     "data/insertion_random.dat" using 1:2 with points pt 7 ps 1.2 lc 1 notitle, \
     "data/heap_random.dat"      using 1:2 smooth csplines with lines lw 2.5 lc 2 title "Heap Sort", \
     "data/heap_random.dat"      using 1:2 with points pt 5 ps 1.2 lc 2 notitle

set output "plots/swaps_sorted.png"
set title "Swaps — Sorted data (ascending)"
plot "data/insertion_sorted.dat" using 1:2 smooth csplines with lines lw 2.5 lc 1 title "Insertion Sort", \
     "data/insertion_sorted.dat" using 1:2 with points pt 7 ps 1.2 lc 1 notitle, \
     "data/heap_sorted.dat"      using 1:2 smooth csplines with lines lw 2.5 lc 2 title "Heap Sort", \
     "data/heap_sorted.dat"      using 1:2 with points pt 5 ps 1.2 lc 2 notitle

set output "plots/swaps_reverse.png"
set title "Swaps — Sorted data (descending)"
plot "data/insertion_reverse.dat" using 1:2 smooth csplines with lines lw 2.5 lc 1 title "Insertion Sort", \
     "data/insertion_reverse.dat" using 1:2 with points pt 7 ps 1.2 lc 1 notitle, \
     "data/heap_reverse.dat"      using 1:2 smooth csplines with lines lw 2.5 lc 2 title "Heap Sort", \
     "data/heap_reverse.dat"      using 1:2 with points pt 5 ps 1.2 lc 2 notitle

# --- Comparisons ---

set ylabel "Comparisons"

set output "plots/iters_random.png"
set title "Comparisons — Random data"
plot "data/insertion_random.dat" using 1:3 smooth csplines with lines lw 2.5 lc 1 title "Insertion Sort", \
     "data/insertion_random.dat" using 1:3 with points pt 7 ps 1.2 lc 1 notitle, \
     "data/heap_random.dat"      using 1:3 smooth csplines with lines lw 2.5 lc 2 title "Heap Sort", \
     "data/heap_random.dat"      using 1:3 with points pt 5 ps 1.2 lc 2 notitle

set output "plots/iters_sorted.png"
set title "Comparisons — Sorted data (ascending)"
plot "data/insertion_sorted.dat" using 1:3 smooth csplines with lines lw 2.5 lc 1 title "Insertion Sort", \
     "data/insertion_sorted.dat" using 1:3 with points pt 7 ps 1.2 lc 1 notitle, \
     "data/heap_sorted.dat"      using 1:3 smooth csplines with lines lw 2.5 lc 2 title "Heap Sort", \
     "data/heap_sorted.dat"      using 1:3 with points pt 5 ps 1.2 lc 2 notitle

set output "plots/iters_reverse.png"
set title "Comparisons — Sorted data (descending)"
plot "data/insertion_reverse.dat" using 1:3 smooth csplines with lines lw 2.5 lc 1 title "Insertion Sort", \
     "data/insertion_reverse.dat" using 1:3 with points pt 7 ps 1.2 lc 1 notitle, \
     "data/heap_reverse.dat"      using 1:3 smooth csplines with lines lw 2.5 lc 2 title "Heap Sort", \
     "data/heap_reverse.dat"      using 1:3 with points pt 5 ps 1.2 lc 2 notitle
