main: main.c examples/*
	cc -Wall -Werror -O3 -o main main.c
debug: main.c examples/*
	cc -Wall -Werror -O3 -o debug main.c -g
	
