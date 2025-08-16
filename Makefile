CC      := gcc
LDFLAGS := -lm
SRC     := $(wildcard src/*.c)
HDR     := $(wildcard src/*.h)
ifdef DEBUG
	CFLAGS := -Wall -Wextra -std=gnu99 -pedantic -g -Og
else
	CFLAGS := -Wall -Wextra -std=gnu99 -pedantic
endif

.PHONY: test
test: test/unit.c
	$(CC) $(CFLAGS) -o test/unit $(SRC) test/unit.c $(LDFLAGS)
	./test/unit
