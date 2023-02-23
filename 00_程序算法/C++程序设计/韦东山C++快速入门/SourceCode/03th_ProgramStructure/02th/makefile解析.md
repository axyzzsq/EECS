# 本例中使用的Makefile

```makefile
person: main.o person.o 
	g++ -o $@ $^
	
%.o : %.cpp
	g++ -c -o $@ $<

clean:
	rm -f *.o person	
```

This is a Makefile, which is a type of file used to automate the build process of a program.

The Makefile defines three targets:

1. "person: main.o person.o" - This is the target that will be built when you run the `make` command. It depends on the `main.o` and `person.o` files, which are the object files for the `main.cpp` and `person.cpp` source files, respectively. The `g++` command is used to link the object files together into an executable named `person`. The `$@` and `$^` are special variables that represent the target and dependencies, respectively.
2. "%.o : %.cpp" - This is a pattern rule that tells `make` how to build any object file that has a `.cpp` source file with the same name. For example, if there is a source file named `foo.cpp`, `make` will use this pattern rule to build an object file named `foo.o`. The `g++` command is used to compile the source file into an object file. The `$@` and `$<` are special variables that represent the target and first dependency, respectively.
3. "clean" - This is a target that can be used to clean up the build directory by removing all object files and the `person` executable. The `rm` command is used to remove files. The `-f` option tells `rm` to ignore any errors that might occur if the files do not exist.

To use this Makefile, you can run the `make` command in the same directory as the Makefile. This will build the `person` executable. If you want to clean up the build directory, you can run the `make clean` command.



## The different between `S^`and `S<`

In a Makefile, `$^` and `$<` are special variables that are used to represent the dependencies of a target.

`$^` is used to represent all of the dependencies of the target, with each dependency separated by a space. This means that if a target has multiple dependencies, `$^` will include all of them. For example, in the Makefile rule:

```makefile
person: main.o person.o
    g++ -o $@ $^
```

`$^` would expand to `main.o person.o`, since those are the dependencies of the `person` target.

On the other hand, `$<` is used to represent only the first dependency of the target. This is useful when a target has only one dependency, or when you only need to reference the first dependency in a rule. For example, in the Makefile rule:

```makefile
%.o: %.cpp
    g++ -c -o $@ $<

```

`$<` would expand to the first dependency of the target, which is always a `.cpp` file in this case.

In summary, `$^` represents all dependencies of a target, while `$<` represents only the first dependency.