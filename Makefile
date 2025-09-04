main: main.c examples/*
	cc -Wall -Werror -O3 -std=c23 -o main main.c
debug: main.c examples/*
	cc -Wall -Werror -O3 -std=c23 -o debug main.c -g
	
