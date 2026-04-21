all: selection_sort

selection_sort: main.o array_filter.o array_sort.o array_std.o swap.o
	gcc -o selection_sort main.o array_filter.o array_sort.o array_std.o swap.o

array_filter.o: array_filter.c
array_sort.o: array_sort.c swap.h array_filter.h
array_std.o: array_std.c array_std.h
main.o: main.c array_std.h array_sort.h
swap.o: swap.c