set terminal png size 1200,800
set grid
set xlabel "Array size (elements)"
set ylabel "Time (seconds)"
set key top left
set logscale x 2
set logscale y

# --- measured (log-log) ---

set output "plots/random.png"
set title "Random data (log-log)"
plot "data/insertion_random.dat" using 1:2 with linespoints lw 2 pt 7 lc 1 title "Insertion Sort", \
     "data/heap_random.dat"      using 1:2 with linespoints lw 2 pt 5 lc 2 title "Heap Sort"

set output "plots/sorted.png"
set title "Sorted data — ascending (log-log)"
plot "data/insertion_sorted.dat" using 1:2 with linespoints lw 2 pt 7 lc 1 title "Insertion Sort", \
     "data/heap_sorted.dat"      using 1:2 with linespoints lw 2 pt 5 lc 2 title "Heap Sort"

set output "plots/reverse.png"
set title "Sorted data — descending (log-log)"
plot "data/insertion_reverse.dat" using 1:2 with linespoints lw 2 pt 7 lc 1 title "Insertion Sort", \
     "data/heap_reverse.dat"      using 1:2 with linespoints lw 2 pt 5 lc 2 title "Heap Sort"

# --- ideal (log-log) ---

set output "plots/random_i.png"
set title "Random data — ideal complexity (log-log)"
plot "data/ideal_insertion_random.dat" using 1:2 with linespoints lw 2 pt 7 lc 1 title "O(n^2)", \
     "data/ideal_heap_random.dat"      using 1:2 with linespoints lw 2 pt 5 lc 2 title "O(n log n)"

set output "plots/sorted_i.png"
set title "Sorted data — ascending — ideal complexity (log-log)"
plot "data/ideal_insertion_sorted.dat" using 1:2 with linespoints lw 2 pt 7 lc 1 title "O(n)", \
     "data/ideal_heap_sorted.dat"      using 1:2 with linespoints lw 2 pt 5 lc 2 title "O(n log n)"

set output "plots/reverse_i.png"
set title "Sorted data — descending — ideal complexity (log-log)"
plot "data/ideal_insertion_reverse.dat" using 1:2 with linespoints lw 2 pt 7 lc 1 title "O(n^2)", \
     "data/ideal_heap_reverse.dat"      using 1:2 with linespoints lw 2 pt 5 lc 2 title "O(n log n)"
