#!/usr/bin/env python3

# Leka - LekaOS
# Copyright 2024 APF France handicap
# SPDX-License-Identifier: Apache-2.0

import argparse
import datetime
import glob
import logging
import os
import re
import sys
from dataclasses import dataclass
from pathlib import Path
from time import sleep
from typing import List, Optional

import serial
from colorama import Fore, Style


# Constants
@dataclass
class Config:
    TESTS_FUNCTIONAL_ROOT: Path = Path("_build/LEKA_V1_2_DEV/tests/functional/tests/")
    TESTS_RESULTS_ROOT: Path = Path("_build/LEKA_V1_2_DEV/tests/functional/results/")
    TESTS_BIN_EXTENSION: str = ".bin"
    RESPONSE_TIMEOUT: float = 30.0
    RESPONSE_RETRY_DELAY: float = 0.1
    SERIAL_TIMEOUT: float = 0.1
    SERIAL_BAUD_RATE: int = 115200
    BREAK_DELAY: int = 1


class SerialConnection:
    def __init__(self, port: str, baud_rate: int, timeout: float):
        try:
            self.com = serial.Serial(port, baud_rate, timeout=timeout)
            logging.info("Connected to %s", self.com.name)
        except serial.serialutil.SerialException as error:
            logging.error("Serial connection failed: %s", error)
            raise

    def read_line(self) -> str:
        return self.com.readline().decode("utf-8", errors="replace")

    def reset_buffers(self) -> None:
        self.com.reset_input_buffer()
        self.com.reset_output_buffer()
        self.com.send_break(Config.BREAK_DELAY)
        sleep(Config.BREAK_DELAY)


class TestResult:
    def __init__(self, source_path: Path):
        self.source_path = source_path
        self.result_path = self._generate_result_path()
        self.failures: List[str] = []
        self.content: str = ""

    def _generate_result_path(self) -> Path:
        timestamp = datetime.datetime.now().strftime("%Y_%m_%d_%H.%M.%S")
        # Create results directory if it doesn't exist
        Config.TESTS_RESULTS_ROOT.mkdir(parents=True, exist_ok=True)
        return Config.TESTS_RESULTS_ROOT / f"{self.source_path.stem}_{timestamp}.txt"

    def write_data(self, data: str) -> None:
        try:
            with open(self.result_path, "a", encoding="utf-8") as f:
                f.write(data)
        except IOError as e:
            logging.error("Failed to write to result file: %s", e)
            raise

    def read_content(self) -> str:
        try:
            with open(self.result_path, "r", encoding="utf-8") as f:
                self.content = f.read()
            return self.content
        except IOError as e:
            logging.error("Failed to read result file: %s", e)
            raise


class TestRunner:
    def __init__(self, test_path: Path, serial_conn: SerialConnection):
        self.test_path = test_path
        self.serial = serial_conn
        self.result = TestResult(test_path)

    def flash_device(self) -> bool:
        logging.info("Flashing %s...", self.test_path)

        cmd_flash = (
            f"openocd -f interface/stlink.cfg "
            f"-f target/stm32f7x.cfg "
            f"-c 'program {self.test_path} 0x08000000' "
            f"-c exit >/dev/null 2>&1"
        )

        if os.system(cmd_flash) != 0:
            logging.warning("Flash failed!")
            return False

        sleep(1)

        cmd_reset = (
            "openocd -f interface/stlink.cfg "
            "-f target/stm32f7x.cfg "
            "-c init -c 'reset run' "
            "-c exit >/dev/null 2>&1"
        )

        if os.system(cmd_reset) != 0:
            logging.warning("Reset failed!")
            return False

        return True

    def wait_for_response(self) -> Optional[str]:
        max_retries = int(Config.RESPONSE_TIMEOUT / Config.RESPONSE_RETRY_DELAY)
        for _ in range(max_retries):
            sleep(Config.RESPONSE_RETRY_DELAY)
            data = self.serial.read_line()
            if data:
                return data
        return None

    def run(self) -> bool:
        if not self.flash_device():
            return False

        while True:
            data = self.wait_for_response()
            if data is None:
                return False

            if data.rstrip() == "<<END_OF_TESTS>>":
                return True
            elif data.rstrip() != "." and data.rstrip() != "<<START_OF_TESTS>>":
                if data.strip():
                    self.result.write_data(data)

    def check_status(self) -> bool:
        try:
            with open(self.result.result_path, "r", encoding="utf-8") as f:
                content = f.read()

            # Check for test failures
            failure_pattern = re.compile(r".*\.cpp:[0-9].+: Failure|\[  FAILED  \]")
            self.result.failures = failure_pattern.findall(content)

            # Check if all tests passed
            return "All tests passed!" in content and not self.result.failures

        except IOError as e:
            logging.error("Failed to read result file: %s", e)
            raise


def flash_erase() -> int:
    return os.system("st-flash --connect-under-reset --reset erase")


def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Run functional tests")

    parser.add_argument(
        "-p",
        "--port",
        default="/dev/tty.usbmodem*",
        help="serial port path used for the robot",
    )
    parser.add_argument(
        "--response-timeout",
        type=float,
        default=Config.RESPONSE_TIMEOUT,
        help="response timeout in seconds",
    )
    parser.add_argument(
        "--no-flash-erase", action="store_false", help="disable flash erase"
    )

    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument(
        "-b", "--bin-files", nargs="+", type=Path, help="list binary executables"
    )
    group.add_argument(
        "--all", action="store_true", help="select all binary executables"
    )

    return parser.parse_args()


def print_test_summary(results: List[TestRunner]) -> None:
    failed_tests = [r for r in results if not r.check_status()]

    print("\nResults files:")
    for runner in results:
        print(str(runner.result.result_path))

    print("\nTest Results:")
    for runner in results:
        status = "❌" if runner in failed_tests else "✅"
        print(f"{status} {runner.test_path}")

    # Print detailed failure information
    if failed_tests:
        print("\nDetailed Failure Information:")
        for failed_test in failed_tests:
            print(f"\n{Fore.YELLOW}{failed_test.result.result_path}{Style.RESET_ALL}\n")
            content = failed_test.result.read_content()
            if content.strip():
                print(content)
            else:
                print(f"{Fore.YELLOW}⚠️ Warning: No test output data!{Style.RESET_ALL}")

        print(
            f"\n{Fore.RED}❌ {len(failed_tests)} of {len(results)} tests failed{Style.RESET_ALL}\n"
        )
    else:
        print(f"\n{Fore.GREEN}✅ All {len(results)} tests passed!{Style.RESET_ALL}")


def main() -> int:
    # Setup logging
    logging.basicConfig(
        level=logging.INFO, format="%(asctime)s - %(levelname)s - %(message)s"
    )

    args = parse_arguments()

    # Find test files
    if args.all:
        test_files = list(
            Config.TESTS_FUNCTIONAL_ROOT.glob(f"**/*{Config.TESTS_BIN_EXTENSION}")
        )
    else:
        test_files = args.bin_files

    if not test_files:
        logging.warning("No test files found!")
        return 1

    # Setup serial connection
    ports = glob.glob(args.port)
    port = ports[0] if ports else args.port
    serial_conn = SerialConnection(port, Config.SERIAL_BAUD_RATE, Config.SERIAL_TIMEOUT)

    # Initial flash erase if needed
    if args.no_flash_erase:
        flash_erase()

    serial_conn.reset_buffers()

    # Run tests
    logging.info("Running tests...")
    results = []
    for test_file in test_files:
        runner = TestRunner(test_file, serial_conn)
        if runner.run():
            results.append(runner)

    if args.no_flash_erase:
        flash_erase()

    serial_conn.reset_buffers()

    # Print detailed results
    print_test_summary(results)

    # Final flash erase
    logging.info("Erasing flash after tests...")
    flash_erase()

    return 1 if any(not r.check_status() for r in results) else 0


if __name__ == "__main__":
    sys.exit(main())
