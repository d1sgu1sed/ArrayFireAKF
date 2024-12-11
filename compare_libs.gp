set terminal pngcairo size 2700,1500 enhanced font 'arial, 30' lw 3
set output sprintf("compare_libs.png")

set multiplot layout 1,1\
    margins 0.1,0.9,0.1,0.9\
    spacing 0.05,0.06

set zeroaxis
# set xtics 40
set grid front lc rgb "grey" lt 2 dt 2 lw 1

set title 'Время для кода с использованием библиотек с учётом генерации сигналов. С OpenACC'

file1 = 'exec_time_libs_no_long.txt'
file2 = 'exec_time_libs_py.txt'

set xrange [10:21]
# set yrange [0:1000]

set xlabel 'Длина сигнала (бит)'
set ylabel 'Время выполнения (сек)'

plot file1 using 1:2 with lines title 'C++' lc rgb "green" lw 2,\
     file2 using 1:2 with lines title 'Python' lc rgb "red" lw 2,\