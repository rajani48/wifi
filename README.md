FreeBSD WiFi Debug Module

Build Steps:
============
Build and load chdbg module
----------------------------
1. cd chdbg
2. make
3. kldload ./chdbg.ko

Build and load sample module
------------------------------
1. cd sample
2. make
3. kldload ./sample.ko

Build and run wifi_dbg.c
1. cc wifi_dbg.c
2. ./a.out
