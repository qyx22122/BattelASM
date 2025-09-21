main: main.c memmon.c examples/*
	cc -Wall -Werror -O3 -std=c23 -lraylib -o main main.c
debug: main.c examples/*
	cc -Wall -Werror -O3 -std=c23 -o debug main.c -g
	
