#!/usr/bin/env python3

# Leka - LekaOS
# Copyright 2024 APF France handicap
# SPDX-License-Identifier: Apache-2.0

import argparse
import datetime
import re
import sys
from dataclasses import dataclass
from pathlib import Path
from time import sleep
from typing import List, Optional

from colorama import Fore, Style

from shared.test_utils import (
    SerialConnection,
    FlashHandler,
    Logger,
    SerialConfig,
)


@dataclass
class Config:
    TESTS_FUNCTIONAL_ROOT: Path = Path("_build/LEKA_V1_2_DEV/tests/functional/tests/")
    TESTS_RESULTS_ROOT: Path = Path("_build/LEKA_V1_2_DEV/tests/functional/results/")
    TESTS_BIN_EXTENSION: str = ".bin"


class TestResult:
    def __init__(self, source_path: Path):
        self.source_path = source_path
        self.result_path = self._generate_result_path()
        self.failures: List[str] = []
        self.content: str = ""

    def _generate_result_path(self) -> Path:
        timestamp = datetime.datetime.now().strftime("%Y_%m_%d_%H.%M.%S")
        Config.TESTS_RESULTS_ROOT.mkdir(parents=True, exist_ok=True)
        return Config.TESTS_RESULTS_ROOT / f"{self.source_path.stem}_{timestamp}.txt"

    def write_data(self, data: str) -> None:
        try:
            with open(self.result_path, "a", encoding="utf-8") as f:
                f.write(data)
        except IOError as e:
            Logger.print_failure(f"Failed to write to result file: {e}")
            raise

    def read_content(self) -> str:
        try:
            with open(self.result_path, "r", encoding="utf-8") as f:
                self.content = f.read()
            return self.content
        except IOError as e:
            Logger.print_failure(f"Failed to read result file: {e}")
            raise


class TestRunner:
    def __init__(self, test_path: Path, serial_conn: SerialConnection):
        self.test_path = test_path
        self.serial = serial_conn
        self.result = TestResult(test_path)

    def run(self) -> bool:
        if not FlashHandler.flash_binary(self.test_path):
            return False

        while True:
            data = self._wait_for_response()
            if data is None:
                return False

            if data.rstrip() == "<<END_OF_TESTS>>":
                return True
            elif data.rstrip() != "." and data.rstrip() != "<<START_OF_TESTS>>":
                if data.strip():
                    self.result.write_data(data)

    def _wait_for_response(self) -> Optional[str]:
        max_retries = int(
            SerialConfig.RESPONSE_TIMEOUT / SerialConfig.RESPONSE_RETRY_DELAY
        )
        for _ in range(max_retries):
            sleep(SerialConfig.RESPONSE_RETRY_DELAY)
            data = self.serial.read_line()
            if data:
                return data
        return None

    def check_status(self) -> bool:
        try:
            content = self.result.read_content()
            failure_pattern = re.compile(r".*\.cpp:[0-9].+: Failure|\[  FAILED  \]")
            self.result.failures = failure_pattern.findall(content)
            return "All tests passed!" in content and not self.result.failures
        except IOError as e:
            Logger.print_failure(f"Failed to read result file: {e}")
            raise


def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Run functional tests")
    parser.add_argument(
        "-p",
        "--port",
        default="/dev/tty.usbmodem*",
        help="serial port path used for the robot",
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
            f"\n{Fore.RED}❌ {len(failed_tests)} of {len(results)} "
            f"tests failed{Style.RESET_ALL}\n"
        )
    else:
        print(f"\n{Fore.GREEN}✅ All {len(results)} tests passed!{Style.RESET_ALL}")


def main() -> int:
    args = parse_arguments()

    # Find test files
    if args.all:
        test_files = list(
            Config.TESTS_FUNCTIONAL_ROOT.glob(f"**/*{Config.TESTS_BIN_EXTENSION}")
        )
    else:
        test_files = args.bin_files

    if not test_files:
        Logger.print_failure("No test files found!")
        return 1

    # Setup serial connection
    serial_conn = SerialConnection(args.port)
    serial_conn.connect()

    # Initial flash erase if needed
    if args.no_flash_erase:
        FlashHandler.erase()

    serial_conn.reset_buffers()

    # Run tests
    Logger.print_start("Running tests")
    results = []
    for test_file in test_files:
        runner = TestRunner(test_file, serial_conn)
        if runner.run():
            results.append(runner)

    if args.no_flash_erase:
        FlashHandler.erase()

    serial_conn.reset_buffers()

    # Print detailed results
    print_test_summary(results)

    # Final flash erase
    Logger.print_start("Erasing flash after tests")
    FlashHandler.erase()

    return 1 if any(not r.check_status() for r in results) else 0


if __name__ == "__main__":
    sys.exit(main())
