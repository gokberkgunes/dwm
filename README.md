# dwm
dwm is a plain, minimal, fast, extensible dynamic window manager. This fork
includes many patches to introduce eye-candy and features. Applied patches are
given as `.diff` files in the `patches` folder.

## Configuration
One should edit `config.h` for keybindings, colors, function calls and so on.

## Installation
1. One has to alter `makeconfig.mk` and `Makefile` as they see fit. Xlib header
   files are required to build.
2. Run below command to compile and install to the system.

```sh
doas make clean install
```
