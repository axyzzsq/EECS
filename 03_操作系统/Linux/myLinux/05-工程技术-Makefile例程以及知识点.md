# Makefile例程以及知识点

## 例程1

```makefile
######### Makefile COMMON ########
include ../Rules.make
#c 还是 c++  设置源文件的文件扩展名。
EXTENSION = c  

# 设置传感器型号
SENSOR=gc0406_mipi

#设置要构建的可执行程序的名称
EXE = SF2APP
#源文件路径 空格 可以继续添如 src src1 src2
SUBDIR = main audio button camera light network public state_machine ../common
#.设置目标文件.o文件存放路径
OBJ = obj
#.d文件存放路径 设置依赖文件的目录
DEP = dep
 
#预处理选项（要包含的.h文件的路径）添加预处理器标志以包含下面这些目录
CFLAGS += -I$(SAMPLE_TOP_DIR)/MYSF2
CFLAGS += -I$(SAMPLE_TOP_DIR)/MYSF2/public
# easylogger
CFLAGS += -I$(SAMPLE_TOP_DIR)/MYSF2/public/easylogger/inc
CFLAGS += -I$(SAMPLE_TOP_DIR)/MYSF2/public/easylogger/plugins/file

#设置链接的库文件的目录
LDIR = lib/
LIBS = $(SDK_LIBS) -L$(LDIR) $(LDFLAGS) -lacw_mpi -lpthread -lrt -lmipi -lzbar
#将 libcurl 库添加到链接库中
LIBS += -lcurl
#3-online_mp3play_16kamr 将用于音频处理的库添加到链接库中
LIBS += -lfvad -lvo-amrwbenc -lmad -lm 
# ffmpeg 设置静态库的路径
SDK_LIB_DIR_STA = $(LIB_PATH)/static
# 将 ffmpeg 库添加到链接库中
LIBS += -L$(SDK_LIB_DIR_STA) -lavformat -lavcodec -lavutil -lswresample


#设置调试标志
DEBUG = 0
#编译器的选项
#如果调试标志为 0，则设置优化和警告标志
ifeq ($(DEBUG),0)
  CFLAGS 	+= -O2 -Wall
else
  CFLAGS 	+= -O0 -g2 -Wall
endif
 
#后面的内容都不需要修改 
CXX_SOURCES = $(shell find $(SUBDIR) -name '*.c' )  #查找SUBDIR目录下的所有源文件
#$(info CXX_SOURCES: $(CXX_SOURCES))
CXX_OBJECTS = $(patsubst  %.$(EXTENSION), $(OBJ)/%.o, $(notdir $(CXX_SOURCES))) #从源文件创建目标文件
#$(info CXX_OBJECTS: $(CXX_OBJECTS))
DEP_FILES  = $(patsubst  %.$(EXTENSION), $(DEP)/%.d, $(notdir $(CXX_SOURCES))) #从源文件创建依赖文件
#$(info DEP_FILES: $(DEP_FILES))

all: $(EXE) install  # 设置默认目标以构建可执行文件并安装它

$(EXE): $(CXX_OBJECTS) #通过链接目标文件创建可执行文件
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(CXX_OBJECTS) -o $(EXE) $(LIBS) 

$(OBJ)/%.o: $(shell find $(SUBDIR) -name %.$(EXTENSION)) #将源文件编译成目标文件。
	@if [ ! -d $(OBJ) ]; then mkdir -p $(OBJ); fi;
	@$(CC) -c $(CFLAGS) $(CPPFLAGS) -MMD -MP -MF $(patsubst %.o,%.d,$(DEP)/$(notdir $@)) -MT $(patsubst %.o,%.d,$(DEP)/$(notdir $@)) -o $@  $(shell find $(SUBDIR) -name $(patsubst %.o,%.c,$(notdir $@)) )

$(DEP)/%.d: $(shell find $(SUBDIR) -name %.$(EXTENSION))  # 创建依赖文件
	@if [ ! -d $(DEP) ]; then mkdir -p $(DEP); fi;
	@$(CC) $(CFLAGS) $(CPPFLAGS) -MM $(shell find $(SUBDIR) -name $(patsubst %.d,%.c,$(notdir $@)) ) | sed -e 1's,^,$(OBJ)/,' > $@

install:  #将可执行文件复制到指定目录
	sleep 0.3
	-mkdir -p $(INSTALL_DIR)
	chmod 777 $(EXE)
	-cp -a $(EXE) $(INSTALL_DIR)
	if [ -d /home/qmxh/nfsroot/share ]; then cp -a $(EXE) /home/qmxh/nfsroot/share; fi;

ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEP_FILES) #包括依赖文件
endif
 
.PHONY:clean 
clean:
	-rm  -f  $(CXX_OBJECTS)  $(DEP_FILES)  $(EXE)  #删除目标文件、依赖文件和可执行文件
```

如果先输入`make clean`再输入`make`，程序的执行顺序如下：

1. 目标`clean`将被执行，它将删除所有`.o`、`.d`和`EXE`文件。
2. 目标`all`将被执行。这取决于`$(EXE)`和`install`。
3. 目标`$(EXE)`将被执行，这取决于`$(CXX_OBJECTS)`.
4. 目标`$(CXX_OBJECTS)`将被执行。它为变量中指定的所有源文件生成目标文件`$(SUBDIR)`。
5. 目标`$(DEP_FILES)`将被执行。它为变量中指定的所有源文件生成依赖文件`$(SUBDIR)`。
6. 目标`install`将被执行，它将可执行文件安装到指定目录。