#!/usr/bin/gnuplot
set term pdfcairo font "Source-Han-Serif-CN,14" size 5.40in,3.3in
# set title font "Source-Han-Serif-CN-Bold,14"
set encoding utf8
set xlabel "布隆过滤器位图比特与总数据量之比"
set ylabel "每 1K 数据定位时间 (ms) / 空间 (MB)"
set output "figure/benchmark-bloom.pdf"
# set title "布隆过滤器在位图与数据不同比例下定位时间与空间使用"
plot "benchmark-bloom.txt" u 1:2 w lp pt 5 ps 0.8 t "时间", "" u 1:3 w lp pt 7 ps 0.8 t "空间"
set output