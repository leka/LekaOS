// Mbed OS
// Copyright 2018 ARM Limited
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "PrettyPrinter.h"

#include "LogKit.h"

void leka::ble::printError(ble_error_t error, const char *msg)
{
	log_debug("%s: ", msg);
	switch (error) {
		case BLE_ERROR_NONE:
			log_error("BLE_ERROR_NONE: No error");
			break;
		case BLE_ERROR_BUFFER_OVERFLOW:
			log_error(
				"BLE_ERROR_BUFFER_OVERFLOW: The requested action would cause a buffer overflow and has been "
				"aborted");
			break;
		case BLE_ERROR_NOT_IMPLEMENTED:
			log_error(
				"BLE_ERROR_NOT_IMPLEMENTED: Requested a feature that isn't yet implement or isn't supported by the "
				"target HW");
			break;
		case BLE_ERROR_PARAM_OUT_OF_RANGE:
			log_error("BLE_ERROR_PARAM_OUT_OF_RANGE: One of the supplied parameters is outside the valid range");
			break;
		case BLE_ERROR_INVALID_PARAM:
			log_error("BLE_ERROR_INVALID_PARAM: One of the supplied parameters is invalid");
			break;
		case BLE_STACK_BUSY:
			log_error("BLE_STACK_BUSY: The stack is busy");
			break;
		case BLE_ERROR_INVALID_STATE:
			log_error("BLE_ERROR_INVALID_STATE: Invalid state");
			break;
		case BLE_ERROR_NO_MEM:
			log_error("BLE_ERROR_NO_MEM: Out of Memory");
			break;
		case BLE_ERROR_OPERATION_NOT_PERMITTED:
			log_error("BLE_ERROR_OPERATION_NOT_PERMITTED");
			break;
		case BLE_ERROR_INITIALIZATION_INCOMPLETE:
			log_error("BLE_ERROR_INITIALIZATION_INCOMPLETE");
			break;
		case BLE_ERROR_ALREADY_INITIALIZED:
			log_error("BLE_ERROR_ALREADY_INITIALIZED");
			break;
		case BLE_ERROR_UNSPECIFIED:
			log_error("BLE_ERROR_UNSPECIFIED: Unknown error");
			break;
		case BLE_ERROR_INTERNAL_STACK_FAILURE:
			log_error("BLE_ERROR_INTERNAL_STACK_FAILURE: internal stack failure");
			break;
		case BLE_ERROR_NOT_FOUND:
			log_error("BLE_ERROR_NOT_FOUND");
			break;
		default:
			log_error("Unknown error");
	}
}

/** print device address to the terminal */
void leka::ble::printAddress(const ::ble::address_t &addr)
{
	log_info("%02x:%02x:%02x:%02x:%02x:%02x\r\n", addr[5], addr[4], addr[3], addr[2], addr[1], addr[0]);
}

void leka::ble::printMacAddress()
{
	/* Print out device MAC address to the console*/
	::ble::own_address_type_t addr_type;
	::ble::address_t address;
	BLE::Instance().gap().getAddress(addr_type, address);
	log_info("DEVICE MAC ADDRESS: ");
	leka::ble::printAddress(address);
}

auto leka::ble::phy_to_string(::ble::phy_t phy) -> const char *
{
	switch (phy.value()) {
		case ::ble::phy_t::LE_1M:
			return "LE 1M";
		case ::ble::phy_t::LE_2M:
			return "LE 2M";
		case ::ble::phy_t::LE_CODED:
			return "LE coded";
		default:
			return "invalid PHY";
	}
}

// LCOV_EXCL_STOP
