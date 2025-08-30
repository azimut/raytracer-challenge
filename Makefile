CC        := gcc
LDFLAGS   := -lm
SRC       := $(wildcard lib/*.c)
HDR       := $(wildcard lib/*.h)
DIMENSION ?= 1024
CFLAGS    := -Wall -Wextra -std=gnu11 -DDIMENSION=$(DIMENSION)
BUILDS    := $(addprefix build/,$(basename $(notdir $(wildcard src/*.c))))

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

media/thumbs/%.jpg: media/%.jpg ; convert $< -resize '240x' $@
media/%.jpg:        media/%.ppm ; convert $< $@
media/%.ppm:        build/%     ; time $<
$(BUILDS): build/%: src/%.c $(SRC)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $(SRC) $< $(LDFLAGS)

	$(CC) $(CFLAGS) -o $@ $(SRC) $< $(LDFLAGS)
