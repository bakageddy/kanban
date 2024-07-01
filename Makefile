main: main.c ./lib/libraylib.a
	gcc -o main main.c -L./lib/ -l:libraylib.a -I./include/ -lm
