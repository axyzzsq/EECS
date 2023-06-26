
Human: main.o Chinese.o Englishman.o
	g++ -o $@ $^ # $^表示所有依赖文件. $@表示目标文件

%.o : %.cpp
	g++ -c -o $@ $< # $<表示第一个依赖文件, $@表示目标文件

clean:
	rm -f *.o Human	# rm -f表示强制删除,不提示