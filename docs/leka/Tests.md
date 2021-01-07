# Tests

## About

This document explains the different kinds of tests used in the development of LekaOS.

## Unit Tests

Unit tests (located in [`tests/unit`](../../tests/unit)) are intended to run on the host computer.

LekaOS unit tests use GoogleTest and gMock. Please refer to their respectif documentation:

- [GoogleTest Github repository](https://github.com/google/googletest)
- [GoogleTest Introduction](https://github.com/google/googletest/blob/master/googletest/docs/primer.md)
- [gMock for Dummies](https://github.com/google/googletest/blob/master/googlemock/docs/for_dummies.md)
- [gMock Cookbook](https://github.com/google/googletest/blob/master/googlemock/docs/cook_book.md)

### Setup

To build and run the unit tests you need to:

```bash
# Install brew deps
$ brew install gcovr lcov
```

### Build & Run

To build and run unit tests, you need to:

```bash
# Config unit tests
$ make config_unit_tests

# Build & run
$ make ut
```

## Functional Tests

Functional tests (located in [`tests/functional`](../../tests/functional)) are intended to be run on the robot. They included I/O, BLE, scheduling, etc.

Examples:

- motors
- LEDs
- BLE communication

They are straight forward, as simple as possible and focused on very specific functionalities.

If you need to integrate different modules, refer to [App Tests](../app/README.md).

## Behavioral Tests

Behavioral tests (located in [`tests/behavioral`](../../tests/behavioral)) are "multi-functional" tests. They allow us to test how modules work together, define robot's behaviours, and improve concrete part of LekaOS.
They may not `assert` anything but can define things to observe.

Examples:

- reinforcers (LEDs + motors + sounds)
- behaviors
- high-level scheduling

## Certification Tests

Certification tests (located in [`tests/cert`](../../tests/cert)) are intended to be run during the certification process.

For example:

- EMC (Electromagnetic compatibility)
- Security
- Crash tests
- Failure tests


