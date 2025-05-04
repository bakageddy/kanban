main: main.c ./lib/libraylib.a
	gcc -o main main.c -L./lib/ -l:libraylib.a -I./include/ -lm -ggdb

kanban: kanban.c ./lib/libraylib.a
	gcc -o kanban kanban.c -L./lib/ -l:libraylib.a -I./include/ -lm -ggdb
