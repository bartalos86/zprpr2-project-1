TASK := main.c

compile:
	gcc $(TASK) -std=c99 -pedantic -Wall  -Wextra -o compiled

run:
	clear && gcc $(TASK) -std=c99 -pedantic -Wall  -Wextra -o compiled && ./compiled