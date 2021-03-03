# Tools

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
ruby tools/ioc_editor.rb --check-labels targets/TARGET_LEKA_V1.0_DEV/pins/1.0.0/Leka-MCU_Pins-v1.0.0.ioc
```

## PinNames.h generator

In order to access the Leka defined pin names, they must be added to the target's `PinNames.h` file.

The [`tools/pin_names_generator.rb`](../../tools/pin_names_generator.rb) script does just that!

Run the following:

```bash
ruby tools/pin_names_generator.rb targets/TARGET_LEKA_V1_0_DEV/pins/1.1.0/Leka-MCU_Pins-v1.1.0.reference.csv targets/TARGET_LEKA_V1_0_DEV
```
