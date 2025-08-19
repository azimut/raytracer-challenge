CC      := gcc
LDFLAGS := -lm
SRC     := $(wildcard src/*.c)
HDR     := $(wildcard src/*.h)
ifdef DEBUG
	CFLAGS := -Wall -Wextra -std=gnu99 -pedantic -ggdb3 -O0
else
	CFLAGS := -Wall -Wextra -std=gnu99 -pedantic
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
