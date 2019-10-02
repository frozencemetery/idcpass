CC = clang
FLAGS = -Og -ggdb -Wall -Wextra
FLAGS += $(shell pkg-config --cflags --libs xtst x11)

xfake: xfake.c
	$(CC) $(FLAGS) xfake.c -o xfake
