#!/usr/bin/gnuplot

set terminal pngcairo
#file output name
set output 'output.png'

#label x
set xlabel "Iterações"
set autoscale

#label y
set ylabel "Energia"

#title of file
set title ""

set grid
set style data points

#key font and size
set key font ",15"

set key spacing 5
set key top center
#unset key

set style line 1 lc rgb  '#8b1a0e' pt 8  ps 1 lt 1 lw 2
set style line 2 lc rgb  '#5e9c36' pt 2  ps 1 lt 1 lw 2 
set style line 3 lc rgb  '#f2520d' pt 3  ps 1 lt 1 lw 2 
set style line 4 lc rgb  '#6228d7' pt 4  ps 1 lt 1 lw 2 
set style line 5 lc rgb  '#2a8f1e' pt 5  ps 1 lt 1 lw 2 
set style line 6 lc rgb  '#8b1a0e' pt 6  ps 1 lt 1 lw 2
set style line 7 lc rgb  '#cc3396' pt 7  ps 1 lt 1 lw 2 
set style line 8 lc rgb  '#1972e6' pt 8  ps 1 lt 1 lw 2 
set style line 9 lc rgb  '#468db9' pt 9  ps 1 lt 1 lw 2 
set style line 10 lc rgb '#55aa97' pt 10 ps 1 lt 1 lw 2 
set style line 11 lc rgb '#3a858e' pt 11 ps 1 lt 1 lw 2
set style line 12 lc rgb '#fa440f' pt 12 ps 1 lt 1 lw 2 
set style line 13 lc rgb '#946b93' pt 13 ps 1 lt 1 lw 2 
set style line 14 lc rgb '#8ffab3' pt 14 ps 1 lt 1 lw 2 
set style line 15 lc rgb '#fdb4bd' pt 15 ps 1 lt 1 lw 2 
set style line 16 lc rgb '#ed9821' pt 15 ps 1 lt 1 lw 2 

set style line 12 lc rgb '#808080' lt 0 lw 1
set grid back ls 12

set style line 11 lc rgb '#808080' lt 1
set border 3 back ls 11
set tics nomirror

set style func linespoints
#set logscale y

#set x range with 
#set xrange[0:1000]

#f(x) = a * exp(b * x + c)

#fit f(x) 'data.log'  using 2:3 via a, b, c

#plot '-' w p ls 1, '-' w p ls 2, '-' w p ls 3

#change file input in consulta1.txt / any type it is ok
    plot 'data.txt'   using 1:3 title 'Energia'   with linespoints ls 2 pointsize 3


#http://gnuplot.sourceforge.net/demo/lines_arrows.html


