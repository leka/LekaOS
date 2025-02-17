#!/usr/bin/env python3

# Leka - LekaOS
# Copyright 2024 APF France handicap
# SPDX-License-Identifier: Apache-2.0

import argparse
import glob
import re
import subprocess
import sys
from dataclasses import dataclass
from time import sleep
from typing import List, Tuple

import serial
import serial.tools.list_ports
from colorama import Fore, Style

#
# MARK: - Configuration
#


@dataclass
class Config:
    OS_BIN_FILE_PATH: str = "_build/LEKA_V1_2_DEV/app/os/LekaOS.bin"
    SERIAL_TIMEOUT: float = 0.1
    RESPONSE_RETRY_DELAY: float = 0.1
    BREAK_DELAY: int = 1


class SerialConfig:
    def __init__(self, port: str, response_timeout: float):
        self.port = port
        self.response_timeout = response_timeout
        self.max_retries = response_timeout / Config.RESPONSE_RETRY_DELAY
        self.baud_rate = 115200


#
# MARK: - Utility Class
#


class Logger:
    @staticmethod
    def print_start(message: str) -> None:
        print(Fore.CYAN + f"\n{message}..." + Style.RESET_ALL)

    @staticmethod
    def print_success(message: str) -> None:
        print(Fore.CYAN + f"{message}... ✅" + Style.RESET_ALL)

    @staticmethod
    def print_failure(message: str) -> None:
        print(Fore.RED + f"{message}... ❌" + Style.RESET_ALL)

    @staticmethod
    def print_progress() -> None:
        print("•", end="", flush=True)

    @staticmethod
    def print_data_received() -> None:
        print(Fore.GREEN + "•" + Style.RESET_ALL, end="", flush=True)


#
# MARK: - Serial Handler
#


class SerialHandler:
    def __init__(self, config: SerialConfig):
        self.config = config
        self.com = None

    def connect(self) -> None:
        ports = glob.glob(self.config.port)
        port = ports[0] if ports else self.config.port

        Logger.print_start(f"Connecting to {port}")
        try:
            self.com = serial.Serial(
                port, self.config.baud_rate, timeout=Config.SERIAL_TIMEOUT
            )
            Logger.print_success(f"Connecting to {self.com.name}")
        except serial.serialutil.SerialException as error:
            Logger.print_failure(f"Connecting to {port}")
            print(f"{error}")
            sys.exit(1)

    def reset_buffer(self) -> None:
        Logger.print_start("Resetting com buffer")
        self.com.reset_input_buffer()
        self.com.reset_output_buffer()
        self.com.send_break(Config.BREAK_DELAY)
        sleep(Config.BREAK_DELAY)
        Logger.print_success("Resetting com buffer")

    def read_data(self, duration: int) -> List[str]:
        Logger.print_start(f"Waiting for LekaOS to run for {duration} seconds")
        data = []

        for second in range(duration):
            if self.com.in_waiting > 0:
                Logger.print_data_received()
                lines = self.com.readlines()
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


#
# MARK: - Flash Handler
#


class FlashHandler:
    @staticmethod
    def erase() -> None:
        Logger.print_start("Erasing flash")
        try:
            subprocess.run(
                "st-flash --connect-under-reset --reset erase", shell=True, check=True
            )
            Logger.print_success("Erasing flash")
        except subprocess.CalledProcessError:
            Logger.print_failure("Erasing flash")
            sys.exit(1)

    @staticmethod
    def flash_os(bin_path: str) -> None:
        Logger.print_start(f"Flashing {bin_path}")
        cmd_flash = (
            f"openocd -f interface/stlink.cfg "
            f"-c 'transport select hla_swd' "
            f"-f target/stm32f7x.cfg "
            f"-c 'program {bin_path} 0x08000000' "
            f"-c exit "
        )

        try:
            subprocess.run(cmd_flash, shell=True, check=True)
            Logger.print_success(f"Flashing {bin_path}")
        except subprocess.CalledProcessError:
            Logger.print_failure(f"Flashing {bin_path}")
            sys.exit(1)
        sleep(1)

        Logger.print_start("Reseting robot")
        cmd_reset = (
            "openocd -f interface/stlink.cfg "
            "-c 'transport select hla_swd' "
            "-f target/stm32f7x.cfg "
            "-c init -c 'reset run' "
            "-c exit "
        )

        try:
            subprocess.run(cmd_reset, shell=True, check=True)
            Logger.print_success("Reseting robot")
        except subprocess.CalledProcessError:
            Logger.print_failure("Reseting robot")
            sys.exit(1)
        sleep(1)


#
# MARK: - Data Analyzer
#


class SleepAnalyzer:
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

        avg_sleep_time = sum(s[0] for s in stats) / len(stats)
        avg_deep_sleep_time = sum(s[1] for s in stats) / len(stats)

        Logger.print_success("Analyzing sleep data")
        return avg_sleep_time, avg_deep_sleep_time


#
# MARK: - Main
#


def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Run functional tests")
    parser.add_argument(
        "-p",
        "--port",
        metavar="PORT",
        default="/dev/tty.usbmodem*",
        help="serial port path used for the robot",
    )
    parser.add_argument(
        "--response-timeout",
        metavar="RESPONSE_TIMEOUT",
        type=float,
        default=30.0,
        help="response timeout is seconds",
    )
    parser.add_argument(
        "--no-flash-erase", action="store_false", help="disable flash erase"
    )
    parser.add_argument(
        "-d",
        "--duration",
        metavar="DURATION",
        type=int,
        default=18000,
        help="duration in seconds to wait for the system to sleep",
    )
    parser.add_argument(
        "-s",
        "--deep-sleep-percentage",
        metavar="DEEP_SLEEP_PERCENTAGE",
        type=int,
        default=95,
        help="deep sleep percentage",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_arguments()

    serial_config = SerialConfig(args.port, args.response_timeout)
    serial_handler = SerialHandler(serial_config)
    serial_handler.connect()

    if args.no_flash_erase:
        FlashHandler.erase()

    FlashHandler.flash_os(Config.OS_BIN_FILE_PATH)
    serial_handler.reset_buffer()

    data = serial_handler.read_data(args.duration)
    sleep_time, deep_sleep_time = SleepAnalyzer.analyze_sleep_data(data)

    print(f"\nAverage sleep: {sleep_time}%")
    print(f"Average deep sleep: {deep_sleep_time}%")

    if deep_sleep_time >= args.deep_sleep_percentage:
        print(
            Fore.GREEN + f"Deep sleep is higher than {args.deep_sleep_percentage}%, "
            "this is good! ✅" + Style.RESET_ALL
        )
        ret = 0
    else:
        print(
            Fore.RED + f"Deep sleep is lower than {args.deep_sleep_percentage}%, "
            "this is bad! ❌" + Style.RESET_ALL
        )
        ret = 1

    print("Erasing flash after tests...")
    FlashHandler.erase()
    return ret


if __name__ == "__main__":
    sys.exit(main())
