# 2D Image Convolution Speed Test

## Timing Results (Machine A)

```
#-----------------------------------------------------------------------------
# Matrix 2D Convolution Speed Test
#-----------------------------------------------------------------------------
# Image size: 1920 x 1080
# Naive C++ implementation:
#  * CPU-only
#  * single-threaded
#  * image and kernel stored column-major
#
# Seed value for random number generator: 1669255634825439
#-----------------------------------------------------------------------------
Kernel Size,Convolution Time (milliseconds)
"(3, 3)",25
"(5, 5)",62
"(7, 7)",110
"(9, 9)",169
"(11, 11)",257
"(13, 13)",367
"(15, 15)",499
"(17, 17)",651
"(19, 19)",821
"(21, 21)",1008
"(23, 23)",1204
"(25, 25)",1428
"(27, 27)",1653
"(29, 29)",1973
"(31, 31)",2214
```

## Machines Used for Profiling

### Machine A "Mac Laptop"
macOS Monterey Version 12.4  
MacBook Pro (13-inch, 2017, Four Thunderbolt 3 Ports)  
Processor: 3.1 GHz Dual-Core Intel Core i5  
Memory: 8 GB 2133 MHz LPDDR3  
Graphics: Intel Iris Plus Graphics 650 1536 MB  

### Machine B "Desktop"
Ubuntu Desktop
