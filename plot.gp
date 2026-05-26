set terminal png size 1200,800
set grid
set xlabel "Array size (elements)"
set ylabel "Time (seconds)"
set key top left
set logscale x 2

set output "plots/random.png"
set title "Random data"
plot "data/insertion_random.dat" using 1:2 with linespoints lw 2 pt 7 title "Insertion Sort", \
     "data/heap_random.dat"      using 1:2 with linespoints lw 2 pt 5 title "Heap Sort"

set output "plots/sorted.png"
set title "Sorted data (ascending)"
plot "data/insertion_sorted.dat" using 1:2 with linespoints lw 2 pt 7 title "Insertion Sort", \
     "data/heap_sorted.dat"      using 1:2 with linespoints lw 2 pt 5 title "Heap Sort"

set output "plots/reverse.png"
set title "Sorted data (descending)"
plot "data/insertion_reverse.dat" using 1:2 with linespoints lw 2 pt 7 title "Insertion Sort", \
     "data/heap_reverse.dat"      using 1:2 with linespoints lw 2 pt 5 title "Heap Sort"
