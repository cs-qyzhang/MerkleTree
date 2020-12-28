#!/usr/bin/gnuplot
set term pdfcairo font "Source-Han-Serif-CN,14" size 5.80in,4.0in
set title font "Source-Han-Serif-CN-Bold,16"
set encoding utf8
set logscale
set key left top
set xlabel "数据量"
set ylabel "时间 (ms) / 空间 (KB)"
set output "figure/build-time.pdf"
set title "logscale 坐标下不同数据量对应的构建时间和空间使用"
plot "data.txt" u 1:2 w lp pt 5 ps 0.8 t "构建时间", "" u 1:4 w lp pt 7 ps 0.8 t "空间使用"
set output