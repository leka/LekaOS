#!/usr/bin/env python3

"""Run functional tests on the Leka device."""

# Leka - LekaOS
# Copyright 2024 APF France handicap
# SPDX-License-Identifier: Apache-2.0

import argparse
import datetime
import logging
import re
import shutil
import sys
import subprocess
import time
from pathlib import Path
from subprocess import CalledProcessError, run
from typing import List, Optional

import colorama
import serial
import serial.tools.list_ports

# Initialize Colorama
colorama.init(autoreset=True)

# Constants
TESTS_FUNCTIONAL_ROOT_DIRECTORY = Path("_build/LEKA_V1_2_DEV/tests/functional/tests/")
TESTS_BIN_EXTENSION = ".bin"

# Configure Logging
logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s [%(levelname)s] %(message)s",
    handlers=[logging.StreamHandler(sys.stdout)],
)


def check_external_tools():
    """
    Ensure that required external tools are available.
    """
    required_tools = ["openocd", "st-flash"]
    missing_tools = [tool for tool in required_tools if not shutil.which(tool)]
    if missing_tools:
        logging.error(f"Missing required tools: {', '.join(missing_tools)}")
        sys.exit(1)


def valid_file(arg: str) -> Path:
    """
    Validate that the provided file exists and has the correct extension.
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
    """
    return list(TESTS_FUNCTIONAL_ROOT_DIRECTORY.rglob(f"*{TESTS_BIN_EXTENSION}"))


def warning_print(message: str):
    """
    Print a warning message in yellow.
    """
    print(colorama.Fore.YELLOW + f"⚠️ Warning: {message}" + colorama.Style.RESET_ALL)


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
        """
        self.path = path
        self.serial = serial_connection
        self.response_timeout = response_timeout
        self.result_filepath = self.define_result_path()
        self.failures: List[str] = []

    def define_result_path(self) -> Path:
        """
        Define the path for the result file based on the current timestamp.
        """
        timestamp = datetime.datetime.now().strftime("%Y-%m-%d_%H.%M.%S")
        return self.path.parent / f"{self.path.stem}_{timestamp}.txt"

    def generate_result_file(self):
        """
        Create an empty result file.
        """
        try:
            self.result_filepath.touch(exist_ok=False)
            logging.debug(f"Created result file at {self.result_filepath}")
        except OSError as e:
            logging.exception(f"Could not create or open file: {self.result_filepath}")
            logging.exception(f"Error: {e}")
            sys.exit(1)

    def edit_result_file(self, data: str):
        """
        Append data to the result file.
        """
        try:
            with self.result_filepath.open("a") as file:
                file.write(data)
            logging.debug(f"Appended data to {self.result_filepath}")
        except OSError as e:
            logging.exception(f"Could not write to file: {self.result_filepath}")
            logging.exception(f"Error: {e}")
            sys.exit(1)

    def flash(self) -> bool:
        """
        Flash the binary to the device using OpenOCD and reset the device.
        Returns True if both commands succeed, False otherwise.
        """
        flash_command = [
            "openocd",
            "-f",
            "interface/stlink.cfg",
            "-c",
            "transport select hla_swd",
            "-f",
            "target/stm32f7x.cfg",
            "-c",
            f"program {self.path} 0x08000000",
            "-c",
            "exit",
        ]
        reset_command = [
            "openocd",
            "-f",
            "interface/stlink.cfg",
            "-c",
            "transport select hla_swd",
            "-f",
            "target/stm32f7x.cfg",
            "-c",
            "init",
            "-c",
            "reset run",
            "-c",
            "exit",
        ]
        return self._run_command(
            flash_command, "Error during flashing"
        ) and self._run_command(reset_command, "Error during reset")

    def _run_command(self, command: List[str], error_msg: str) -> bool:
        """
        Run a shell command and return True if successful.
        Logs an error and returns False if the command fails.
        """
        try:
            run(
                command,
                check=True,
                stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL,
            )
            logging.debug(f"Command '{' '.join(command)}' executed successfully")
            return True
        except CalledProcessError as e:
            logging.error(f"{error_msg}: {e}")
            return False

    def read_serial_output(self) -> Optional[str]:
        """
        Read a line from the serial connection and return it as a string.
        """
        try:
            data = self.serial.readline().decode("utf-8").strip()
            logging.debug(f"Read from serial: {data}")
            return data if data else None
        except serial.SerialException as e:
            logging.error(f"Serial read error: {e}")
            return None

    def wait_for_response(self) -> Optional[str]:
        """
        Wait for a response from the device within the response timeout.
        """
        retries = int(self.response_timeout / 0.1)
        for _ in range(retries):
            time.sleep(0.1)
            data = self.read_serial_output()
            if data:
                return data
        return None

    def run(self) -> bool:
        """
        Run the test by flashing the binary and collecting test results.
        Returns True if flashing succeeds, False otherwise.
        """
        logging.info(f"Running test {self.path.name} ...")
        self.generate_result_file()
        if not self.flash():
            warning_print("Error flashing!")
            return False

        while True:
            data = self.wait_for_response()
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
        Returns True if all tests passed, False otherwise.
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


def erase_flash():
    """
    Erase the flash memory of the device.
    """
    try:
        logging.info("Erasing flash memory...")
        run(["st-flash", "--connect-under-reset", "--reset", "erase"], check=True)
        logging.debug("Flash memory erased successfully.")
    except CalledProcessError as e:
        logging.error(f"Error erasing flash: {e}")
        sys.exit(1)


def reset_buffer(serial_conn: serial.Serial):
    """
    Reset the serial input and output buffers and send a break signal.
    """
    try:
        serial_conn.reset_input_buffer()
        serial_conn.reset_output_buffer()
        serial_conn.send_break()
        time.sleep(1)
        logging.debug("Serial buffers reset and break signal sent.")
    except serial.SerialException as e:
        logging.error(f"Error resetting serial buffers: {e}")
        sys.exit(1)


def parse_arguments() -> argparse.Namespace:
    """
    Parse command-line arguments.
    """
    parser = argparse.ArgumentParser(description="Run functional tests")

    parser.add_argument(
        "-p",
        "--port",
        metavar="PORT",
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
        "-v",
        "--verbose",
        action="store_true",
        help="Enable verbose (debug) logging",
    )

    return parser.parse_args()


def main() -> int:
    """
    Main function to parse arguments, initialize tests, and run tests on the device.
    """
    args = parse_arguments()

    # Set logging level based on verbose flag
    if args.verbose:
        logging.getLogger().setLevel(logging.DEBUG)
        logging.debug("Verbose logging enabled.")

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

    # Find serial ports matching the pattern
    ports = list(serial.tools.list_ports.grep(r"usbmodem"))
    serial_port = ports[0].device if ports else args.port

    for port in ports:
        logging.info(f"Found serial port: {port.device}")

    # Establish serial connection
    try:
        with serial.Serial(serial_port, 115200, timeout=0.1) as ser:
            logging.info(f"Connected to {serial_port}")

            # Flash erase if enabled
            if args.flash_erase:
                erase_flash()

            reset_buffer(ser)

            logging.info("Running tests...")
            run_tests = []
            for bin_file in bin_files:
                test = Test(bin_file, ser, args.response_timeout)
                if test.run():
                    run_tests.append(test)

            # Flash erase after tests if enabled
            if args.flash_erase:
                erase_flash()

            reset_buffer(ser)

            # Print summary
            fails = []
            print("\nResults files:")
            for test in run_tests:
                print(test.result_filepath)
                if not test.check_status():
                    fails.append(test)

            print("\n     TESTS")
            for test in run_tests:
                status = " ❌ " if test in fails else " ✅ "
                print(f"{status} {test.path.name}")

            print("\n")
            for test in fails:
                print(
                    colorama.Fore.YELLOW
                    + f"{test.result_filepath}"
                    + colorama.Style.RESET_ALL
                )
                test.print_result_file()

            if fails:
                print(
                    colorama.Fore.RED
                    + f" ❌ {len(fails)} out of {len(run_tests)} suites have failed..."
                    + colorama.Style.RESET_ALL
                )
                return 1
            else:
                print(
                    colorama.Fore.GREEN
                    + f" ✅ All {len(run_tests)} suites have passed!"
                    + colorama.Style.RESET_ALL
                )
                return 0

    except serial.SerialException as e:
        logging.error(f"Could not open serial port {serial_port}: {e}")
        sys.exit(1)


if __name__ == "__main__":
    sys.exit(main())
