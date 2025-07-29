main.exe: main.o alpha_number_tree.o
	gcc main.o alpha_number_tree.o -o main.exe

main.o: main.c alpha_number_tree.h
	gcc -g -c main.c -o main.o

alpha_number_tree.o: alpha_number_tree.c alpha_number_tree.h
	gcc -g -c alpha_number_tree.c -o alpha_number_tree.o
