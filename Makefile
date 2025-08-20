CC      := gcc
LDFLAGS := -lm
SRC     := $(wildcard src/*.c)
HDR     := $(wildcard src/*.h)
CFLAGS  := -Wall -Wextra -std=gnu99 -pedantic
ifdef DEBUG
	CFLAGS += -ggdb3 -O0
endif
ifdef SANITIZE
	CFLAGS += -fsanitize=address -fno-omit-frame-pointer
endif

.PHONY: test
test: test/unit; ./test/unit
test/unit: test/unit.c $(SRC) $(HDR)
	$(CC) $(CFLAGS) -o $@ $(SRC) $< $(LDFLAGS)

media/5raysphere.jpg: media/5raysphere.ppm ; convert $< $@
media/5raysphere.ppm: test/5raysphere      ; $< > $@
test/5raysphere: test/5raysphere.c $(SRC)
	$(CC) $(CFLAGS) -o $@ $(SRC) $< $(LDFLAGS)

media/6shading.jpg: media/6shading.ppm ; convert $< $@
media/6shading.ppm: test/6shading      ; $< > $@
test/6shading: test/6shading.c $(SRC)
	$(CC) $(CFLAGS) -o $@ $(SRC) $< $(LDFLAGS)
