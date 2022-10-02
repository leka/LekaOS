# Tests

## About

This document explains the different kinds of tests used in the development of LekaOS.

## Unit Tests

Unit tests (located in [`tests/unit`](../../tests/unit)) are intended to run on the host computer.

LekaOS unit tests use GoogleTest and gMock. Please refer to their respectif documentation:

- [GoogleTest Github repository](https://github.com/google/googletest)
- [GoogleTest Introduction](https://google.github.io/googletest/primer.html)
- [gMock for Dummies](https://google.github.io/googletest/gmock_for_dummies.html)
- [gMock Cookbook](https://google.github.io/googletest/gmock_cook_book.html)

### Setup

To build and run the unit tests you need to:

```bash
# Install brew deps
brew install gcovr lcov
```

### Build & Run

To build and run unit tests, you need to:

```bash
# Config unit tests
make config_unit_tests

# Build & run
make ut

# Build, run & generate coverage
make utc
```
