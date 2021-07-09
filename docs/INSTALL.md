# Setup & Installation

## Introduction

LekaOS repository is made to be easy to use:

- simple directory structure
- CMake based with [USCRPL/mbed-cmake](https://github.com/USCRPL/mbed-cmake/)
- Code completion with VSCode + [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)
- format with `.clang-format` and `.editorconfig`

## How to install

Before starting, make sure you've read the [mbed documentation](https://os.mbed.com/docs/mbed-os/v6.8/introduction/index.html).

We'll guide you in the process, just follow the steps.

### 0. Install dependencies & tools

To get up and running, you need a few tools installed on your system.

#### macOS

First install the Command Line Tools and brew: <https://brew.sh/>

```bash
# install CLT
xcode-select --install

# install brew
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

Then install the tools:

```bash
# first install GNU core utilities
brew install coreutils
# don’t forget to add `$(brew --prefix coreutils)/libexec/gnubin` to your `$PATH`

# then the prerequisites
brew install git curl python

# and finally the needed tools
brew install make cmake ninja lcov gcovr stlink ccache open-ocd clang-format
```

#### Ubuntu/Linux

*Note: The following instructions have not really been tested, so you might need to modify the names of the packages.*

```bash
# first update
sudo apt update

# then the prerequisites
sudo apt install build-essential git python-minimal

# and finally the needed tools
sudo apt install make cmake ninja-build lcov gcovr stlink ccache open-ocd
```

##### Ubuntu 18.04
*Note: A lot of the errors encountered with this distro come from the out of date software provided by apt-get. If something doesn't seem to work make sure the installed version is the latest one or the one you need.*
```bash
# first update
sudo apt update

# then the prerequisites
sudo apt install build-essential git python-minimal

# then some first pre-compatible tools
sudo apt install make ninja-build lcov gcovr ccache

# then cmake needs to be installed with snap to have the latest version
# apt-get repo isn't up to date
sudo snap install cmake

# then install last version of openocd from git
# move to the folder in which you want to install openocd
# follow https://github.com/ntfreak/openocd to check if these instructions are up to date
git clone git://git.code.sf.net/p/openocd/code openocd
cd openocd/
./bootstrap
./configure
make
sudo make install

# then install the latest version of clang-format
# follow https://apt.llvm.org/ to check if these instructions are up to date
# add the last version to your source list with the following script
bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"

# update the package manager list
sudo apt-get update

# install the desired version (change the 12 if a more recent version is needed)
sudo apt-get install clang-format-12

# then you have to create a new shortcut to allow the "clang-format" command to call clang-format-12
sudo ln -s /usr/lib/llvm-12/bin/clang-format /usr/bin/clang-format
# if this call does not work, check the link to which the clang-format-12 shortcut in /usr/bin/clang-format-12 sends to
```
Then you will need to install the latest version of _st-link_ driver, the recommended way is with the debian package that you can download on this page :
https://github.com/stlink-org/stlink/releases
*You might need to scroll a little bit*

### 1. Clone the repository

To start with:

```bash
# Clone the repository
git clone https://github.com/leka/LekaOS && cd LekaOS

# Clone or curl Mbed OS using the Makefile
make mbed_clone
# or
make mbed_curl

# With mbed_clone You can specify a branch
make mbed_clone BRANCH=master
```

### 2. Install mbed-cli & co

I recommend the manual install. Make sure to follow the instructions from mbed:

> <https://os.mbed.com/docs/mbed-os/v6.3/build-tools/install-and-set-up.html>

```bash
# install mbed-cli and mbed-tools (mbed-cli2)
python3 -m pip install -U --user mbed-cli mbed-tools

# then install the important packages
python3 -m pip install -U --user pyserial intelhex prettytable

# and finally mbed-cli requirements
python3 -m pip install -U --user -r https://raw.githubusercontent.com/ARMmbed/mbed-os/master/requirements.txt
```

### 3. Install arm-none-eabi-gcc

#### macOS

```bash
brew tap ArmMbed/homebrew-formulae
brew install arm-none-eabi-gcc
```

#### Ubuntu/Linux

You can download the toolchain here:

> <https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads>

The file to download should have a name looking like this: `gcc-arm-none-eabi-10-2020-q4-major-x86_64-linux.tar.bz2`, save it to your `$HOME` directory (or anywhere you can remember).

Then do the following (you might need to change the name of the file depending on the version):

```bash
# untar the new package in the directory of your choosing
tar -xjvf gcc-arm-none-eabi-10-2020-q4-major-x86_64-linux.tar.bz2

# add the new toolchain to your path
export PATH=$PATH:$HOME/gcc-arm-none-eabi-10-2020-q4-major-x86_64-linux/bin/

# test it works
arm-none-eabi-gcc --version
```

## How to use

We put together an handy [`Makefile`](./Makefile) to make it easier to configure and compile your projects.

```bash
# first configure the project
make config

# you can also specify a build type (default is Release)
make config BUILD_TYPE=Debug

# the build LekaOS, spikes & functional tests
make
```

### Flashing the board

> ⚠️ // TODO - add instructions for openOCD

```bash
make flash BIN_PATH=_build/path/to/binary_file.bin
```

## LekaOS, spikes & tests

The main idea behind this template is to have your main source files (those for the big project you're working on) under the [`./src`](./src) directory.

Now, you sometimes need to create a simple, very basic example project to test a new features, investigate a bug or try a different solution to a problem.

These can be added to the [`spikes`](./spikes) directory inside their own directory. You'll need at leat a `main.cpp` and a `CMakeLists.txt`. See [`blinky`](./spikes/blinky) for a working example.
