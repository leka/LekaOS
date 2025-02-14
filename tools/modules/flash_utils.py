# Leka - LekaOS
# Copyright 2024 APF France handicap
# SPDX-License-Identifier: Apache-2.0

"""
Utility functions for flashing the OS binary to the device.
"""

import logging
from pathlib import Path
import subprocess
import sys
import shutil
import time

from colorama import Fore, Style


def check_external_tools():
    """
    Ensure that required external tools are available.

    Raises:
        SystemExit: If any required tool is missing.
    """
    required_tools = ["openocd", "st-flash"]
    missing_tools = [tool for tool in required_tools if not shutil.which(tool)]
    if missing_tools:
        logging.error(f"Missing required tools: {', '.join(missing_tools)}")
        sys.exit(1)


def flash_os_and_reset(os_bin_path: Path) -> bool:
    """
    Flash the OS binary and reset the device.

    Args:
        os_bin_path (Path): Path to the OS binary.

    Returns:
        bool: True if both flashing and resetting succeed; False otherwise.
    """
    try:
        flash_os(str(os_bin_path))
        return True
    except SystemExit:
        return False


def flash_os(os_bin_path: str):
    """
    Flash the OS binary to the device using OpenOCD.

    Args:
        os_bin_path (str): Path to the OS binary file.

    Raises:
        SystemExit: If flashing fails.
    """
    print(f"Flashing {os_bin_path}...")
    cmd_flash = (
        f"openocd -f interface/stlink.cfg "
        f"-c 'transport select hla_swd' "
        f"-f target/stm32f7x.cfg "
        f"-c 'program {os_bin_path} 0x08000000' "
        f"-c exit"
    )
    print(cmd_flash)
    flash = subprocess.run(cmd_flash, shell=True, check=False)
    if flash.returncode != 0:
        print(f"Flashing {os_bin_path}... {Fore.RED}❌{Style.RESET_ALL}")
        sys.exit(1)
    print(f"Flashing {os_bin_path}... {Fore.GREEN}✅{Style.RESET_ALL}")

    time.sleep(1)

    print("Resetting robot...")
    cmd_reset = (
        "openocd -f interface/stlink.cfg "
        "-c 'transport select hla_swd' "
        "-f target/stm32f7x.cfg "
        "-c init -c 'reset run' "
        "-c exit"
    )
    print(cmd_reset)
    reset = subprocess.run(cmd_reset, shell=True, check=False)
    if reset.returncode != 0:
        print(f"Resetting robot... {Fore.RED}❌{Style.RESET_ALL}")
        sys.exit(1)
    print(f"Resetting robot... {Fore.GREEN}✅{Style.RESET_ALL}")

    time.sleep(1)


def erase_flash():
    """
    Erase the flash memory of the device using st-flash.

    Raises:
        SystemExit: If erasing fails.
    """
    print("Erasing flash...")
    cmd_erase = "st-flash --connect-under-reset --reset erase"
    ret = subprocess.run(cmd_erase, shell=True)
    if ret.returncode != 0:
        print(f"Erasing flash... {Fore.RED}❌{Style.RESET_ALL}")
        sys.exit(1)
    print(f"Erasing flash... {Fore.GREEN}✅{Style.RESET_ALL}")


def print_end_success(message: str):
    """
    Print a success message in cyan with a checkmark.

    Args:
        message (str): The message to print.
    """
    print(f"{Fore.CYAN}{message}... ✅{Style.RESET_ALL}")


def print_end_failure(message: str):
    """
    Print a failure message in red with a cross mark.

    Args:
        message (str): The message to print.
    """
    print(f"{Fore.RED}{message}... ❌{Style.RESET_ALL}")
