# ESP-IDF  V4.0系统构建

[乐鑫系统构建-支持文档](https://docs.espressif.com/projects/esp-idf/zh_CN/stable/esp32/api-guides/build-system.html)

## 一、概述

- **项目配置**
  - 保存在项目根目录下名为 `sdkconfig` 的文件中，可以通过 `idf.py menuconfig` 进行修改，且一个项目只能包含一个项目配置。
- **应用程序**
  - 是由 ESP-IDF 构建得到的可执行文件。一个项目通常会构建两个应用程序：项目应用程序（可执行的主文件，即用户自定义的固件）和引导程序（启动并初始化项目应用程序）。
- **组件（库）**
  -  是模块化且独立的代码，会被编译成 **静态库（.a 文件）**并链接到应用程序。部分组件由 ESP-IDF 官方提供，其他组件则来源于其它开源项目。

## 二、使用构建系统

###  (1) idf.py

 ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326193524.png)

###  (2)创建一个新的项目

运行 `idf.py create-project` 命令可以开始创建的新项目。

```
idf.py create-project --path my_projects my_new_project
```

以上命令会直接在 my_projects 目录下创建一个名为 my_new_project 的新项目。

###  (3)示例项目结构

示例项目的目录树结构可能如下所示:

```
- myProject/
             - CMakeLists.txt
             - sdkconfig
             - components/ - component1/ - CMakeLists.txt
                                         - Kconfig
                                         - src1.c
                           - component2/ - CMakeLists.txt
                                         - Kconfig
                                         - src1.c
                                         - include/ - component2.h
             - main/       - CMakeLists.txt
                           - src1.c
                           - src2.c

             - build/
```

该示例项目 “myProject” 包含以下组成部分：

- 顶层项目 CMakeLists.txt 文件，这是 CMake 用于学习如何构建项目的主要文件，***可以在这个文件中设置项目全局的 CMake 变量***。顶层项目 CMakeLists.txt 文件会导入 [/tools/cmake/project.cmake](https://github.com/espressif/esp-idf/blob/v4.4/tools/cmake/project.cmake) 文件，由它负责实现构建系统的其余部分。该文件最后会设置项目的名称，并定义该项目。
- “sdkconfig” 项目配置文件，执行 `idf.py menuconfig` 时会创建或更新此文件，文件中保存了项目中所有组件（包括 ESP-IDF 本身）的配置信息。 `sdkconfig` 文件可能会也可能不会被添加到项目的源码管理系统中。
- 可选的 “components” 目录中包含了项目的部分自定义组件，并不是每个项目都需要这种自定义组件，但它有助于构建可复用的代码或者导入第三方（不属于 ESP-IDF）的组件。或者，也可以在顶层 CMakeLists.txt 中设置 `EXTRA_COMPONENT_DIRS` 变量以查找其他指定位置处的组件。有关详细信息，请参阅 [重命名 main 组件](https://docs.espressif.com/projects/esp-idf/zh_CN/stable/esp32/api-guides/build-system.html#rename-main)。如果项目中源文件较多，建议将其归于组件中，而不是全部放在 “main” 中。

- “main” 目录是一个特殊的组件，它包含项目本身的源代码。”main” 是默认名称，CMake 变量 `COMPONENT_DIRS` 默认包含此组件，但您可以修改此变量。
- “build” 目录是存放构建输出的地方，如果没有此目录，`idf.py` 会自动创建。CMake 会配置项目，并在此目录下生成临时的构建文件。随后，在主构建进程的运行期间，该目录还会保存临时目标文件、库文件以及最终输出的二进制文件。**此目录通常不会添加到项目的源码管理系统中，也不会随项目源码一同发布。** 

​		每个组件目录都包含一个 `CMakeLists.txt` 文件，里面会定义一些变量以控制该组件的构建过程，以及其与整个项目的集成。更多详细信息请参阅 [组件 CMakeLists 文件](https://docs.espressif.com/projects/esp-idf/zh_CN/stable/esp32/api-guides/build-system.html#component-directories)。

​		每个组件还可以包含一个 `Kconfig` 文件，它用于定义 `menuconfig` 时展示的 [组件配置](https://docs.espressif.com/projects/esp-idf/zh_CN/stable/esp32/api-guides/build-system.html#component-configuration) 选项。某些组件可能还会包含 `Kconfig.projbuild` 和 `project_include.cmake` 特殊文件，它们用于 [覆盖项目的部分设置](https://docs.espressif.com/projects/esp-idf/zh_CN/stable/esp32/api-guides/build-system.html#override-project-config)。

## 三、项目CMakeLists文件

每个项目都有一个顶层 `CMakeLists.txt` 文件，包含整个项目的构建设置。默认情况下，项目 CMakeLists 文件会非常小。

### (1)最小的CMakeLists文件示例

```
cmake_minimum_required(VERSION 3.5)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(myProject)
```

- **必要部分**

  每个项目都要按照上面显示的顺序添加上述三行代码：

  - `cmake_minimum_required(VERSION 3.5)` 必须放在 CMakeLists.txt 文件的第一行，它会告诉 CMake 构建该项目所需要的最小版本号。ESP-IDF 支持 CMake 3.5 或更高的版本。
  - `include($ENV{IDF_PATH}/tools/cmake/project.cmake)` 会导入 CMake 的其余功能来完成配置项目、检索组件等任务。
  - `project(myProject)` 会创建项目本身，并指定项目名称。该名称会作为最终输出的二进制文件的名字，即 `myProject.elf` 和 `myProject.bin`。每个 CMakeLists 文件只能定义一个项目。

- **可选部分**

  以下这些变量都有默认值，用户可以覆盖这些变量值以自定义构建行为。更多实现细节，请参阅 [/tools/cmake/project.cmake](https://github.com/espressif/esp-idf/blob/v4.4/tools/cmake/project.cmake) 文件。

  - `COMPONENT_DIRS`：组件的搜索目录，默认为 `IDF_PATH/components`、 `PROJECT_DIR/components`、和 `EXTRA_COMPONENT_DIRS`。如果您不想在这些位置搜索组件，请覆盖此变量。
  - `EXTRA_COMPONENT_DIRS`：用于搜索组件的其它可选目录列表。路径可以是相对于项目目录的相对路径，也可以是绝对路径。
  - `COMPONENTS`：要构建进项目中的组件名称列表，默认为 `COMPONENT_DIRS` 目录下检索到的所有组件。使用此变量可以“精简”项目以缩短构建时间。请注意，如果一个组件通过 `COMPONENT_REQUIRES` 指定了它依赖的另一个组件，则会自动将其添加到 `COMPONENTS` 中，所以 `COMPONENTS` 列表可能会非常短。

以上变量中的路径可以是绝对路径，或者是相对于项目目录的相对路径。

请使用 [cmake 中的 set 命令](https://cmake.org/cmake/help/v3.5/command/set.html) 来设置这些变量，如 `set(VARIABLE "VALUE")`。请注意，`set()` 命令需放在 `include(...)` 之前，`cmake_minimum(...)` 之后。

### (2)重命名 `main` 组件

构建系统会对 `main` 组件进行特殊处理。假如 `main` 组件位于预期的位置（即 ${PROJECT_PATH}/main），那么它会被自动添加到构建系统中。其他组件也会作为其依赖项被添加到构建系统中，这使用户免于处理依赖关系，并提供即时可用的构建功能。重命名 `main` 组件会减轻上述这些幕后工作量，但要求用户指定重命名后的组件位置，并手动为其添加依赖项。重命名 `main` 组件的步骤如下：

- 重命名 `main` 目录。
- 在项目 CMakeLists.txt 文件中设置 `EXTRA_COMPONENT_DIRS`，并添加重命名后的 `main` 目录。
- 在组件的 CMakeLists.txt 文件中设置 `COMPONENT_REQUIRES` 或 `COMPONENT_PRIV_REQUIRES` 以指定依赖项。

### (3)覆盖默认的构建规范

构建系统设置了一些全局的构建规范（编译标志、定义等），这些规范可用于编译来自所有组件的所有源文件。

例如，其中一个默认的构建规范是编译选项 `Wextra`。假设一个用户想用 `Wno-extra` 来覆盖这个选项， 应在 `project()` 之后进行:

```
cmake_minimum_required(VERSION 3.5)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(myProject)

idf_build_set_property(COMPILE_OPTIONS "-Wno-error" APPEND)
```

这确保了用户设置的编译选项不会被默认的构建规范所覆盖，因为默认的构建规范是在 `project()` 内设置的。

## 四、组件CMakeLists文件

​		每个项目都包含一个或多个组件，这些组件可以是 ESP-IDF 的一部分，可以是项目自身组件目录的一部分，也可以从自定义组件目录添加（[见上文](https://docs.espressif.com/projects/esp-idf/zh_CN/stable/esp32/api-guides/build-system.html#component-directories)）。

​		组件是 `COMPONENT_DIRS` 列表中包含 `CMakeLists.txt` 文件的任何目录。

### (1)搜索组件

​		搜索 `COMPONENT_DIRS` 中的目录列表以查找项目的组件，此列表中的目录可以是组件自身（即包含 CMakeLists.txt 文件的目录），也可以是子目录为组件的顶级目录。

​		当 CMake 运行项目配置时，它会记录本次构建包含的组件列表，它可用于调试某些组件的添加/排除。

### (2)同名组件

​		ESP-IDF 在搜索所有待构建的组件时，会按照 `COMPONENT_DIRS` 指定的顺序依次进行，这意味着在默认情况下，首先搜索 ESP-IDF 内部组件（`IDF_PATH/components`），然后是 `EXTRA_COMPONENT_DIRS` 中的组件，最后是项目组件（`PROJECT_DIR/components`）。如果这些目录中的两个或者多个包含具有相同名字的组件，则使用搜索到的最后一个位置的组件。这就允许将组件复制到项目目录中再修改以覆盖 ESP-IDF 组件，如果使用这种方式，ESP-IDF 目录本身可以保持不变。

​		Note:如果在现有项目中通过将组件移动到一个新位置来覆盖它，项目不会自动看到新组件的路径。请运行 `idf.py reconfigure` 命令后（或删除项目构建文件夹）再重新构建。

### (3)最小组件 CMakeLists 文件

最小组件 `CMakeLists.txt` 文件通过使用 `idf_component_register` 将组件添加到构建系统中。

> - idf_component_register(SRCS “foo.c” “bar.c”
>
>   INCLUDE_DIRS “include” REQUIRES mbedtls)

- `SRCS` 是源文件列表（`*.c`、`*.cpp`、`*.cc`、`*.S`），里面所有的源文件都将会编译进组件库中。
- `INCLUDE_DIRS` 是目录列表，里面的路径会被添加到所有需要该组件的组件（包括 main 组件）全局 include 搜索路径中。
- `REQUIRES` 实际上并不是必需的，但通常需要它来声明该组件需要使用哪些其它组件，请参考 [组件依赖](https://docs.espressif.com/projects/esp-idf/zh_CN/stable/esp32/api-guides/build-system.html#component-requirements)。

上述命令会构建生成与组件同名的库，并最终被链接到应用程序中。

上述目录通常设置为相对于 `CMakeLists.txt` 文件的相对路径，当然也可以设置为绝对路径。

还有其它参数可以传递给 `idf_component_register`，具体可参考 [here](https://docs.espressif.com/projects/esp-idf/zh_CN/stable/esp32/api-guides/build-system.html#cmake-component-register)。

有关更完整的 `CMakeLists.txt` 示例，请参阅 [组件依赖示例](https://docs.espressif.com/projects/esp-idf/zh_CN/stable/esp32/api-guides/build-system.html#id29) 和 [组件 CMakeLists 示例](https://docs.espressif.com/projects/esp-idf/zh_CN/stable/esp32/api-guides/build-system.html#id42)。

### (4)创建新组件

​		使用 `idf.py create-component` 命令创建新组件。 新组件将包含构建组件所需的一组文件。您可以将组件的头文件纳入到您的项目中，并使用其功能。请运行 `idf.py create-component --help` 命令获取更多信息。

示例:

```
idf.py -C components create-component my_component
```

该示例将在当前工作目录下的子目录 components 中创建一个新的组件。更多关于组件的信息，请参考 [上文](https://docs.espressif.com/projects/esp-idf/zh_CN/stable/esp32/api-guides/build-system.html#component-directories)。

### (5)预设的组件变量

以下专用于组件的变量可以在组件 CMakeLists 中使用，但不建议修改：

- `COMPONENT_DIR`：组件目录，即包含 `CMakeLists.txt` 文件的绝对路径，它与 `CMAKE_CURRENT_SOURCE_DIR` 变量一样，路径中不能包含空格。
- `COMPONENT_NAME`：组件名，与组件目录名相同。
- `COMPONENT_ALIAS`：库别名，由构建系统在内部为组件创建。
- `COMPONENT_LIB`：库名，由构建系统在内部为组件创建。

以下变量在项目级别中被设置，但可在组件 CMakeLists 中使用：

- `CONFIG_*`：项目配置中的每个值在 cmake 中都对应一个以 `CONFIG_` 开头的变量。更多详细信息请参阅 [Kconfig](https://docs.espressif.com/projects/esp-idf/zh_CN/stable/esp32/api-reference/kconfig.html)。
- `ESP_PLATFORM`：ESP-IDF 构建系统处理 CMake 文件时，其值设为1。

### (6)构建/项目变量

以下是可作为构建属性的构建/项目变量，可通过组件 CMakeLists.txt 中的 `idf_build_get_property` 查询其变量值。

- `PROJECT_NAME`：项目名，在项目 CMakeLists.txt 文件中设置。
- `PROJECT_DIR`：项目目录（包含项目 CMakeLists 文件）的绝对路径，与 `CMAKE_SOURCE_DIR` 变量相同。
- `COMPONENTS`：此次构建中包含的所有组件的名称，具体格式为用分号隔开的 CMake 列表。
- `IDF_VER`：ESP-IDF 的 git 版本号，由 `git describe` 命令生成。
- `IDF_VERSION_MAJOR`、 `IDF_VERSION_MINOR`、 `IDF_VERSION_PATCH`: ESP-IDF 的组件版本，可用于条件表达式。请注意这些信息的精确度不如 `IDF_VER` 变量，版本号 `v4.0-dev-*`， `v4.0-beta1`， `v4.0-rc1` 和 `v4.0` 对应的 `IDF_VERSION_*` 变量值是相同的，但是 `IDF_VER` 的值是不同的。
- `IDF_TARGET`：项目的硬件目标名称。
- `PROJECT_VER`：项目版本号。
  - 如果设置 [CONFIG_APP_PROJECT_VER_FROM_CONFIG](https://docs.espressif.com/projects/esp-idf/zh_CN/stable/esp32/api-reference/kconfig.html#config-app-project-ver-from-config) 选项，将会使用 [CONFIG_APP_PROJECT_VER](https://docs.espressif.com/projects/esp-idf/zh_CN/stable/esp32/api-reference/kconfig.html#config-app-project-ver) 的值。
  - 或者，如果在项目 CMakeLists.txt 文件中设置了 `PROJECT_VER` 变量，则该变量值可以使用。
  - 或者，如果 `PROJECT_DIR/version.txt` 文件存在，其内容会用作 `PROJECT_VER` 的值。
  - 或者，如果项目位于某个 Git 仓库中，则使用 `git describe` 命令的输出作为 `PROJECT_VER` 的值。
  - 否则，`PROJECT_VER` 的值为 1。

其它与构建属性有关的信息请参考 [这里](https://docs.espressif.com/projects/esp-idf/zh_CN/stable/esp32/api-guides/build-system.html#cmake-build-properties)。

### (7) 组件编译控制

在编译特定组件的源文件时，可以使用 [target_compile_options](https://cmake.org/cmake/help/v3.5/command/target_compile_options.html) 函数来传递编译器选项:

```
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-unused-variable)
```

如果给单个源文件指定编译器标志，可以使用 CMake 的 [set_source_files_properties](https://cmake.org/cmake/help/v3.5/command/set_source_files_properties.html) 命令:

```
set_source_files_properties(mysrc.c
    PROPERTIES COMPILE_FLAGS
    -Wno-unused-variable
)
```

如果上游代码在编译的时候发出了警告，那这么做可能会很有效。

请注意，上述两条命令只能在组件 CMakeLists 文件的 `idf_component_register` 命令之后调用。



## 五、组件配置

​		每个组件都可以包含一个 `Kconfig` 文件，和 `CMakeLists.txt` 放在同一目录下。`Kconfig` 文件中包含要添加到该组件配置菜单中的一些配置设置信息。

​		运行 menuconfig 时，可以在 `Component Settings` 菜单栏下找到这些设置。

​		创建一个组件的 Kconfig 文件，最简单的方法就是使用 ESP-IDF 中现有的 Kconfig 文件作为模板，在这基础上进行修改。

​		有关示例请参阅 [添加条件配置](https://docs.espressif.com/projects/esp-idf/zh_CN/stable/esp32/api-guides/build-system.html#add-conditional-config)。

## 六、预处理定义

ESP-IDF 构建系统会在命令行中添加以下 C 预处理器定义：

- `ESP_PLATFORM`：可以用来检测在 ESP-IDF 内发生了构建行为。
- `IDF_VER`：定义 git 版本字符串，例如：`v2.0` 用于标记已发布的版本，`v1.0-275-g0efaa4f` 则用于标记任意某次的提交记录。

## 七、组件依赖

​		编译各个组件时，ESP-IDF 系统会递归评估其依赖项。这意味着每个组件都需要声明它所依赖的组件，即 “requires”。

### (1)编写组件

```
idf_component_register(...
                       REQUIRES mbedtls
                       PRIV_REQUIRES console spiffs)
```

- `REQUIRES` 需要包含所有在当前组件的 **公共头文件**里 #include 的头文件所在的组件。
- `PRIV_REQUIRES` 需要包含被当前组件的**源文件** #include 的头文件所在的组件（除非已经被设置在了 `REQUIRES` 中）。以及是当前组件正常工作必须要链接的组件。
- `REQUIRES` 和 `PRIV_REQUIRES` 的值不能依赖于任何配置选项 (`CONFIG_xxx` 宏)。这是因为在配置加载之前，依赖关系就已经被展开。其它组件变量（比如包含路径或源文件）可以依赖配置选择。
- 如果当前组件除了 [通用组件依赖项](https://docs.espressif.com/projects/esp-idf/zh_CN/stable/esp32/api-guides/build-system.html#id31) 中设置的通用组件（比如 RTOS、libc 等）外，并不依赖其它组件，那么对于上述两个 `REQUIRES` 变量，可以选择其中一个或是两个都不设置。

如果组件仅支持某些硬件目标（`IDF_TARGET` 的值），则可以在 `idf_component_register` 中指定 `REQUIRED_IDF_TARGETS` 来声明这个需求。在这种情况下，如果构建系统导入了不支持当前硬件目标的组件时就会报错。

### (2)组件依赖示例

假设现在有一个 `car` 组件，它需要使用 `engine` 组件，而 `engine` 组件需要使用 `spark_plug` 组件：

```
- autoProject/
             - CMakeLists.txt
             - components/ - car/ - CMakeLists.txt
                                     - car.c
                                     - car.h
                           - engine/ - CMakeLists.txt
                                     - engine.c
                                     - include/ - engine.h
                           - spark_plug/  - CMakeLists.txt
                                          - plug.c
                                          - plug.h
```

#### ①Car 组件

`car.h` 头文件是 `car` 组件的公共接口。该头文件直接包含了 `engine.h`，这是因为它需要使用 `engine.h` 中的一些声明:

```
/* car.h */
#include "engine.h"

#ifdef ENGINE_IS_HYBRID
#define CAR_MODEL "Hybrid"
#endif
```

同时` car.c`也包含了 `car.h`:

```
/* car.c */
#include "car.h"
```

**这代表文件 `car/CMakeLists.txt` 需要声明 `car` 需要 `engine`：**

```
idf_component_register(SRCS "car.c"
                  INCLUDE_DIRS "."
                  REQUIRES engine)
```

- `SRCS` 提供 `car` 组件中源文件列表。
- `INCLUDE_DIRS` 提供该组件公共头文件目录列表，由于 `car.h` 是公共接口，所以这里列出了所有包含了 `car.h` 的目录。
- `REQUIRES` 给出该组件的公共接口所需的组件列表。由于 `car.h` 是一个公共头文件并且包含了来自 `engine` 的头文件，所以我们这里包含 `engine`。这样可以确保任何包含 `car.h` 的其他组件也能递归地包含所需的 `engine.h`。

#### ②Engine 组件

`engine` 组件也有一个公共头文件 `include/engine.h`，但这个头文件更为简单:

```
/* engine.h */
#define ENGINE_IS_HYBRID

void engine_start(void);
```

在 `engine.c` 中执行:

```
/* engine.c */
#include "engine.h"
#include "spark_plug.h"

...
```

在该组件中，`engine` 依赖于 `spark_plug`，但这是私有依赖关系。编译 `engine.c` 需要 `spark_plug.h` 但不需要包含 `engine.h`。

这代表文件 `engine/CMakeLists.txt` 可以使用 `PRIV_REQUIRES`：

```
idf_component_register(SRCS "engine.c"
                  INCLUDE_DIRS "include"
                  PRIV_REQUIRES spark_plug)
```

因此，`car` 组件中的源文件不需要在编译器搜索路径中添加 `spark_plug` include 目录。这可以加快编译速度，避免编译器命令行过于的冗长。

#### ③Spark Plug组件

`spark_plug` 组件没有依赖项，它有一个公共头文件 `spark_plug.h`，但不包含其他组件的头文件。

这代表 `spark_plug/CMakeLists.txt` 文件不需要任何 `REQUIRES` 或 `PRIV_REQUIRES`：

```
idf_component_register(SRCS "spark_plug.c"
                  INCLUDE_DIRS ".")
```

### (3)源文件 Include 目录

每个组件的源文件都是用这些 Include 路径目录编译的，这些路径在传递给 `idf_component_register` 的参数中指定：

```
idf_component_register(..
                       INCLUDE_DIRS "include"
                       PRIV_INCLUDE_DIRS "other")
```

- 当前组件的 `INCLUDE_DIRS` 和 `PRIV_INCLUDE_DIRS`。
- `REQUIRES` 和 `PRIV_REQUIRES` 参数指定的所有其他组件（即当前组件的所有公共和私有依赖项）所设置的 `INCLUDE_DIRS`。
- 递归列出所有组件 `REQUIRES` 列表中 `INCLUDE_DIRS` 目录（如递归展开这个组件的所有公共依赖项）。

### (4)主要组件依赖项

`main` 组件比较特别，因为它在构建过程中自动依赖所有其他组件。所以不需要向这个组件传递 `REQUIRES` 或 `PRIV_REQUIRES`。有关不再使用 `main` 组件时需要更改哪些内容，请参考 [重命名 main 组件](https://docs.espressif.com/projects/esp-idf/zh_CN/stable/esp32/api-guides/build-system.html#rename-main)。

### (5)通用组件依赖项

为避免重复性工作，各组件都用自动依赖一些“通用” IDF 组件，即使它们没有被明确提及。这些组件的头文件会一直包含在构建系统中。

通用组件包括：cxx、newlib、freertos、esp_hw_support、heap、log、lwip、soc、hal、esp_rom、esp_common、esp_system。

### (6)在构建中导入组件

- 默认情况下，每个组件都会包含在构建系统中。
- 如果将 `COMPONENTS` 变量设置为项目直接使用的最小组件列表，那么构建系统会扩展到包含所有组件。完整的组件列表为：
  - `COMPONENTS` 中明确提及的组件。
  - 这些组件的依赖项（以及递归运算后的组件）。
  - 每个组件都依赖的通用组件。
- 将 `COMPONENTS` 设置为所需组件的最小列表，可以显著减少项目的构建时间。

### (7) 构建系统中依赖处理的实现细节

- 在 CMake 配置进程的早期阶段会运行 `expand_requirements.cmake` 脚本。该脚本会对所有组件的 CMakeLists.txt 文件进行局部的运算，得到一张组件依赖关系图（[此图可能会有闭环](https://docs.espressif.com/projects/esp-idf/zh_CN/stable/esp32/api-guides/build-system.html#component-circular-dependencies)）。此图用于在构建目录中生成 `component_depends.cmake` 文件。
- CMake 主进程会导入该文件，并以此来确定要包含到构建系统中的组件列表（内部使用的 `BUILD_COMPONENTS` 变量）。`BUILD_COMPONENTS` 变量已排好序，依赖组件会排在前面。由于组件依赖关系图中可能存在闭环，因此不能保证每个组件都满足该排序规则。如果给定相同的组件集和依赖关系，那么最终的排序结果应该是确定的。
- CMake 会将 `BUILD_COMPONENTS` 的值以 “Component names:” 的形式打印出来。
- 然后执行构建系统中包含的每个组件的配置。
- 每个组件都被正常包含在构建系统中，然后再次执行 CMakeLists.txt 文件，将组件库加入构建系统。

#### 组件依赖顺序

`BUILD_COMPONENTS` 变量中组件的顺序决定了构建过程中的其它顺序，包括：

- 项目导入 [project_include.cmake](https://docs.espressif.com/projects/esp-idf/zh_CN/stable/esp32/api-guides/build-system.html#project-include-cmake) 文件的顺序。
- 生成用于编译（通过 `-I` 参数）的头文件路径列表的顺序。请注意，对于给定组件的源文件，仅需将该组件的依赖组件的头文件路径告知编译器。

### (8) 覆盖项目的部分设置

#### project_include.cmake

如果组件的某些构建行为需要在组件 CMakeLists 文件之前被执行，您可以在组件目录下创建名为 `project_include.cmake` 的文件，`project.cmake` 在运行过程中会导入此 CMake 文件。

`project_include.cmake` 文件在 ESP-IDF 内部使用，以定义项目范围内的构建功能，比如 `esptool.py` 的命令行参数和 `bootloader` 这个特殊的应用程序。

与组件 `CMakeLists.txt` 文件有所不同，在导入``project_include.cmake`` 文件的时候，当前源文件目录（即 `CMAKE_CURRENT_SOURCE_DIR``和工作目录）为项目目录。如果想获得当前组件的绝对路径，可以使用 ``COMPONENT_PATH` 变量。

请注意，`project_include.cmake` 对于大多数常见的组件并不是必需的。例如给项目添加 include 搜索目录，给最终的链接步骤添加 `LDFLAGS` 选项等等都可以通过 `CMakeLists.txt` 文件来自定义。详细信息请参考 [可选的项目变量](https://docs.espressif.com/projects/esp-idf/zh_CN/stable/esp32/api-guides/build-system.html#optional-project-variable)。

`project_include.cmake` 文件会按照 `BUILD_COMPONENTS` 变量中组件的顺序（由 CMake 记录）依次导入。即只有在当前组件所有依赖组件的 `project_include.cmake` 文件都被导入后，当前组件的 `project_include.cmake` 文件才会被导入，除非两个组件在同一个依赖闭环中。如果某个 `project_include.cmake` 文件依赖于另一组件设置的变量，则要特别注意上述情况。更多详情请参阅 [构建系统中依赖处理的实现细节](https://docs.espressif.com/projects/esp-idf/zh_CN/stable/esp32/api-guides/build-system.html#component-requirements-implementation)。

在 `project_include.cmake` 文件中设置变量或目标时要格外小心，这些值被包含在项目的顶层 CMake 文件中，因此他们会影响或破坏所有组件的功能。

#### KConfig.projbuild

与 `project_include.cmake` 类似，也可以为组件定义一个 KConfig 文件以实现全局的 [组件配置](https://docs.espressif.com/projects/esp-idf/zh_CN/stable/esp32/api-guides/build-system.html#component-configuration)。如果要在 menuconfig 的顶层添加配置选项，而不是在 “Component Configuration” 子菜单中，则可以在 `CMakeLists.txt` 文件所在目录的 KConfig.projbuild 文件中定义这些选项。

在此文件中添加配置时要小心，因为这些配置会包含在整个项目配置中。在可能的情况下，请为 [组件配置](https://docs.espressif.com/projects/esp-idf/zh_CN/stable/esp32/api-guides/build-system.html#component-configuration) 创建 KConfig 文件。

`project_include.cmake` 文件在 ESP-IDF 内部使用，以定义项目范围内的构建功能，比如 `esptool.py` 的命令行参数和 `bootloader` 这个特殊的应用程序。

#### 仅配置组件

仅配置组件是一类不包含源文件的特殊组件，仅包含 `Kconfig.projbuild`、`KConfig` 和 `CMakeLists.txt` 文件，该 `CMakeLists.txt` 文件仅有一行代码，调用了 `idf_component_register()` 函数。此函数会将组件导入到项目构建中，但不会构建任何库，也不会将头文件添加到任何 include 搜索路径中。

## 八、组件CMakeLists示例

​		因为构建环境试图设置大多数情况都能工作的合理默认值，所以组件 `CMakeLists.txt` 文件可能非常小，甚至是空的，请参考 [最小组件 CMakeLists 文件](https://docs.espressif.com/projects/esp-idf/zh_CN/stable/esp32/api-guides/build-system.html#minimum-cmakelists)。但有些功能往往需要覆盖 [预设的组件变量](https://docs.espressif.com/projects/esp-idf/zh_CN/stable/esp32/api-guides/build-system.html#preset-component-variables) 才能实现。

​		以下是组件 CMakeLists 文件的更高级的示例。

### (1)添加条件配置

配置系统可用于根据项目配置中选择的选项有条件地编译某些文件。

`Kconfig`:

```
config FOO_ENABLE_BAR
    bool "Enable the BAR feature."
    help
        This enables the BAR feature of the FOO component.
```

`CMakeLists.txt`:

```
 set(srcs "foo.c" "more_foo.c")

 if(CONFIG_FOO_ENABLE_BAR)
     list(APPEND srcs "bar.c")
 endif()

idf_component_register(SRCS "${srcs}"
                     ...)
```

上述示例使用了 CMake 的 [if](https://cmake.org/cmake/help/v3.5/command/if.html) 函数和 [list APPEND](https://cmake.org/cmake/help/v3.5/command/list.html) 函数。

也可用于选择或删除某一实现，如下所示：

`Kconfig`:

```
config ENABLE_LCD_OUTPUT
    bool "Enable LCD output."
    help
        Select this if your board has a LCD.

config ENABLE_LCD_CONSOLE
    bool "Output console text to LCD"
    depends on ENABLE_LCD_OUTPUT
    help
        Select this to output debugging output to the lcd

config ENABLE_LCD_PLOT
    bool "Output temperature plots to LCD"
    depends on ENABLE_LCD_OUTPUT
    help
        Select this to output temperature plots
```

`CMakeLists.txt`:

```
if(CONFIG_ENABLE_LCD_OUTPUT)
   set(srcs lcd-real.c lcd-spi.c)
else()
   set(srcs lcd-dummy.c)
endif()

# 如果启用了控制台或绘图功能，则需要加入字体
if(CONFIG_ENABLE_LCD_CONSOLE OR CONFIG_ENABLE_LCD_PLOT)
   list(APPEND srcs "font.c")
endif()

idf_component_register(SRCS "${srcs}"
                    ...)
```

### (2)硬件目标的条件判断

CMake 文件可以使用 `IDF_TARGET` 变量来获取当前的硬件目标。

此外，如果当前的硬件目标是 `xyz` （即 `IDF_TARGET=xyz`），那么 Kconfig 变量 `CONFIG_IDF_TARGET_XYZ` 同样也会被设置。

请注意，组件可以依赖 `IDF_TARGET` 变量，但不能依赖这个 Kconfig 变量。同样也不可在 CMake 文件的 `include` 语句中使用 Kconfig 变量，在这种上下文中可以使用 `IDF_TARGET`。

### (3)生成源代码

有些组件的源文件可能并不是由组件本身提供，而必须从另外的文件生成。假设组件需要一个头文件，该文件由 BMP 文件转换后（使用 bmp2h 工具）的二进制数据组成，然后将头文件包含在名为 graphics_lib.c 的文件中:

```
add_custom_command(OUTPUT logo.h
     COMMAND bmp2h -i ${COMPONENT_DIR}/logo.bmp -o log.h
     DEPENDS ${COMPONENT_DIR}/logo.bmp
     VERBATIM)

add_custom_target(logo DEPENDS logo.h)
add_dependencies(${COMPONENT_LIB} logo)

set_property(DIRECTORY "${COMPONENT_DIR}" APPEND PROPERTY
     ADDITIONAL_MAKE_CLEAN_FILES logo.h)
```

这个示例改编自 [CMake 的一则 FAQ](https://gitlab.kitware.com/cmake/community/-/wikis/FAQ#how-can-i-generate-a-source-file-during-the-build)，其中还包含了一些同样适用于 ESP-IDF 构建系统的示例。

这个示例会在当前目录（构建目录）中生成 logo.h 文件，而 logo.bmp 会随组件一起提供在组件目录中。因为 logo.h 是一个新生成的文件，一旦项目需要清理，该文件也应该要被清除。因此，要将该文件添加到 [ADDITIONAL_MAKE_CLEAN_FILES](https://cmake.org/cmake/help/v3.5/prop_dir/ADDITIONAL_MAKE_CLEAN_FILES.html) 属性中。

Note: 如果需要生成文件作为项目 CMakeLists.txt 的一部分，而不是作为组件 CMakeLists.txt 的一部分，此时需要使用 `${PROJECT_PATH}` 替代 `${COMPONENT_DIR}`，使用 `${PROJECT_NAME}.elf` 替代 `${COMPONENT_LIB}`。

如果某个源文件是从其他组件中生成，且包含 `logo.h` 文件，则需要调用 `add_dependencies`， 在这两个组件之间添加一个依赖项，以确保组件源文件按照正确顺序进行编译。

### (4)嵌入二进制数据

有时您的组件希望使用一个二进制文件或者文本文件，但是您又不希望将它们重新格式化为 C 源文件。

这时，您可以在组件注册中指定 `EMBED_FILES` 参数，用空格分隔要嵌入的文件名称:

```
idf_component_register(...
                       EMBED_FILES server_root_cert.der)
```

或者，如果文件是字符串，则可以使用 `EMBED_TXTFILES` 变量，把文件的内容转成以 null 结尾的字符串嵌入:

```
idf_component_register(...
                       EMBED_TXTFILES server_root_cert.pem)
```

文件的内容会被添加到 Flash 的 .rodata 段，用户可以通过符号名来访问，如下所示:

```
extern const uint8_t server_root_cert_pem_start[] asm("_binary_server_root_cert_pem_start");
extern const uint8_t server_root_cert_pem_end[]   asm("_binary_server_root_cert_pem_end");
```

符号名会根据文件全名生成，如 `EMBED_FILES` 中所示，字符 `/`、`.` 等都会被下划线替代。符号名称中的 _binary 前缀由 objcopy 命令添加，对文本文件和二进制文件都是如此。

如果要将文件嵌入到项目中，而非组件中，可以调用 `target_add_binary_data` 函数:

```
target_add_binary_data(myproject.elf "main/data.bin" TEXT)
```

并将这行代码放在项目 CMakeLists.txt 的 `project()` 命令之后，修改 `myproject.elf` 为你自己的项目名。如果最后一个参数是 `TEXT`，那么构建系统会嵌入以 null 结尾的字符串，如果最后一个参数被设置为 `BINARY`，则将文件内容按照原样嵌入。

有关使用此技术的示例，请查看 file_serving 示例 [protocols/http_server/file_serving/main/CMakeLists.txt](https://github.com/espressif/esp-idf/blob/v4.4/examples/protocols/http_server/file_serving/main/CMakeLists.txt) 中的 main 组件，两个文件会在编译时加载并链接到固件中。

也可以嵌入生成的文件:

```
add_custom_command(OUTPUT my_processed_file.bin
                  COMMAND my_process_file_cmd my_unprocessed_file.bin)
target_add_binary_data(my_target "my_processed_file.bin" BINARY)
```

上述示例中，`my_processed_file.bin` 是通过命令 `my_process_file_cmd` 从文件 `my_unprocessed_file.bin` 中生成，然后嵌入到目标中。

使用 `DEPENDS` 参数来指明对目标的依赖性:

```
add_custom_target(my_process COMMAND ...)
target_add_binary_data(my_target "my_embed_file.bin" BINARY DEPENDS my_process)
```

`target_add_binary_data` 的 `DEPENDS` 参数确保目标首先执行。

### (5)代码和数据的存放

### (6)完全覆盖组件的构建过程

当然，在有些情况下，上面提到的方法不一定够用。如果组件封装了另一个第三方组件，而这个第三方组件并不能直接在 ESP-IDF 的构建系统中工作，在这种情况下，就需要放弃 ESP-IDF 的构建系统，改为使用 CMake 的 [ExternalProject](https://cmake.org/cmake/help/v3.5/module/ExternalProject.html) 功能。组件 CMakeLists 示例如下:

```
# 用于 quirc 的外部构建过程，在源目录中运行
# 并生成 libquirc.a
externalproject_add(quirc_build
    PREFIX ${COMPONENT_DIR}
    SOURCE_DIR ${COMPONENT_DIR}/quirc
    CONFIGURE_COMMAND ""
    BUILD_IN_SOURCE 1
    BUILD_COMMAND make CC=${CMAKE_C_COMPILER} libquirc.a
    INSTALL_COMMAND ""
    )

# 将 libquirc.a 添加到构建系统中
add_library(quirc STATIC IMPORTED GLOBAL)
add_dependencies(quirc quirc_build)

set_target_properties(quirc PROPERTIES IMPORTED_LOCATION
    ${COMPONENT_DIR}/quirc/libquirc.a)
set_target_properties(quirc PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
    ${COMPONENT_DIR}/quirc/lib)

set_directory_properties( PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES
    "${COMPONENT_DIR}/quirc/libquirc.a")
```

（上述 CMakeLists.txt 可用于创建名为 `quirc` 的组件，该组件使用自己的 Makefile 构建 [quirc](https://github.com/dlbeer/quirc) 项目。）

- `externalproject_add` 定义了一个外部构建系统。

  - 设置 `SOURCE_DIR`、`CONFIGURE_COMMAND`、`BUILD_COMMAND` 和 `INSTALL_COMMAND`。如果外部构建系统没有配置这一步骤，可以将 `CONFIGURE_COMMAND` 设置为空字符串。在 ESP-IDF 的构建系统中，一般会将 `INSTALL_COMMAND` 变量设置为空。
  - 设置 `BUILD_IN_SOURCE`，即构建目录与源目录相同。否则，您也可以设置 `BUILD_DIR` 变量。
  - 有关 `externalproject_add()` 命令的详细信息，请参阅 [ExternalProject](https://cmake.org/cmake/help/v3.5/module/ExternalProject.html)。

- 第二组命令添加了一个目标库，指向外部构建系统生成的库文件。为了添加 include 目录，并告知 CMake 该文件的位置，需要再设置一些属性。

- 最后，生成的库被添加到 [ADDITIONAL_MAKE_CLEAN_FILES](https://cmake.org/cmake/help/v3.5/prop_dir/ADDITIONAL_MAKE_CLEAN_FILES.html) 中。即执行 `make clean` 后会删除该库。请注意，构建系统中的其他目标文件不会被删除。

  Note:当外部构建系统使用 PSRAM 时，请记得将 `-mfix-esp32-psram-cache-issue` 添加到 C 编译器的参数中。关于该标志的更多详细信息，请参考 [CONFIG_SPIRAM_CACHE_WORKAROUND](https://docs.espressif.com/projects/esp-idf/zh_CN/stable/esp32/api-reference/kconfig.html#config-spiram-cache-workaround)。

## 九、自定义sdkconfig的默认值

对于示例工程或者其他您不想指定完整 sdkconfig 配置的项目，但是您确实希望覆盖 ESP-IDF 默认值中的某些键值，则可以在项目中创建 `sdkconfig.defaults` 文件。重新创建新配置时将会用到此文件，另外在 `sdkconfig` 没有设置新配置值时，上述文件也会被用到。

如若需要覆盖此文件的名称或指定多个文件，请设置 `SDKCONFIG_DEFAULTS` 环境变量或在顶层 CMakeLists.txt 文件中设置 `SDKCONFIG_DEFAULTS`。在指定多个文件时，使用分号作为分隔符。未指定完整路径的文件名将以当前项目的相对路径来解析。

一些 IDF 示例中包含了 `sdkconfig.ci` 文件。该文件是 CI（持续集成）测试框架的一部分，在正常构建过程中会被忽略。

### 依赖于硬件目标的 sdkconfig 默认值

除了 `sdkconfig.defaults` 之外，构建系统还将从 `sdkconfig.defaults.TARGET_NAME` 文件加载默认值，其中 `IDF_TARGET` 的值为 `TARGET_NAME`。例如，对于 `ESP32` 这个硬件目标，sdkconfig 的默认值会首先从 `sdkconfig.defaults` 获取，然后再从 `sdkconfig.defaults.esp32` 获取。

如果使用 `SDKCONFIG_DEFAULTS` 覆盖了 sdkconfig 默认文件的名称，则硬件目标的 sdkconfig 默认文件名也会从 `SDKCONFIG_DEFAULTS` 值中派生。
