#!/usr/bin/gnuplot
set term pdfcairo font "Source-Han-Serif-CN,14" size 5.80in,4.0in
set title font "Source-Han-Serif-CN-Bold,16"
set encoding utf8
set key left top
set xlabel "假数据比例"
set ylabel "每 1K 数据测试时间 (ms)"
set output "figure/benchmark-false-percent.pdf"
set title "不同假数据比例下有无布隆过滤器测试 1K 数据使用的时间"
plot "benchmark-false-percent.txt" u 1:2 w lp pt 5 ps 0.8 t "无布隆过滤器", "" u 1:3 w lp pt 7 ps 0.8 t "有布隆过滤器"
set output