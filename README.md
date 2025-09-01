# The Ray Tracer Challenge

My source code, written in C.

## Renders

<p float="left" align="middle">
  <a href="media/7world.jpg"><img src="media/thumbs/7world.jpg" width="200" /></a>
  <a href="media/9plane.jpg"><img src="media/thumbs/9plane.jpg" width="200" /></a>
</p>

## Usage

To render an image with the given 1:1 dimensions. `SPEED` adds some CFLAGS that makes the binary run faster.

``` shellsession
$ make media/11reflections.jpg DIMENSION=1024 SPEED=1
```

### Development

To compile&run tests, checking for address memory leaks with `ASAN`.

``` shellsession
$ make -B test SANITIZE=1
```

To profile the given `TARGET` and generate a `gprof2dot` png image.

``` shellsession
$ make -B profile TARGET=11reflections
```

To compile&run `TARGET` with `valgrind` to detect memory issues.

``` shellsession
$ make -B valgrind TARGET=11reflections
```

To compile&run tests, with debug symbols. I let my IDE run part of this command for me to launch its debugger.

``` shellsession
$ make -B test DEBUG=1
```

## Code Overview

- Chapter 1: `tuple.h`
- Chapter 2: `canvas.h` and `color.h`
- Chapter 3: `matrix.h`
- Chapter 4: `transformation.h`
- Chapter 5: `raycasting.h`, `shapes.h`
- Chapter 6: `shading.h`, `material.h`
- Chapter 7: `world.h`, `camera.h`
- Chapter 8: Shadows
- Chapter 9: Planes
- Chapter 10: `patterns.h`

