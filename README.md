# LekaOS

## About

Welcome to LekaOS!

![](./docs/assets/leka-alpha.jpeg)

## Installation

Make sure you have installed:

- arm-gcc-none-eabi ([instructions](https://github.com/osx-cross/homebrew-arm#using-the-prebuilt-binaries))
- mbed-cli ([instructions](https://os.mbed.com/docs/mbed-os/v5.15/tools/manual-installation.html))

Then run the following:

```shell
# clone project
git clone https://github.com/leka/LekaOS
cd LekaOS

# import mbed-os
# it may complain about missing pip dependencies and will try to install them
# but might fail with pycryptodome, pyusb, hidapi, cmsis_pack_manager, psutil, click
# it should not be a problem and you can continue
mbed deploy

# build LekaOS for target, e.g. NUCLEO_WB55RG
mbed compile -m NUCLEO_WB55RG

# flash the program
mbed compile -m NUCLEO_WB55RG --flash
```

## Testing

Tests are available in the `test` folder:

- [unit tests](./test/unit)
- [functional tests](./test/functional)
- [app tests](./test/app)

## License

Copyright (c) APF France handicap. All rights reserved.

Licensed under the [Apache-2.0](./LICENSE) license.
