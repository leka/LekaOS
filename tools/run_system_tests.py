#!/usr/bin/env python3

# Leka - LekaOS
# Copyright 2024 APF France handicap
# SPDX-License-Identifier: Apache-2.0

from colorama import Fore, Style
from time import sleep
import argparse
import glob
import re
import serial
import serial.tools.list_ports
import subprocess
import sys


#
# MARK: - argparse
#


OS_BIN_FILE_PATH = "_build/LEKA_V1_2_DEV/app/os/LekaOS.bin"

parser = argparse.ArgumentParser(description='Run functional tests')

parser.add_argument('-p', '--port', metavar='PORT',
                    default='/dev/tty.usbmodem*',
                    help='serial port path used for the robot')

parser.add_argument('--response-timeout', metavar='RESPONSE_TIMEOUT',
                    default=30.0,
                    help='response timeout is seconds')

parser.add_argument('--no-flash-erase', action='store_false',
                    help='disable flash erase')

args = parser.parse_args()


#
# MARK: - Serial
#

print("Hello, System Tests! 🚀")


PORTS = glob.glob(args.port)
SERIAL_PORT = PORTS[0] if (len(PORTS) != 0) else args.port

RESPONSE_TIMEOUT = args.response_timeout  # in seconds
RESPONSE_RETRY_DELAY = 0.1  # in seconds
SERIAL_TIMEOUT = 0.1  # in seconds

MAX_GET_LINE_RETRIES = RESPONSE_TIMEOUT / RESPONSE_RETRY_DELAY


def connect_serial():
    try:
        global com
        com = serial.Serial(SERIAL_PORT, 115200, timeout=SERIAL_TIMEOUT)
        print_start(f"Connecting to {com.name}")
    except serial.serialutil.SerialException as error:
        print_end_failure(f"Connecting to {com.name}")
        print(f"{error}")
        parser.print_help()
        sys.exit(1)

    print_end_success(f"Connecting to {com.name}")


def read_output_serial():
    return com.readline().decode("utf-8")


def wait_for_response():
    data = ''
    no_response_counter = 0

    while (no_response_counter <= MAX_GET_LINE_RETRIES):
        sleep(RESPONSE_RETRY_DELAY)
        data = read_output_serial()
        if (data):
            return data
        no_response_counter += 1

    return None


#
# MARK: - Functions
#

def print_start(message):
    print(Fore.CYAN + f"\n{message}..." + Style.RESET_ALL)


def print_end_success(message):
    print(Fore.CYAN + f"{message}... ✅" + Style.RESET_ALL)


def print_end_failure(message):
    print(Fore.RED + f"{message}... ❌" + Style.RESET_ALL)


def flash_os():
    print_start(f"Flashing {OS_BIN_FILE_PATH}")
    CMD_FLASH = (f"openocd -f interface/stlink.cfg "
                 f"-c 'transport select hla_swd' "
                 f"-f target/stm32f7x.cfg "
                 f"-c 'program {OS_BIN_FILE_PATH} 0x08000000' "
                 f"-c exit ")
    print(CMD_FLASH)
    flash = subprocess.run(CMD_FLASH, shell=True)
    if not flash:
        print_end_failure(f"Flashing {OS_BIN_FILE_PATH}")
        sys.exit(1)
    else:
        print_end_success(f"Flashing {OS_BIN_FILE_PATH}")

    sleep(1)

    print_start("Reseting robot")
    CMD_RESET = ("openocd -f interface/stlink.cfg "
                 "-c 'transport select hla_swd' "
                 "-f target/stm32f7x.cfg "
                 "-c init -c 'reset run' "
                 "-c exit ")
    print(CMD_FLASH)
    reset = subprocess.run(CMD_RESET, shell=True)
    if not reset:
        print_end_failure("Reseting robot")
        sys.exit(1)
    else:
        print_end_success("Reseting robot")

    sleep(1)


#
# MARK: - Main script
#


def erase_flash():
    print_start("Erasing flash")
    ret = subprocess.run("st-flash --connect-under-reset --reset erase", shell=True)
    if not ret:
        print_end_failure("Erasing flash")
        sys.exit(1)
    else:
        print_end_success("Erasing flash")


def reset_buffer():
    print_start("Resetting com buffer")
    BREAK_DELAY = 1
    com.reset_input_buffer()
    com.reset_output_buffer()
    com.send_break(BREAK_DELAY)
    sleep(BREAK_DELAY)
    print_end_success("Resetting com buffer")


def wait_for_system_to_sleep(duration=180):
    print_start(f"Waiting for LekaOS to run for {duration} seconds")
    data = []
    for second in range(duration):
        if com.in_waiting > 0:
            print(Fore.GREEN + "•" + Style.RESET_ALL, end='', flush=True)
            lines = com.readlines()
            for line in lines:
                line = str(line, 'utf-8', errors='replace').rstrip()
                data.append(line)
        else:
            print("•", end='', flush=True)

        if (second + 1) % 60 == 0 and (second + 1) != duration:
            print()

        sleep(1)

    print()
    data = list(filter(lambda string: 'watchdog' in string, data))
    print("\n".join(data))
    print_end_success(f"Waiting for LekaOS to run for {duration} seconds")
    return data[-10:]


def calculate_sleep_deep_statistics(lines):
    def parse_line(line):
        pattern = re.compile(r'slp:\s*(\d+)%.*?dsl:\s*(\d+)%')
        match = pattern.search(line)
        if match:
            return map(int, match.groups())
        return None, None

    print_start("Analyzing sleep data")

    if not lines:
        print("No sleep data found")
        print_end_failure("Analyzing sleep data")
        sys.exit(1)

    sum_sleep, sum_deep_sleep, count = 0, 0, 0

    for line in lines:
        sleep, deep_sleep = parse_line(line)
        if sleep is not None and deep_sleep is not None:
            sum_sleep += sleep
            sum_deep_sleep += deep_sleep
            count += 1

    if count == 0:
        print_end_failure("Analyzing sleep data")
        sys.exit(1)

    print_end_success("Analyzing sleep data")
    return sum_sleep / count, sum_deep_sleep / count

#
# MARK: - Main
#


FLASH_ERASE_FLAG = args.no_flash_erase


def main():
    connect_serial()

    if FLASH_ERASE_FLAG:
        erase_flash()

    flash_os()

    reset_buffer()

    data = wait_for_system_to_sleep(180)

    sleep, deep_sleep = calculate_sleep_deep_statistics(data)

    print()

    print(Fore.CYAN + f"Average sleep: {sleep}%" + Style.RESET_ALL)
    print(Fore.CYAN + f"Average deep sleep: {deep_sleep}%" + Style.RESET_ALL)

    if deep_sleep > 95:
        print(Fore.GREEN + "Deep sleep is higher than 95%, this is good! ✅" + Style.RESET_ALL)
        return 0
    else:
        print(Fore.RED + "Deep sleep is lower than 95%, this is bad! ❌" + Style.RESET_ALL)
        return 1


if __name__ == '__main__':
    sys.exit(main())
