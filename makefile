CC = gcc
all: create_random_file
create_random_file: create_random_file.c
		$(CC) -o $@ $< -lm


