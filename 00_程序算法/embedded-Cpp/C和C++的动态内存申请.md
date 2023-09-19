# C/C++动态内存申请

## 一、Difference between C & C++

`new` and `delete` are C++ language keywords used for dynamic memory allocation, while `malloc` and `free` are functions used for dynamic memory allocation in the C language.

Here are some key differences between `new` and `malloc`, and `delete` and `free`:

- Type Safety: new and delete are type-safe, while malloc and free are not. This means that new and delete are aware of the type of data they are allocating and deallocating, and will automatically adjust for the size of the data type. malloc and free do not have this information, so the programmer must manually specify the size of the data type.

  类型安全：`new`和 `delete `是类型安全的，而 `malloc` 和 `free` 不是。 这意味着 `new` 和 `delete` 知道它们正在分配和释放的数据类型，并将自动调整数据类型的大小。 `malloc` 和 `free` 没有这个信息，所以程序员必须手动指定数据类型的大小。

- Constructor/Destructor: `new` calls the constructor of the object being created, while `malloc` does not. Similarly, `delete` calls the destructor of the object being deleted, while `free` does not.

  构造函数/析构函数：`new`调用被创建对象的构造函数，而`malloc`则不会。 同样，`delete `会调用被删除对象的析构函数，而 `free `则不会。

- Exception Handling: new throws a std::bad_alloc exception if it fails to allocate memory, while malloc returns a null pointer. This means that new can be used with exception handling, while malloc cannot.

  异常处理：`new`分配内存失败抛出`std::bad_alloc`异常，而`malloc`返回空指针。 这意味着` new `可以用于异常处理，而 `malloc` 不能。

- C++ Objects: new and delete are used to allocate and deallocate C++ objects, while malloc and free are used for general memory allocation.

  C++ 对象：`new `和 `delete` 用于分配和释放 C++ 对象，而 `malloc `和` free `用于一般内存分配。

Here's an example to illustrate the differences:

```cpp
// C++ example using new and delete
int* p = new int;
*p = 5;
delete p;

// C example using malloc and free
int* q = (int*)malloc(sizeof(int));
*q = 5;
free(q);

```

In this example, `new` is used to allocate memory for an integer and initialize it with the value 5. `delete` is used to deallocate the memory when it is no longer needed.

在此示例中，`new` 用于为整数分配内存并使用值 5 对其进行初始化。`delete` 用于在不再需要时释放内存。

In the C example, `malloc` is used to allocate memory for an integer, and `free` is used to deallocate the memory when it is no longer needed.

在 C 示例中，`malloc `用于为整数分配内存，而` free` 用于在不再需要时释放内存。

In general, it is recommended to use `new` and `delete` in C++ code, as they are safer and more convenient to use with C++ objects. However, `malloc` and `free` may be more appropriate in certain situations, such as when working with legacy C code or when interfacing with external libraries that use C-style memory allocation.

一般来说，推荐在C++代码中使用`new`和`delete`，因为它们与C++对象一起使用更安全、更方便。 但是，在某些情况下，`malloc `和 `free` 可能更合适，例如处理遗留 C 代码或与使用 C 风格内存分配的外部库交互时。

## C++  memory alloc 

When `new` and `delete` are called in C++, the following actions take place:

1. `new` is called: When `new` is called, the C++ runtime allocates memory from the system heap to hold the object. The amount of memory allocated is determined by the size of the object being created, which is specified in bytes using the `sizeof` operator. If there is not enough memory available to satisfy the request, `new` throws a `std::bad_alloc` exception.

    `new`被调用：当`new`被调用时，C++运行时从系统堆中分配内存来保存对象。 分配的内存量取决于正在创建的对象的大小，使用 sizeof 运算符以字节为单位指定。 如果没有足够的可用内存来满足请求，`new` 将抛出 `std::bad_alloc` 异常。

2. Constructor is called: Once the memory is allocated, the constructor of the object being created is called. The constructor initializes the object's member variables and sets the object's initial state.

   构造函数被调用：一旦分配了内存，就会调用正在创建的对象的构造函数。 构造函数初始化对象的成员变量并设置对象的初始状态。

3. A pointer is returned: Once the constructor has completed, `new` returns a pointer to the newly created object. This pointer is a memory address that points to the beginning of the object's memory block.

   返回一个指针：一旦构造函数完成，`new` 返回一个指向新创建对象的指针。 这个指针是一个内存地址，指向对象内存块的开始。

4. `delete` is called: When the object is no longer needed, the `delete` operator is called. This triggers the object's destructor, which frees any resources held by the object, such as dynamically allocated memory or file handles.

   `delete`被调用：当不再需要对象时，调用`delete`操作符。 这会触发对象的析构函数，它会释放对象持有的任何资源，例如动态分配的内存或文件句柄。

5. Memory is released: After the destructor has completed, the `delete` operator releases the memory used by the object back to the system heap. The memory is marked as available for reuse by other parts of the program.

   释放内存：析构函数完成后，`delete` 运算符将对象使用的内存释放回系统堆。 内存被标记为可供程序的其他部分重用。

It is important to note that the `new` and `delete` operators should always be used in pairs. If an object is created using `new`, it should always be deleted using `delete`. Likewise, if an array of objects is created using `new[]`, it should be deleted using `delete[]`. Failing to match `new` and `delete` calls can lead to memory leaks, where memory is allocated but never released, eventually causing the program to run out of memory.

请务必注意，`new` 和 `delete` 运算符应始终成对使用。 如果一个对象是使用 new 创建的，它应该总是使用 delete 删除。 同样，如果使用“new[]”创建对象数组，则应使用“delete[]”将其删除。 未能匹配 `new` 和 `delete` 调用会导致内存泄漏，即分配内存但从未释放，最终导致程序耗尽内存。

In summary, `new` and `delete` are used to dynamically allocate and deallocate memory for C++ objects. When `new` is called, memory is allocated for the object and its constructor is called. When `delete` is called, the object's destructor is called, and the memory used by the object is released back to the system heap.

总之，`new` 和 `delete` 用于为 C++ 对象动态分配和释放内存。 调用 new 时，会为对象分配内存并调用其构造函数。 当 delete 被调用时，对象的析构函数被调用，对象使用的内存被释放回系统堆。