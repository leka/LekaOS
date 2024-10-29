#!/usr/bin/env python3

"""Run sleep functionality tests on the Leka device."""

# Leka - LekaOS
# Copyright 2024 APF France handicap
# SPDX-License-Identifier: Apache-2.0

import argparse
import sys
import re
from modules.logger import configure_logging
from modules.serial_utils import connect_serial, reset_buffer, wait_for_system_to_sleep
from modules.flash_utils import (
    flash_os,
    erase_flash,
    print_end_failure,
    print_end_success,
)
from colorama import Fore, Style


OS_BIN_FILE_PATH = "_build/LEKA_V1_2_DEV/app/os/LekaOS.bin"


def parse_arguments() -> argparse.Namespace:
    """
    Parse command-line arguments.

    Returns:
        argparse.Namespace: Parsed arguments.
    """
    parser = argparse.ArgumentParser(description="Run sleep functionality tests")

    parser.add_argument(
        "-p",
        "--port",
        metavar="PORT",
        default="/dev/tty.usbmodem*",
        help="Serial port path used for the robot connection",
    )
    parser.add_argument(
        "--response-timeout",
        type=float,
        default=30.0,
        metavar="RESPONSE_TIMEOUT",
        help="Response timeout in seconds (default: 30.0)",
    )
    parser.add_argument(
        "--no-flash-erase",
        action="store_false",
        dest="flash_erase",
        help="Disable flash erase",
    )
    parser.add_argument(
        "-d",
        "--duration",
        type=int,
        default=18000,
        metavar="DURATION",
        help="Duration in seconds to wait for the system to sleep (default: 18000)",
    )
    parser.add_argument(
        "-s",
        "--deep-sleep-percentage",
        type=int,
        default=95,
        metavar="DEEP_SLEEP_PERCENTAGE",
        help="Deep sleep percentage (default: 95)",
    )
    parser.add_argument(
        "-v",
        "--verbose",
        action="store_true",
        help="Enable verbose (debug) logging",
    )

    return parser.parse_args()


def calculate_sleep_deep_statistics(lines: list) -> tuple:
    """
    Calculate average sleep and deep sleep percentages from collected data.

    Args:
        lines (list): List of strings containing sleep data.

    Returns:
        tuple: Average sleep percentage and average deep sleep percentage.
    """

    def parse_line(line: str) -> tuple:
        """
        Parse a line to extract sleep and deep sleep percentages.

        Args:
            line (str): A line of text containing sleep data.

        Returns:
            tuple: Sleep percentage and deep sleep percentage if matched; otherwise, (None, None).
        """
        pattern = re.compile(r"slp:\s*(\d+)%.*?dsl:\s*(\d+)%")
        match = pattern.search(line)
        if match:
            return map(int, match.groups())
        return None, None

    print("Analyzing sleep data...")
    if not lines:
        print_end_failure("No sleep data found")
        sys.exit(1)

    sum_sleep, sum_deep_sleep, count = 0, 0, 0

    for line in lines:
        sleep_val, deep_sleep_val = parse_line(line)
        if sleep_val is not None and deep_sleep_val is not None:
            sum_sleep += sleep_val
            sum_deep_sleep += deep_sleep_val
            count += 1

    if count == 0:
        print_end_failure("Analyzing sleep data")
        sys.exit(1)

    print_end_success("Analyzing sleep data")
    return sum_sleep / count, sum_deep_sleep / count


def main() -> int:
    """
    Main function to parse arguments, initialize tests, and run sleep tests on the device.

    Returns:
        int: Exit status code (0 for success, 1 for failure).
    """
    args = parse_arguments()
    configure_logging(args.verbose)

    # Connect to serial port
    com = connect_serial(args.port)

    # Flash erase if enabled
    if args.flash_erase:
        erase_flash()

    # Flash the OS binary
    flash_os(OS_BIN_FILE_PATH)

    # Reset serial buffers
    reset_buffer(com)

    # Wait for system to sleep and collect data
    data = wait_for_system_to_sleep(com, args.duration)

    # Calculate sleep statistics
    sleep_avg, deep_sleep_avg = calculate_sleep_deep_statistics(data)

    # Display results
    print(f"\n{Fore.CYAN}Average sleep: {sleep_avg}%{Style.RESET_ALL}")
    print(f"{Fore.CYAN}Average deep sleep: {deep_sleep_avg}%{Style.RESET_ALL}\n")

    # Evaluate deep sleep percentage
    if deep_sleep_avg >= args.deep_sleep_percentage:
        print(
            f"{Fore.GREEN}Deep sleep is higher than {args.deep_sleep_percentage}%, \
                this is good! ✅{Style.RESET_ALL}"
        )
        ret = 0
    else:
        print(
            f"{Fore.RED}Deep sleep is lower than {args.deep_sleep_percentage}%, \
                this is bad! ❌{Style.RESET_ALL}"
        )
        ret = 1

    # Flash erase after tests if enabled
    if args.flash_erase:
        print("Erasing flash after tests...")
        erase_flash()

    return ret


if __name__ == "__main__":
    sys.exit(main())
