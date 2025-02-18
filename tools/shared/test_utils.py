#!/usr/bin/env python3

# Leka - LekaOS
# Copyright 2024 APF France handicap
# SPDX-License-Identifier: Apache-2.0

import glob
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path
from time import sleep
from typing import Optional

import serial
from colorama import Fore, Style


@dataclass
class SerialConfig:
    SERIAL_TIMEOUT: float = 0.1
    SERIAL_BAUD_RATE: int = 115200
    RESPONSE_TIMEOUT: float = 30.0
    RESPONSE_RETRY_DELAY: float = 0.1
    BREAK_DELAY: int = 1


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


class SerialConnection:
    def __init__(self, port: str):
        self.port = self._resolve_port(port)
        self.com: Optional[serial.Serial] = None

    def _resolve_port(self, port: str) -> str:
        ports = glob.glob(port)
        return ports[0] if ports else port

    def connect(self) -> None:
        Logger.print_start(f"Connecting to {self.port}")
        try:
            self.com = serial.Serial(
                self.port,
                SerialConfig.SERIAL_BAUD_RATE,
                timeout=SerialConfig.SERIAL_TIMEOUT,
            )
            Logger.print_success(f"Connecting to {self.com.name}")
        except serial.serialutil.SerialException as error:
            Logger.print_failure(f"Connecting to {self.port}")
            print(f"{error}")
            sys.exit(1)

    def reset_buffers(self) -> None:
        Logger.print_start("Resetting com buffer")
        self.com.reset_input_buffer()
        self.com.reset_output_buffer()
        self.com.send_break(SerialConfig.BREAK_DELAY)
        sleep(SerialConfig.BREAK_DELAY)
        Logger.print_success("Resetting com buffer")

    def read_line(self) -> str:
        return self.com.readline().decode("utf-8", errors="replace")


class FlashHandler:
    @staticmethod
    def erase() -> None:
        Logger.print_start("Erasing flash")
        try:
            subprocess.run(
                "st-flash --connect-under-reset --reset erase",
                shell=True,
                check=True,
                capture_output=True,
            )
            Logger.print_success("Erasing flash")
        except subprocess.CalledProcessError:
            Logger.print_failure("Erasing flash")
            sys.exit(1)

    @staticmethod
    def flash_binary(bin_path: Path) -> bool:
        Logger.print_start(f"Flashing {bin_path}")

        cmd_flash = (
            f"openocd -f interface/stlink.cfg "
            f"-f target/stm32f7x.cfg "
            f"-c 'program {bin_path} 0x08000000' "
            f"-c exit"
        )

        try:
            subprocess.run(cmd_flash, shell=True, check=True, capture_output=True)
            Logger.print_success(f"Flashing {bin_path}")
        except subprocess.CalledProcessError:
            Logger.print_failure(f"Flashing {bin_path}")
            return False

        sleep(1)
        return FlashHandler.reset_device()

    @staticmethod
    def reset_device() -> bool:
        Logger.print_start("Resetting device")
        cmd_reset = (
            "openocd -f interface/stlink.cfg "
            "-f target/stm32f7x.cfg "
            "-c init -c 'reset run' "
            "-c exit"
        )

        try:
            subprocess.run(cmd_reset, shell=True, check=True, capture_output=True)
            Logger.print_success("Resetting device")
            sleep(1)
            return True
        except subprocess.CalledProcessError:
            Logger.print_failure("Resetting device")
            return False
