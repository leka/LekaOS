#!/usr/bin/env python3

# Leka - LekaOS
# Copyright 2024 APF France handicap
# SPDX-License-Identifier: Apache-2.0

import argparse
import re
import sys
from dataclasses import dataclass
from time import sleep
from typing import List, Tuple

from colorama import Fore, Style

from shared.test_utils import SerialConnection, FlashHandler, Logger

#
# MARK: - Configuration
#


@dataclass
class Config:
    OS_BIN_FILE_PATH: str = "_build/LEKA_V1_2_DEV/app/os/LekaOS.bin"


class SystemTestRunner:
    def __init__(self, serial_conn: SerialConnection):
        self.serial = serial_conn

    def read_data(self, duration: int) -> List[str]:
        Logger.print_start(f"Waiting for LekaOS to run for {duration} seconds")
        data = []

        for second in range(duration):
            if self.serial.com.in_waiting > 0:
                Logger.print_data_received()
                lines = self.serial.com.readlines()
                data.extend(
                    [str(line, "utf-8", errors="replace").rstrip() for line in lines]
                )
            else:
                Logger.print_progress()

            if (second + 1) % 60 == 0 and (second + 1) != duration:
                print()
            sleep(1)

        print()
        return data

    @staticmethod
    def analyze_sleep_data(lines: List[str]) -> Tuple[float, float]:
        Logger.print_start("Analyzing sleep data")

        filtered_data = list(filter(lambda s: "watchdog" in s, lines))[-10:]
        if not filtered_data:
            Logger.print_failure("No sleep data found")
            sys.exit(1)

        pattern = re.compile(r"slp:\s*(\d+)%.*?dsl:\s*(\d+)%")
        stats = [
            tuple(map(int, pattern.search(line).groups()))
            for line in filtered_data
            if pattern.search(line)
        ]

        if not stats:
            Logger.print_failure("Analyzing sleep data")
            sys.exit(1)

        avg_sleep = sum(s[0] for s in stats) / len(stats)
        avg_deep_sleep = sum(s[1] for s in stats) / len(stats)

        Logger.print_success("Analyzing sleep data")
        return avg_sleep, avg_deep_sleep


#
# MARK: - Main
#


def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Run system tests")
    parser.add_argument(
        "-p",
        "--port",
        default="/dev/tty.usbmodem*",
        help="serial port path used for the robot",
    )
    parser.add_argument(
        "--no-flash-erase", action="store_false", help="disable flash erase"
    )
    parser.add_argument(
        "-d",
        "--duration",
        type=int,
        default=18000,
        help="duration in seconds to wait for the system to sleep",
    )
    parser.add_argument(
        "-s",
        "--deep-sleep-percentage",
        type=int,
        default=95,
        help="deep sleep percentage threshold",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_arguments()

    serial_conn = SerialConnection(args.port)
    serial_conn.connect()

    if args.no_flash_erase:
        FlashHandler.erase()

    FlashHandler.flash_binary(Config.OS_BIN_FILE_PATH)
    serial_conn.reset_buffers()

    runner = SystemTestRunner(serial_conn)
    data = runner.read_data(args.duration)
    sleep_time, deep_sleep_time = runner.analyze_sleep_data(data)

    print(f"\nAverage sleep: {sleep_time}%")
    print(f"Average deep sleep: {deep_sleep_time}%")

    threshold = args.deep_sleep_percentage
    if deep_sleep_time >= threshold:
        msg = f"Deep sleep is higher than {threshold}%, " "this is good! ✅"
        print(Fore.GREEN + msg + Style.RESET_ALL)
        ret = 0
    else:
        msg = f"Deep sleep is lower than {threshold}%, " "this is bad! ❌"
        print(Fore.RED + msg + Style.RESET_ALL)
        ret = 1

    print("Erasing flash after tests...")
    FlashHandler.erase()
    return ret


if __name__ == "__main__":
    sys.exit(main())
