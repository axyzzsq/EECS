# C++快速入门

## 一、C++基础知识

### 1、class是对struct的扩展

- 含有数据成员
- 含有成员函数

### 2、访问控制

- private
  - private只供类内部的函数使用
- public
  - public类外的程序可以使用
- protected

### 3、this指针

在类的成员函数中使用，表示当前对象

### 4、类和对象

```c++
int a;  // int是类型，a是变量
Person per; //Person是类型, per是对象
```

### 5、程序结构

- 类定义(.h)/类实现(.cpp)

- 命名空间

  ```c++
  /*.h/.cpp文件中: */
  namespace a  
      //声明或定义函数; 
      int fun();
         void fun2()...
  
  /*调用者源文件中:  */
  //1. 直接使用: 
  a::fun, a::fun2
  //2. using声明:
  using a::fun;  // 以后调用fun即表示a::fun
  //3. using编译:
  using namespace a ; // 以后调用fun, fun2即可
  ```

### 6、重载

函数名相同，参数不同 (类型、数量、顺序不同)

### 7、指针和引用

引用就是别名，引用定时时必须初始化:

```C++
 int a; int &b=a; //b即为a的别名
```

### 8、构造函数和析构函数

- 构造函数: 无返回值，函数名与类名相同

  ```c++
  // 可以设置默认参数
  fun(int a=1, int b=2)
  ```

- 有默认构造函数，一旦实现了自己的构造函数就必须自己实现默认构造函数

- 无参对象定义:` Person p`; 不能写为` Person p()`;  //后者是一个函数

- 类中含有对象成员时，先调用对象成员的默认构造函数，再调用类的构造函数

  - 要调用对象成员的其他构造函数，可以这样写: Student(int sID) : id(sID) 

    构造函数的""前加":"，加上成员的初始化代码

    对象成员：按定义时的顺序构造，与上述初始化代码顺序无关

    析构函数被调用的顺序：相反

- 构造顺序：按运行中定义对象的顺序调用构造函数，静态对象只调用一次构造函数; 全局对象在main函数执行前被构造

- 拷贝构造函数: Person(const Person & s)    

  - 用于

    ```c++
    Person s("lisi"); Person p(s);
    ```

  - 若不提供此函数，则调用默认拷贝构造函数，对各成员变量进行值复制

- 析造函数: 无返回值，无参数，函数形式为"`~类名()`"

  - 对象销毁前瞬间，由系统自动调用析构函数

### 9、new和delete

```c++
Person *p = new Person();         
delete p;
Person *pers = new Person[10]; 
delete []pers;
```

### 9、静态成员

- 静态数据成员

  - 类中定义，类外分配空间和初始化，初始化时不加`static`

    ```c++
    class Person 
        staitc int cnt;
    int Person::cnt = 0; //放在main函数之外
    ```

- 静态成员函数

  - 在类外定义不加static，不能访问非static成员

    ```c++
    //调用：
    Person::getCount();
    ```

### 10、友员

- 在类中声明非成员函数时加上`friend`，它即可访问类的私有数据成员

- 一个类的成员函数也可以是另一个类的友元

  ```c++
  class A 
  private :
       int a;
  public:
       friend void fun1();   // fun1不是A的成员函数也可以访问a
       frined void B::fun2(); // B类的fun2也可以访问a
  
  ```

### 11、运算符重载

- 作为类外函数重载

  ```c++
  Point operator+(cont Point& p1, cont Point& p2) /* p1,p2相加 */
  ```

- 作为类的成员函数重载

  ```c++
  Point operator+(cont Point& p1)   /* this 当前对象和p1相加 */
  ```

- 增量操作符(++,--)

  - 前增量操作

    ```c++
    //类外函数：
    Point& operator++(cont Point& p1) /* p1增加后, return p1 */
    //成员函数：
    Point& operator++(void) /* this对象增加后, return *this */
    ```

  - 后增量操作

    ```c++
    //类外函数：
    Point operator++(cont Point& p1, int b)
        Point n = p1; //p1增加; 
    return n;
    //成员函数：
    Point operator++(intb) 
        Point n = *this;  //*this增加; 
    return n;
    ```

- 返回结果

  - 值返回
    - 返回函数内部定义的局部变量；
    - 该变量在函数执行时被创建，执行完毕时被销毁；
    - 只返回值；
    - 效率低
  - 引用返回
    - 效率高
  - 选择原则
    - 不影响运算结果，效率优先

### 12、const成员函数

- 成员函数声明后面，加上const

  ```c++
  void printInfo(void) const;  //表示此函数没有修改操作，只是单纯地读取
  ```

- const对象只能调用const成员函数

## 二、面向对象编程

### 1、封装

#### 抽象

抽象出数据成员、成员函数

#### 访问控制

##### 类本身

- private : 外界不可见，不能直接访问
- protectec： 外界不可见，不能直接访问，子类可以访问
- public：外界可以直接访问

##### 继承访问控制

- 无论哪种继承方式，在派生类内部使用父类时并无差别
- 不同的继承方式，会影响这两方面：外部代码对派生类的使用、派生类的子类

### 2、继承

```cpp
class Father 
private:
	int money;
public:
	int getMoney(void);
	void it_skill(void);
;
class Son : public Father   
private:
	int toy;
;

Son s;
s.it_skill();
s.getMoney();

```

| 类型     |                             描述                             | 代码示例                                                     |
| -------- | :----------------------------------------------------------: | ------------------------------------------------------------ |
| 公有继承 |          基类的公有成员和保护成员在派生类中均可访问          | `class Base { public: int a; }; class Derived : public Base { };` |
| 私有继承 | 基类的公有成员和保护成员在派生类中均不可访问，只能通过基类的公有成员函数访问 | `class Base { public: int a; }; class Derived : private Base { };` |
| 保护继承 | 基类的公有成员和保护成员在派生类中均可访问，但不能在派生类对象外访问 | `class Base { public: int a; }; class Derived : protected Base { };` |

#### 继承访问控制

1. 不能直接拿父亲的私房钱：派生类不能访问基类的私有成员
2. 可以问父亲要钱：通过protected/public的成员函数
3. 儿子总是比外人亲：派生类可以访问父类的protected成员，其他代码不可以

访问规则：

![image-20230306081033887](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20230306081033887.png)


#### 派生类对象的空间

儿子继承来的财产，他可以捐给别人，也可以私藏(前提：Son看得见这些财产)：

Son可见到的成员，Son可以修改它的权限。

`private: using Father::getMoney;`

### 多继承

#### 一个派生类可以有多个基类

#### 二义性

#### 虚拟继承

### 构造

#### 构造顺序

#### 析构顺序

### 3、多态

#### 多态的引入

#### 多态的限制

#### 类型转换





## 三、高级编程

## 四、设计模式

