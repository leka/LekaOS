// Mbed OS
// Copyright 2018 ARM Limited
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_BLE_PRETTY_PRINTER_H_
#define _LEKA_OS_LIB_BLE_PRETTY_PRINTER_H_

#include "ble/BLE.h"

namespace leka::ble {

void printError(ble_error_t error, const char *msg);
void printAddress(const ::ble::address_t &addr);
void printMacAddress();
auto phy_to_string(::ble::phy_t phy) -> const char *;

}	// namespace leka::ble

#endif	 // _LEKA_OS_LIB_BLE_PRETTY_PRINTER_H_
