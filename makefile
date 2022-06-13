CC=gcc
CFLAGS=-Wextra -Wall -pedantic -Werror
OBJS=main.o search_path.o get_input.o parse_input.o builtins.o variables.o envp.o helpfunc.o erro.o

shell: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS)
