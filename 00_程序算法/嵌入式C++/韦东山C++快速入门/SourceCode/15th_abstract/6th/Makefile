
Human: main.o libHuman.so
	g++ -o $@ $< -L./ -lHuman # -L表示库文件的路径, -l表示库文件的名字

%.o : %.cpp
	g++ -fPIC -c -o $@ $< # -fPIC表示生成位置无关的代码

libHuman.so : Englishman.o Chinese.o Human.o
	g++ -shared -o $@ $^  # -shared表示生成动态库

clean:
	rm -f *.o Human	