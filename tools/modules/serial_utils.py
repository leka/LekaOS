# Leka - LekaOS
# Copyright 2024 APF France handicap
# SPDX-License-Identifier: Apache-2.0


"""
Utility functions for serial communication.
"""


import glob
import sys
from time import sleep
from typing import List, Optional
import serial
from colorama import Fore, Style

SERIAL_TIMEOUT = 0.1  # seconds


def connect_serial(port_pattern: str) -> serial.Serial:
    """
    Connect to the first serial port matching the given pattern.

    Args:
        port_pattern (str): Glob pattern to match serial ports.

    Returns:
        serial.Serial: An open serial connection.

    Raises:
        SystemExit: If unable to connect to the serial port.
    """
    ports = glob.glob(port_pattern)
    serial_port = ports[0] if ports else port_pattern

    try:
        com = serial.Serial(serial_port, 115200, timeout=SERIAL_TIMEOUT)
        print(f"Connected to {com.name}... {Fore.GREEN}✅{Style.RESET_ALL}")
        return com
    except serial.serialutil.SerialException as error:
        print(
            f"Error connecting to {serial_port}: {error}... {Fore.RED}❌{Style.RESET_ALL}"
        )
        sys.exit(1)


def reset_buffer(com: serial.Serial):
    """
    Reset the serial input and output buffers and send a break signal.

    Args:
        com (serial.Serial): The serial connection to reset.
    """
    print("Resetting COM buffer...")
    try:
        com.reset_input_buffer()
        com.reset_output_buffer()
        com.send_break(duration=1)
        sleep(1)
        print(f"Resetting COM buffer... {Fore.GREEN}✅{Style.RESET_ALL}")
    except serial.SerialException as e:
        print(f"Error resetting COM buffer: {e}... {Fore.RED}❌{Style.RESET_ALL}")
        sys.exit(1)


def read_output_serial(com: serial.Serial) -> str:
    """
    Read a line from the serial connection.

    Args:
        com (serial.Serial): The serial connection to read from.

    Returns:
        str: Decoded line from serial output.
    """
    try:
        data = com.readline().decode("utf-8").strip()
        return data
    except serial.SerialException as e:
        print(f"Serial read error: {e}")
        return ""


def wait_for_response(com: serial.Serial, response_timeout: float) -> Optional[str]:
    """
    Wait for a response from the device within the specified timeout.

    Args:
        com (serial.Serial): The serial connection to read from.
        response_timeout (float): Timeout in seconds.

    Returns:
        Optional[str]: The received data if any; otherwise, None.
    """
    retries = int(response_timeout / 0.1)
    for _ in range(retries):
        sleep(0.1)
        data = read_output_serial(com)
        if data:
            return data
    return None


def wait_for_system_to_sleep(com: serial.Serial, duration: int) -> List[str]:
    """
    Wait for the system to run for a specified duration, collecting relevant serial data.

    Args:
        com (serial.Serial): The serial connection to read from.
        duration (int): Duration in seconds to wait.

    Returns:
        List[str]: Collected lines containing 'watchdog'.
    """
    print("Waiting for system to run...")
    data = []
    for second in range(duration):
        if com.in_waiting > 0:
            print(f"{Fore.GREEN}•{Style.RESET_ALL}", end="", flush=True)
            lines = com.readlines()
            for line in lines:
                decoded_line = line.decode("utf-8", errors="replace").rstrip()
                data.append(decoded_line)
        else:
            print("•", end="", flush=True)

        if (second + 1) % 60 == 0 and (second + 1) != duration:
            print()

        sleep(1)

    print()
    filtered_data = [line for line in data if "watchdog" in line][-10:]
    print("\n".join(filtered_data))
    print(
        f"Waiting for system to run for {duration} seconds... {Fore.GREEN}✅{Style.RESET_ALL}"
    )
    return filtered_data
