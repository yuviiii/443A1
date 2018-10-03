CC = gcc
all: create_random_file get_histogram

create_random_file: create_random_file.c
		$(CC) -o $@ $< -lm


get_histogram: get_histogram.c
		$(CC) -o $@ $< -lm

