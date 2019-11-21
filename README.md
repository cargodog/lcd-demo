# mk8x-template-project
This project is intended to serve as a template firmware project for the Kinetis MK8x family of microcontrollers.

## Dependencies
1) GNU ARM GCC toolchain ([download](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm))
1) cmake >= v3.12
1) Kinetis MK8x SDK (included as git submodule)
1) [MicroCLI](https://github.com/cargodog/microcli) lib (included as git submodule)
1) MPaland `printf` lib (included as git submodule)

## Build
1) Update all submodules
    * `git submodule update --init && git submodule sync`
1) Make sure `ARMGCC_DIR` environment variable is set to the location you installed the ARM gcc toolchain.
1) Make sure build scripts are executable
    * `chmod +x *.sh`
1) Execute one of the build scripts:
    * `./build_all.sh` - builds both debug and release configurations
    * `./build_debug.sh` - builds debug configuration only
    * `./build_release.sh` - builds release configuration only

## Project Structure
* `./` : Project configs, build/linker scripts, etc
* `./lib/` : external libraries ([more on this](lib/README.md))
* `./src/` : project source code
