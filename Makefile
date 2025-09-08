NSAMPLES  ?= 1
CC        ?= gcc
SRC       := $(wildcard lib/*.c)
HDR       := $(wildcard lib/*.h)
#PKGS      := cglm
CFLAGS     = -Wall -Wextra -std=gnu11
CFLAGS    += -DNSAMPLES=$(NSAMPLES)
CFLAGS    += $(shell pkg-config --cflags $(PKGS))
LDFLAGS   := $(shell pkg-config --libs $(PKGS)) -lm
BUILDS    := $(addprefix build/,$(basename $(notdir $(wildcard src/*.c))))
DIMENSION ?= 300

ifneq ($(and $(SIZEX),$(SIZEY)),)
	CFLAGS += -DSIZEX=$(SIZEX) -DSIZEY=$(SIZEY)
else
	CFLAGS += -DDIMENSION=$(DIMENSION)
endif

ifdef DEBUG
	CFLAGS += -ggdb3 -O0
endif
ifdef SANITIZE
	CFLAGS += -fsanitize=undefined -fsanitize=address -fPIE -fno-omit-frame-pointer -ggdb3 -O0
endif
ifdef FAST
	CFLAGS += -march=native -mtune=native -O3
endif
ifdef BLAZE
	CFLAGS += -march=native -mtune=native -Ofast -ffast-math -DBLAZE=$(BLAZE)
endif
ifdef GRADIENT
	CFLAGS += -DGRADIENT=1
endif
ifdef AREA_SAMPLES
	CFLAGS += -DAREA_SAMPLES=$(AREA_SAMPLES)
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
clean: ; rm -vf ./profile.* ./build/* ./media/*.ppm

.PHONY: valgrind # NOTE: needs gcc
valgrind: CFLAGS   += -ggdb3 -O0
valgrind: DIMENSION = 50
valgrind: build/$(TARGET) ; valgrind ./build/$(TARGET)

.PHONY: deps
deps:; sudo apt-get install imagemagick time valgrind libcglm-dev
