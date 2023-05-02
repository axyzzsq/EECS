# Flush

## What is it?

In computing, the term "flush" is commonly used to refer to the act of writing data that is currently stored in a buffer to a destination storage device. A buffer is a temporary area of memory used to hold data before it is written to a file, sent over a network, or displayed on a screen.

When data is written to a buffer, it is not immediately written to the destination storage device, but is instead held in the buffer until certain conditions are met, such as when the buffer is full or when the program requests that the data be written. Flushing a buffer means that the data currently stored in the buffer is forced to be written to the destination storage device, regardless of whether or not the buffer is full.

For example, when writing data to a file, a program might use a buffer to temporarily store the data before writing it to the file. If the program does not explicitly flush the buffer, the data may remain in the buffer even after the program has finished executing, potentially resulting in data loss. Flushing the buffer ensures that the data is written to the file before the program terminates.

In summary, flushing is the act of forcibly writing data from a buffer to a destination storage device, typically to ensure that the data is saved or transmitted correctly.

## How to use?

```C
#include <stdio.h>

int main() {
   FILE *fp;
   char str[] = "This is a test.\n";
 
   // Open file for writing
   fp = fopen("test.txt", "w");
   
   // Write string to buffer
   fputs(str, fp);
   
   // Flush buffer to disk
   fflush(fp);
   
   // Close file
   fclose(fp);
   
   return 0;
}

```

In this example, we first open a file called "test.txt" for writing using the `fopen()` function. We then write the string "This is a test." to the buffer using the `fputs()` function. Before we close the file using `fclose()`, we call the `fflush()` function to ensure that any data still stored in the buffer is written to the disk.

The `fflush()` function takes a file pointer as its argument and writes any data still in the buffer associated with that file to the disk. This is particularly useful when you want to ensure that data is immediately written to disk, rather than waiting for the buffer to fill up or the file to be closed.

![image-20230224120049994](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230224120049994.png)

## Why use it?

There are several possible disadvantages of not using flush:

1. Data loss: If you do not flush a buffer before closing a file or terminating a program, any data that is still in the buffer may be lost. This can happen because the data has not yet been written to the destination storage device and is still only stored in memory.

   数据丢失：如果在关闭文件或终止程序之前不刷新缓冲区，则缓冲区中的任何数据都可能丢失。 发生这种情况是因为数据尚未写入目标存储设备，仍仅存储在内存中。

2. Performance issues: When you write data to a file, the operating system typically uses a buffer to store the data temporarily. If you do not flush the buffer, the data may remain in memory for an extended period of time, which can consume system resources and reduce performance.

   性能问题：当你向文件写入数据时，操作系统通常会使用一个缓冲区来临时存储数据。 如果不刷新缓冲区，数据可能会在内存中保留很长时间，这会消耗系统资源并降低性能。

3. File corruption: If a program crashes or terminates unexpectedly while data is still in the buffer, the file may become corrupted. This is because the data has not been fully written to the file and may be incomplete or inconsistent.

   文件损坏：如果程序崩溃或意外终止，而数据仍在缓冲区中，文件可能会损坏。 这是因为数据还没有完全写入文件，可能不完整或不一致。

4. Inaccurate results: If a program performs some calculation based on the data in a buffer, and the buffer has not been flushed, the results may be inaccurate. This can happen because the data used in the calculation may not reflect the most up-to-date values.

   结果不准确：如果程序根据缓冲区中的数据进行一些计算，而缓冲区还没有被刷新，则结果可能不准确。 发生这种情况是因为计算中使用的数据可能未反映最新值。

In summary, not using flush can result in data loss, performance issues, file corruption, and inaccurate results. It is important to ensure that buffers are flushed before closing files or terminating programs to prevent these issues.