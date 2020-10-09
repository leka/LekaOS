# Tests

## About

This document explains the different kinds of tests used in the development of LekaOS.

## Unit Tests

Unit tests (located in [`tests/unit`](../../tests/unit)) are intended to run on the host computer. 

Please refer to mbed-os [Unit Testing documentation](https://os.mbed.com/docs/mbed-os/v6.3/debug-test/unit-testing.html).

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


