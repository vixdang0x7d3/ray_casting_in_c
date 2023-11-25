all: vanilla

vanilla: vanilla.c
	gcc vanilla.c -o vanilla -ggdb -Wall -lraylib -lm
