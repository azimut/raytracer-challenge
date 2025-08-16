CC      := gcc
CFLAGS	:= -Wall -Wextra -std=gnu99 -pedantic -ggdb
LDFLAGS := -lm
SRC     := $(wildcard src/*.c)
HDR     := $(wildcard src/*.h)

.PHONY: test
test: test/unit.c
	$(CC) $(CFLAGS) -o test/unit $(SRC) test/unit.c $(LDFLAGS)
	./test/unit
