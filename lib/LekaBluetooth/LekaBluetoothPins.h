/**
 * @file LekaBluetoothPins.h
 * @author Yann Locatelli
 *
 * @version 0.0
 * @date 2020-09-23
 *
 * @copyright Copyright (c) 2020
 */

#ifndef _LEKA_OS_LIB_BLUETOOTH_PINS_H_
#define _LEKA_OS_LIB_BLUETOOTH_PINS_H_

#include "mbed.h"

constexpr PinName PIN_BM64_TX	  = PC_6;
constexpr PinName PIN_BM64_RX	  = PC_7;
constexpr PinName PIN_BM64_RST	  = PC_8;
constexpr PinName PIN_BM64_WAKEUP = PG_7;

// #define PIN_BM64_TX PC_6
// #define PIN_BM64_RX PC_7
// #define PIN_BM64_RST PC_8
// #define PIN_BM64_WAKEUP PG_7

#endif