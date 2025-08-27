CC      := gcc
LDFLAGS := -lm
SRC     := $(wildcard lib/*.c)
HDR     := $(wildcard lib/*.h)
CFLAGS  := -Wall -Wextra -std=gnu11
ifdef DEBUG
	CFLAGS += -ggdb3 -O0
endif
ifdef SANITIZE
	CFLAGS += -fsanitize=undefined -fsanitize=address -fno-omit-frame-pointer
endif

.PHONY: test
test: build/unit; ./build/unit
build/unit: test/unit.c $(SRC) $(HDR)
	$(CC) $(CFLAGS) -o $@ $(SRC) $< $(LDFLAGS)

media/%.jpg: media/%.ppm ; convert $< $@
media/%.ppm: build/%     ; time $<

build/5raysphere:
build/6shading:
build/7world:
build/9plane:
build/%: src/%.c $(SRC)
	$(CC) $(CFLAGS) -o $@ $(SRC) $< $(LDFLAGS)
