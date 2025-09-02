CC        ?= clang
LDFLAGS   := -lm
SRC       := $(wildcard lib/*.c)
HDR       := $(wildcard lib/*.h)
DIMENSION ?= 1024
CFLAGS     = -Wall -Wextra -std=gnu11 -DDIMENSION=$(DIMENSION)
BUILDS    := $(addprefix build/,$(basename $(notdir $(wildcard src/*.c))))

ifdef DEBUG
	CFLAGS += -ggdb3 -O0
endif
ifdef SANITIZE
	CFLAGS += -fsanitize=undefined -fsanitize=address -fPIE -fno-omit-frame-pointer -ggdb3 -O0
endif
ifdef FAST
	CFLAGS += -march=native -mtune=native -O3
#       CFLAGS += -Ofast -ffast-math
endif

media/thumbs/%.jpg: media/%.jpg ; convert $< -resize '240x' $@
media/%.jpg:        media/%.ppm ; convert $< $@
media/%.ppm:        build/%     ; time $<
$(BUILDS): build/%: src/%.c $(SRC)
	mkdir -p $(@D); $(CC) $(CFLAGS) -o $@ $(SRC) $< $(LDFLAGS)

.PHONY: test
test: build/unit; ./build/unit
build/unit: test/unit.c $(SRC) $(HDR)
	mkdir -p $(@D); $(CC) $(CFLAGS) -o $@ $(SRC) $< $(LDFLAGS)

.PHONY: profile
profile: profile.png
gmon.out:    CFLAGS   += -pg
gmon.out:    DIMENSION = 100
gmon.out:    build/$(TARGET)  ; ./build/$(TARGET)
profile.txt: gmon.out         ; gprof ./build/$(TARGET) $< > $@
profile.dot: profile.txt      ; gprof2dot               $< > $@
profile.png: profile.dot      ; dot -Tpng             < $< > $@

.PHONY: clean
clean: ; rm -f ./profile.* ./build/*

.PHONY: valgrind
valgrind: CFLAGS   += -ggdb3 -O0
valgrind: DIMENSION = 50
valgrind: CC        = gcc
valgrind: build/$(TARGET) ; valgrind ./build/$(TARGET)


.PHONY: deps
deps:; sudo apt-get install imagemagick time valgrind
