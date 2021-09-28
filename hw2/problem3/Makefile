SOURCE = $(wildcard *.c)
TARGETS = $(patsubst %.c, %, $(SOURCE))

CC = gcc
CFLAGS = -g -O3

all:$(TARGETS)

$(TARGETS):%:%.c
	$(CC) $(CFLAGS) -o $@ $^
.PHONY:clean all
clean:
	-rm -rf $(TARGETS)