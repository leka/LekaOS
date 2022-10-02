# Targets

## About

This folder contains the custom targets used for our development.

- `TARGET_DISCO` - they are based and use the `DISCO STM32F769`
- `TARGET_LEKA_vX.X_DEV` - they are custom dev boards made for Leka. The `vX.X` is the current hardware version and the `_DEV` suffix signifies that it's a development (not a production) board
- `TARGET_LEKA_vX.X_PROD` - they are our custom production boards. The `vX.X` is the current hardware version and the `_PROD` suffix signifies that it's a production board

## Pin Names & Definitions

The `TARGET_LEKA_vX.X_DEV` directories contain a `pins` directory with different versions. The pins' versions are for the pin names and functions, not the hardware connections.
