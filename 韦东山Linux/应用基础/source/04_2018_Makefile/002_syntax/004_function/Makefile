
A = a b c 
B = $(foreach f, $(A), $(f).o)


C = a b c d/

D = $(filter %/, $(C))
E = $(filter-out %/, $(C))

files = $(wildcard *.c)


files2 = a.c b.c c.c d.c e.c  abc
files3 = $(wildcard $(files2))

dep_files = $(patsubst %.c,%.d,$(files2))

all:
	@echo B = $(B)
	@echo D = $(D)
	@echo E = $(E)
	@echo files = $(files)
	@echo files3 = $(files3)
	@echo dep_files = $(dep_files)