## Android

- 01-系统架构

  - Linux内核层：Android系统基于Linux内核的特点、优势、驱动等。
  - 硬件抽象层：Android系统如何通过硬件抽象层实现硬件和软件的解耦等。
  - 原生库层：Android系统提供的一些原生库，包括C/C++库、媒体库、图形库等。
  - Android运行时层：Android系统的运行时环境，包括ART、Dalvik、Zygote等。
  - 应用框架层：Android系统提供的应用框架，包括ActivityManager、WindowManager、PackageManager等。
  - 应用层：Android系统提供的一些应用程序，包括浏览器、电话、短信等。

- 02-内存管理
- 03-文件管理
- 04-进程管理
- 05-驱动管理

- 06-应用管理

  - 开发环境：如何搭建和配置Android应用的开发环境，包括IDE、SDK、NDK等。
  - 开发框架：Android应用的开发框架，包括Activity、Service、BroadcastReceiver、ContentProvider等。
  - 开发API：Android应用的开发API，包括UI、数据存储、网络通信、多媒体、传感器等。
  - 开发实例：Android应用的开发实例，包括常见的功能、界面、交互等。

  

- 07-工程技术
  - 调试技术
    - 日志调试：如何使用logcat、dmesg、kmsg等工具查看和分析Android系统的日志信息。
    - 跟踪调试：如何使用strace、ltrace、ftrace等工具跟踪和分析Android系统的系统调用、库函数、内核函数等。
    - 断点调试：如何使用gdb、lldb、kgdb等工具设置和控制Android系统的断点，查看和修改变量、寄存器、内存等。
    - 性能调试：如何使用perf、systrace、traceview等工具测量和分析Android系统的性能指标，如CPU、内存、电源、网络等。
    - 内存调试：如何使用valgrind、memcheck、memleak等工具检测和分析Android系统的内存错误，如泄漏、越界、重复释放等。
  - 分析技术
    - 静态分析：如何使用lint、cppcheck、scan-build等工具对Android系统的源码进行静态分析，检查代码质量、风格、规范等。
    - 动态分析：如何使用asan、tsan、ubsan等工具对Android系统的运行时进行动态分析，检查内存访问、线程同步、未定义行为等。
    - 覆盖率分析：如何使用gcov、lcov等工具对Android系统的测试覆盖率进行分析，统计代码执行情况、覆盖率百分比等。
    - 故障分析：如何使用crashdump、tombstone、anr等工具对Android系统的故障进行分析，收集故障信息、定位故障原因等。
    - 安全分析：如何使用selinux、seccomp等工具对Android系统的安全性进行分析，设置安全策略、检测安全漏洞等。