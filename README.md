# Clay3DS

Clay3DS is a library that makes it easy to use the [Clay](https://github.com/nicbarker/clay) library on the Nintendo 3DS.

## Feature Support

- [x] **Rectangles** • Fully implemented *(including rounded corners)*
- [x] **Borders** • Fully implemented *(including rounded corners)*
- [x] **Scissoring** • Implemented and working correctly
- [x] **Text** • Supported, rendering with the default system font
- [x] **Dual Screens** • Supported, as long as you don't put interactable items on the top screen *(which would not make sense anyway)*
- [x] **Touch Screen** • Implemented and working correctly *(including drag scroll)*
- [x] **Transparency** • Implemented and working correctly
- [ ] **Images** • Not implemented _(yet)_

## Installation

Clay3DS is header only, so just include its header after including `clay.h` and you are good.

You can also use CMake FetchContent if you feel fancy.

## Usage

**LOOK AT THE [EXAMPLES](examples)**

## System Environment

You will need to install [devkitPro](https://devkitpro.org/wiki/Getting_Started) on your system, and install the `3ds-dev` group with the following command:

```sh
dkp-pacman -S 3ds-dev
```

Depending on what you want to do, you may also need to install `cmake` on your system.

## Running the Examples

After you have set up your environment, you can use the following commands to build all the examples.

```sh
# Configure the project using the devkitPro cmake wrapper:
/opt/devkitpro/portlibs/3ds/bin/arm-none-eabi-cmake -S . -DCLAY3DS_BUILD_EXAMPLES=true -B build

# Compile the program in release mode:
cmake -B build -C Release
```

If the compilation succeeds, in the `build` folder you will find the `3dsx` packages you can load on your device.
