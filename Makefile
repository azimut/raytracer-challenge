CC      := gcc
LDFLAGS := -lm
SRC     := $(wildcard src/*.c)
HDR     := $(wildcard src/*.h)
ifdef DEBUG
	CFLAGS := -Wall -Wextra -std=gnu99 -pedantic -g3 -Og
else
	CFLAGS := -Wall -Wextra -std=gnu99 -pedantic
endif

.PHONY: test test/unit
test: test/unit; ./test/unit
test/unit: test/unit.c
	$(CC) $(CFLAGS) -o $@ $(SRC) $< $(LDFLAGS)
