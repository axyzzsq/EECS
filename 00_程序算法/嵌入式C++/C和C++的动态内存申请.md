# C/C++动态内存申请

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