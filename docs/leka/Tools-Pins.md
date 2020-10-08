# Tools

## `.ioc` files editor

The script [`tools/ioc_editor.rb`](../../tools/ioc_editor.rb) can be used to generate/modify the configuraton of an `.ioc` file based on a reference `.csv` file.

It sets up the signals and labels of all the pins.

The reference `.csv` file can be found here:

> [Leka-MCU_Pins.gsheet)](https://www.dropbox.com/scl/fi/kucosyosiw2rfd25cro9y/Leka-MCU_Pins.gsheet)

Run the following:

```
# to get the help
$ ruby tools/ioc_editor.rb --help

# or to get an example
$ ruby tools/ioc_editor.rb --check-labels targets/TARGET_LEKA_V1.0_DEV/pins/1.0.0/Leka-MCU_Pins-v1.0.0.ioc
```
