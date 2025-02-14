#!/usr/bin/env python3

"""Run functional tests on the Leka device."""

# Leka - LekaOS
# Copyright 2024 APF France handicap
# SPDX-License-Identifier: Apache-2.0

import argparse
import datetime
import logging
import re
import sys
from pathlib import Path
from typing import List

import colorama
from colorama import Fore, Style
import serial
from modules.logger import configure_logging
from modules.serial_utils import connect_serial, reset_buffer, wait_for_response
from modules.flash_utils import (
    flash_os_and_reset,
    erase_flash,
)
from modules.flash_utils import check_external_tools

# Initialize Colorama
colorama.init(autoreset=True)

# Constants
TESTS_FUNCTIONAL_ROOT_DIRECTORY = Path("_build/LEKA_V1_2_DEV/tests/functional/tests/")
TESTS_BIN_EXTENSION = ".bin"


def valid_file(arg: str) -> Path:
    """
    Validate that the provided file exists and has the correct extension.

    Args:
        arg (str): The filename to validate.

    Returns:
        Path: The validated file path.

    Raises:
        argparse.ArgumentTypeError: If the file does not exist or has an incorrect extension.
    """
    path = TESTS_FUNCTIONAL_ROOT_DIRECTORY / arg
    if not path.exists():
        raise argparse.ArgumentTypeError(f"⚠️ The file {path} does not exist!")
    if path.suffix != TESTS_BIN_EXTENSION:
        raise argparse.ArgumentTypeError(
            f"⚠️ The filename {path} must have a '{TESTS_BIN_EXTENSION}' extension!"
        )
    return path


def list_bin_files() -> List[Path]:
    """
    List all .bin files in the TESTS_FUNCTIONAL_ROOT_DIRECTORY recursively.

    Returns:
        List[Path]: List of binary file paths.
    """
    return list(TESTS_FUNCTIONAL_ROOT_DIRECTORY.rglob(f"*{TESTS_BIN_EXTENSION}"))


def warning_print(message: str):
    """
    Print a warning message in yellow.

    Args:
        message (str): The warning message to print.
    """
    print(Fore.YELLOW + f"⚠️ Warning: {message}" + Style.RESET_ALL)


class Test:
    """
    Represents a single test that can be flashed to the device and executed.
    """

    def __init__(
        self, path: Path, serial_connection: serial.Serial, response_timeout: float
    ):
        """
        Initialize a test instance with the given binary path, serial connection,
        and response timeout.

        Args:
            path (Path): Path to the binary file.
            serial_connection (serial.Serial): Serial connection to the device.
            response_timeout (float): Timeout for device responses.
        """
        self.path = path
        self.serial = serial_connection
        self.response_timeout = response_timeout
        self.result_filepath = self.define_result_path()
        self.failures: List[str] = []

    def define_result_path(self) -> Path:
        """
        Define the path for the result file based on the current timestamp.

        Returns:
            Path: Path to the result file.
        """
        timestamp = datetime.datetime.now().strftime("%Y-%m-%d_%H.%M.%S")
        return self.path.parent / f"{self.path.stem}_{timestamp}.txt"

    def generate_result_file(self):
        """
        Create an empty result file.

        Raises:
            SystemExit: If the file cannot be created.
        """
        try:
            self.result_filepath.touch(exist_ok=False)
            logging.debug(f"Created result file at {self.result_filepath}")
        except OSError as e:
            logging.exception(f"Could not create or open file: {self.result_filepath}")
            logging.error(f"Error: {e}")
            sys.exit(1)

    def edit_result_file(self, data: str):
        """
        Append data to the result file.

        Args:
            data (str): Data to append.

        Raises:
            SystemExit: If writing to the file fails.
        """
        try:
            with self.result_filepath.open("a") as file:
                file.write(data + "\n")
            logging.debug(f"Appended data to {self.result_filepath}")
        except OSError as e:
            logging.exception(f"Could not write to file: {self.result_filepath}")
            logging.error(f"Error: {e}")
            sys.exit(1)

    def flash(self) -> bool:
        """
        Flash the binary to the device using OpenOCD and reset the device.

        Returns:
            bool: True if flashing and resetting succeed; False otherwise.
        """
        return flash_os_and_reset(self.path)

    def run(self) -> bool:
        """
        Run the test by flashing the binary and collecting test results.

        Returns:
            bool: True if flashing succeeds; False otherwise.
        """
        logging.info(f"Running test {self.path.name} ...")
        self.generate_result_file()
        if not self.flash():
            warning_print("Error flashing!")
            return False

        while True:
            data = wait_for_response(self.serial, self.response_timeout)
            if data is not None:
                if data == "<<END_OF_TESTS>>":
                    logging.info(f"Running test {self.path.name} ... ✅️")
                    break
                elif data != ".":
                    self.edit_result_file(data)
            else:
                logging.warning("No response received.")
                break
        return True

    def check_status(self) -> bool:
        """
        Check the test results to determine if all tests passed.

        Returns:
            bool: True if all tests passed; False otherwise.
        """
        try:
            with self.result_filepath.open("r") as file:
                content = file.read()
                if "All tests passed!" in content:
                    logging.info(f"All tests passed for {self.path.name}.")
                    return True
                else:
                    failure_pattern = re.compile(
                        r".*\.cpp:\d+:.+: Failure|\[  FAILED  \]"
                    )
                    self.failures = failure_pattern.findall(content)
                    if self.failures:
                        logging.warning(
                            f"Failures in {self.path.name}: {self.failures}"
                        )
                        return False
            return False
        except FileNotFoundError:
            logging.error(f"The file: {self.result_filepath} doesn't exist")
            sys.exit(1)

    def print_result_file(self):
        """
        Print the contents of the result file.
        """
        try:
            with self.result_filepath.open("r") as file:
                content = file.read()
                print(content)
        except OSError as e:
            logging.error(f"Could not read file: {self.result_filepath}")
            logging.error(f"Error: {e}")


def parse_arguments() -> argparse.Namespace:
    """
    Parse command-line arguments.

    Returns:
        argparse.Namespace: Parsed arguments.
    """
    parser = argparse.ArgumentParser(description="Run functional tests")

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
        metavar="SECONDS",
        help="Response timeout in seconds (default: 30.0)",
    )
    parser.add_argument(
        "--no-flash-erase",
        action="store_false",
        dest="flash_erase",
        help="Disable flash erase",
    )
    parser.add_argument(
        "-v",
        "--verbose",
        action="store_true",
        help="Enable verbose (debug) logging",
    )
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument(
        "-b",
        "--bin-files",
        metavar="BIN_FILES",
        nargs="+",
        type=valid_file,
        help="List of binary executables",
    )
    group.add_argument(
        "--all", action="store_true", help="Select all binary executables"
    )
    parser.add_argument(
        "--select-port",
        action="store_true",
        help="Interactively select the serial port if multiple are found",
    )

    return parser.parse_args()


def main() -> int:
    """
    Main function to parse arguments, initialize tests, and run tests on the device.

    Returns:
        int: Exit status code (0 for success, 1 for failure).
    """
    args = parse_arguments()
    configure_logging(args.verbose)

    # Check for required external tools
    check_external_tools()

    # Determine which binary files to use
    if args.all:
        bin_files = list_bin_files()
    else:
        bin_files = args.bin_files

    if not bin_files:
        warning_print("No executable binaries found!")
        sys.exit(1)

    # Establish serial connection with optional selection
    try:
        com = connect_serial(args.port)

        # Flash erase if enabled
        if args.flash_erase:
            erase_flash()

        # Reset serial buffers
        reset_buffer(com)

        logging.info("Running tests...")
        run_tests = []
        for bin_file in bin_files:
            test = Test(bin_file, com, args.response_timeout)
            if test.run():
                run_tests.append(test)

        # Flash erase after tests if enabled
        if args.flash_erase:
            erase_flash()

        # Reset serial buffers again after flashing
        reset_buffer(com)

        # Print summary
        fails = []
        print("\nResults files:")
        for test in run_tests:
            print(test.result_filepath)
            if not test.check_status():
                fails.append(test)

        print("\n     TESTS")
        for test in run_tests:
            status = (
                f"{Fore.RED}❌{Style.RESET_ALL}"
                if test in fails
                else f"{Fore.GREEN}✅{Style.RESET_ALL}"
            )
            print(f"{status} {test.path.name}")

        print("\n")
        for test in fails:
            print(Fore.YELLOW + f"{test.result_filepath}" + Style.RESET_ALL)
            test.print_result_file()

        if fails:
            print(
                Fore.RED
                + f" ❌ {len(fails)} out of {len(run_tests)} suites have failed..."
                + Style.RESET_ALL
            )
            return 1
        else:
            print(
                Fore.GREEN
                + f" ✅ All {len(run_tests)} suites have passed!"
                + Style.RESET_ALL
            )
            return 0

    except serial.SerialException as e:
        logging.error(f"Could not open serial port {args.port}: {e}")
        sys.exit(1)
    except Exception as e:
        logging.error(f"An unexpected error occurred: {e}")
        sys.exit(1)


if __name__ == "__main__":
    sys.exit(main())
