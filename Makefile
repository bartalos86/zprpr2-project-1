TASK := main.c

compile:
	gcc $(TASK) -std=c99 -pedantic -Wall -Werror -Wextra -o compiled

run:
	clear && gcc $(TASK) -std=c99 -pedantic -Wall -Werror -Wextra -o compiled && ./compiled