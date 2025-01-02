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
- [x] **Images** • Implemented _(but untested)_

## Installation

Clay3DS is header only, so just include its header after including `clay.h` and you are good.

You can also use CMake FetchContent if you feel fancy.

## Usage

**LOOK AT THE [EXAMPLES](examples)**

## Running the Examples

The easiest way to build the examples is to use [Docker](https://www.docker.com) and the provided `Dockerfile`.

### Docker Builds

Install Docker on your system, and then execute the following command in this directory:

```sh
docker build --rm -t clay3ds --output type=tar,dest=artifacts.tar .
```

After the build is finished, a file named `artifacts.tar`, containing all the executables, will be created.

### Manual Builds

If you want to compile the examples manually, you will need to install `git`, `cmake` and [devkitPro](https://devkitpro.org/wiki/Getting_Started) on your system, and install the `3ds-dev` group using the following commands:

```sh
dkp-pacman -S 3ds-dev
```

---

```sh
# Use the CMake wrapper provided by devkitPro to configure the project.
/opt/devkitpro/portlibs/3ds/bin/arm-none-eabi-cmake -S . -DCLAY3DS_BUILD_EXAMPLES=true -B build

# Compile the project in release mode.
cmake -B build -C Release
```

If the compilation succeeds, in the `build` folder you will find the `3dsx` packages you can load on your device.
