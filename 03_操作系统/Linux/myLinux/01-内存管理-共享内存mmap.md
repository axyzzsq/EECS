# 共享内存-mmap

![image-20230214202947912](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230214202947912.png)

## 例程1

mmap() 系统调用用于将文件或设备映射到内存中，以便可以使用读取和写入等内存访问操作来访问它们。它还可以用于分配可以在多个进程之间共享的匿名内存区域。以下是使用 mmap() 函数将文件映射到内存的示例用法：

```c
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int fd = open("file.txt", O_RDONLY);
    if (fd == -1) {
        printf("Error opening file.\n");
        return -1;
    }

    off_t size = lseek(fd, 0, SEEK_END);
    
    /*
    	NULL：此参数指定所需的内存映射起始地址。如果NULL通过，内核将自动选择一个合适的地址。
    	size：此参数以字节为单位指定内存映射的大小。
    	PROT_READ：此参数指定映射所需的内存保护。在这种情况下，PROT_READ表示映射应该是只读的。
    	MAP_SHARED：此参数指定要创建的映射类型。在这种情况下，MAP_SHARED表示映射应该与也映射同一文件的其他进程共享。
    	fd：此参数是要映射的文件的文件描述符。
    	0：此参数指定文件中开始映射的偏移量。在这种情况下，0表示映射应从文件的开头开始。
    */
    char *addr = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
    
    if (addr == MAP_FAILED) {
        printf("Error mapping file.\n");
        close(fd);
        return -1;
    }

    printf("File contents: %s\n", addr);

    if (munmap(addr, size) == -1) {
        printf("Error unmapping file.\n");
    }

    close(fd);
    return 0;
}

```

此代码以只读模式打开名为“file.txt”的文件，并使用 lseek() 函数确定其大小。然后它使用具有 PROT_READ 权限和 MAP_SHARED 标志的 mmap() 将文件映射到内存，并打印映射内存的内容。最后，它使用 munmap() 取消映射内存区域并关闭文件。

请注意，mmap() 返回的地址是指向映射内存区域起点的指针，可以像任何其他指针一样使用。但是，内存区域的大小必须是系统页面大小的倍数，地址也必须与页面大小对齐。



## 例程2

```C
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>

/*
 * ./copy 1.txt 2.txt
 * argc    = 3
 * argv[0] = "./copy"
 * argv[1] = "1.txt"
 * argv[2] = "2.txt"
 */
int main(int argc, char **argv)
{
        int fd_old, fd_new;
        struct stat stat;
        char *buf;

        /* 1. 判断参数 */
        if (argc != 3)
        {
                printf("Usage: %s <old-file> <new-file>\n", argv[0]);
                return -1;
        }

        /* 2. 打开老文件 */
        fd_old = open(argv[1], O_RDONLY);
        if (fd_old == -1)
        {
                printf("can not open file %s\n", argv[1]);
                return -1;
        }

        /* 3. 确定老文件的大小 */
        if (fstat(fd_old, &stat) == -1)
        {
                printf("can not get stat of file %s\n", argv[1]);
                return -1;
        }

        /* 4. 映射老文件 */
        buf = mmap(NULL, stat.st_size, PROT_READ, MAP_SHARED, fd_old, 0);
        if (buf == MAP_FAILED)
        {
                printf("can not mmap file %s\n", argv[1]);
                return -1;
        }

        /* 5. 创建新文件 */
        fd_new = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
        if (fd_new == -1)
        {
                printf("can not creat file %s\n", argv[2]);
                return -1;
        }

        /* 6. 写新文件 */
        if (write(fd_new, buf, stat.st_size) != stat.st_size)
        {
                printf("can not write %s\n", argv[2]);
                return -1;
        }

        /* 5. 关闭文件 */
        close(fd_old);
        close(fd_new);

        return 0;
}
```

- stat 

  - man stat

    stat - display file or file system status

- 程序测试

  ```shell
  gcc -o copy_mmap copy_mmap.c
  ./copy_mmap  copy_mmap.c new2.c
  cat new2.c #可以看到程序被复制到到new2.c中
  ```

【参考文献】

[共享内存](https://www.jianshu.com/p/096e1b58c678?u_atoken=41a3bca7-d07b-4551-93ff-79fb59a3f5ba&u_asession=012eBEsS3SrcAeCiZtKxSvnFrgo4VLAUv0mICexHWJJdviKiHBmLakAufMcwbqgjwZX0KNBwm7Lovlpxjd_P_q4JsKWYrT3W_NKPr8w6oU7K_YgnIbvQcQhnJVE_BnMOC5E3kHBSeC3ycZ3R-DrPYY52BkFo3NEHBv0PZUm6pbxQU&u_asig=05tEfbUPn5RoZXdCNUGkmmDaGElpOhZoOewMJAUo2FSJU3BESEuMcjXmDTPkY2bH8JYhLZv5b7gGZMtT9VkJeSS-xHyDwGKoOS8hZMb3ranQZG79wHrN_cAupsh7vVQNmpSS6PKQvJvPz4weuO3fy8bwFMwudA4smgzRdjQ_5THXP9JS7q8ZD7Xtz2Ly-b0kmuyAKRFSVJkkdwVUnyHAIJzWHbBlYOnd5BY7GRQyyMeVzBStTJBcUTS8XSwVi9X-uMWrnxlgSSvoRYzeD7jclHWe3h9VXwMyh6PgyDIVSG1W8f_aWkwfbzL9ulnt7JbBelxPV46rxeunwwnCjSohRo2NvyeaX3fHnDH19A_oh0u2WbZLBp6JY2UOjS4PtvZ7YPmWspDxyAEEo4kbsryBKb9Q&u_aref=pyCPYM6xbnmxvtynjLNgd%2FyCU7c%3D)