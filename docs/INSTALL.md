# Setup & Installation

## Introduction

LekaOS repository is made to be easy to use:

- simple directory structure
- compile with CMake (based on [USCRPL/mbed-cmake](https://github.com/USCRPL/mbed-cmake/))
- code completion with VSCode + [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) + [clangd/vscode-clangd](https://github.com/clangd/vscode-clangd)
- format with `.clang-format` and `.editorconfig`

## How to install

Before starting, make sure you've read the [mbed documentation](https://os.mbed.com/docs/mbed-os/v6.15/introduction/index.html).

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
# 💡 don’t forget to add `$(brew --prefix coreutils)/libexec/gnubin` to your `$PATH`

# then the prerequisites
brew install git curl python

# and finally the needed tools
brew install make cmake ninja lcov gcovr stlink ccache open-ocd clang-format
```

### 1. Clone the repository

To start with:

```bash
# Clone the repository
git clone https://github.com/leka/LekaOS && cd LekaOS

# Pull dependencies: mbed-os & mcuboot
make pull_deps
```

### 2. Install mbed-cli & co

I recommend the manual install. Make sure to follow the instructions from mbed:

> <https://os.mbed.com/docs/mbed-os/v6.15/build-tools/install-and-set-up.html>

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

## How to use

We put together an handy [`Makefile`](./Makefile) to make it easier to configure and compile your projects.

### Building the project

```bash
# first configure the project
make config

# you can also specify a build type (default is Release)
make config BUILD_TYPE=Debug

# the build os, spikes
make
```

### Flashing the board

Make sure you have [OpenOCD](https://openocd.org/) installed.

```bash
make flash BIN_PATH=_build/path/to/binary_file.bin
```

### Building & running unit tests

```bash
# first configure unit tests
make config_unit_tests

# build and run them
make ut

# if you want to see coverage
make utc
```

## LekaOS, spikes & tests

The firmware of the robot is located in [`./app/os`](./app/os) & [`./app/bootloader`](./app/bootloader).

> `firmware = bootloader + os`

Sometimes, you might need to create a simple, very basic example project to test a new features, investigate a bug or try a different solution to a problem.

These can be added to the [`spikes`](./spikes) directory inside their own directory. You'll need at leat a `main.cpp` and a `CMakeLists.txt`. See [`mbed_blinky`](./spikes/mbed_blinky) for a working example.

## VSCode & useful tools

We use VSCode and a handful of plugins to work on LekaOS:

- [Better C++ Syntax](https://github.com/jeff-hykin/better-cpp-syntax)
- [Better Comments](https://github.com/aaron-bond/better-comments)
- [Clang-Format](https://github.com/xaverh/vscode-clang-format-provider)
- [clangd](https://github.com/clangd/vscode-clangd)
- [CMake](https://github.com/twxs/vs.language.cmake)
- [CMake Tools](https://github.com/microsoft/vscode-cmake-tools)
- [Cortex-Debug](https://github.com/Marus/cortex-debug)
- [EditorConfig for VSCode](https://github.com/editorconfig/editorconfig-vscode)

It is highly recommended to install them for a better experience and to make sure you comply with our coding style.

### Settings

Settings for those plugins are shared in the repos as template files.

To create a local copy of the settings and configure the differents plugins and tools, you must run the following:

```bash
cp .vscode/settings.template.json .vscode/settings.json
```

Then, to configure autocompletion and code checking using `clangd`:

- run `make config_tools` in your terminal
- in VSCode, run `CMake: Configure` from the `Command Palette` (with `Cmd ⌘ + Shift ⇧ + P`)

💡 **Apple Silicon** - If you're working on an Apple Silicon computer, you'll need to edit the `clangd` settings in your local copy of `.vscode/settings.json` and comment/uncomment the `--query-driver=/path/to/arm-none-eadi-*` lines.
