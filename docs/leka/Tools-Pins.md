# Tools - `PinMap`, `PinNames` & `.ioc` files edition

## `.ioc` files editor

The script [`tools/ioc_editor.rb`](../../tools/ioc_editor.rb) can be used to generate/modify the configuraton of an `.ioc` file based on a reference `.csv` file.

It sets up the signals and labels of all the pins.

The reference `.csv` file can be found here:

> [Leka-MCU_Pins.gsheet)](https://www.dropbox.com/scl/fi/kucosyosiw2rfd25cro9y/Leka-MCU_Pins.gsheet)

Run the following:

```bash
# to get the help
ruby tools/ioc_editor.rb --help

# or to get an example
export TARGET=TARGET_LEKA_V1_2_DEV; \
ruby tools/ioc_editor.rb --check-labels targets/$TARGET
```

## PinNames.h generator

In order to access the Leka defined pin names, they must be added to the target's `PinNames.h` file.

The [`tools/pin_names_generator.rb`](../../tools/pin_names_generator.rb) script does just that!

Run the following:

```bash
export TARGET=TARGET_LEKA_V1_2_DEV; \
ruby tools/pin_names_generator.rb targets/$TARGET/pins/Leka-MCU_Pins-*.reference.csv targets/$TARGET
```
