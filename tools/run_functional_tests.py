#!/usr/bin/env python3


import datetime
import time
from colorama import Fore, Style
import os
import glob
import re
import sys
import argparse

from time import sleep

import serial
import serial.tools.list_ports


#
# MARK: - argparse
#

TESTS_FUNCTIONAL_ROOT_DIRECTORY = "_build/LEKA_V1_2_DEV/tests/functional/tests/"
TESTS_BIN_EXTENSION = ".bin"


def valid_file(parser, arg):
    path = os.path.join(TESTS_FUNCTIONAL_ROOT_DIRECTORY, arg)
    if not os.path.exists(path):
        parser.error("⚠️ The file %s does not exist!" % arg)
    else:
        base, ext = os.path.splitext(arg)
        if not ext.endswith(TESTS_BIN_EXTENSION):
            parser.error(
                "⚠️ The filename %s must have a \'.bin\' extension !" % arg)
    return path


parser = argparse.ArgumentParser(description='Run functional tests')

parser.add_argument('-p', '--port', metavar='PORT', default='/dev/tty.usbmodem*',
                    help='serial port path used for the robot')
parser.add_argument('--response-timeout', metavar='RESPONSE_TIMEOUT', default=5.0,
                    help='response timeout')
parser.add_argument('--no-flash-erase', action='store_false',
                    help='disable flash erase')

group = parser.add_mutually_exclusive_group(required=True)

group.add_argument('-b', '--bin-files', metavar='BIN_FILES', nargs='+', type=lambda s: valid_file(parser, s), default=list(),
                   help='list binary executables')

group.add_argument('--all', action='store_true',
                   help='select all binary executable')


args = parser.parse_args()

#
# MARK: - Serial
#

PORTS = glob.glob(args.port)
SERIAL_PORT = PORTS[0] if (len(PORTS) != 0) else args.port

RESPONSE_TIMEOUT = float(args.response_timeout)   # in seconds
SERIAL_TIMEOUT = 0.1  # in seconds

MAX_GET_LINE_RETRIES = RESPONSE_TIMEOUT / SERIAL_TIMEOUT

try:
    com = serial.Serial(SERIAL_PORT, 115200, timeout=SERIAL_TIMEOUT)
except serial.serialutil.SerialException as error:
    print(f"{error}")
    parser.print_help()
    sys.exit(1)

print(f"Connected to {com.name}")


def read_output_serial():
    return com.readline().decode("utf-8")


def wait_for_response():
    data = ''
    no_response_counter = 0

    while (no_response_counter <= MAX_GET_LINE_RETRIES):
        sleep(.005)
        data = read_output_serial()
        if (data):
            return data
        no_response_counter += 1

    return None


#
# MARK: - Functions
#

TESTS_FUNCTIONAL_BIN_FILES = list()


def list_bin_files():
    set = list()
    for root, dirs, files in os.walk(TESTS_FUNCTIONAL_ROOT_DIRECTORY):
        for filename in files:
            if filename.endswith(TESTS_BIN_EXTENSION):
                set.append(
                    os.path.join(root, filename))

    return set


TESTS_FUNCTIONAL_BIN_FILES = list_bin_files() if args.all else args.bin_files

FLASH_ERASE_FLAG = args.no_flash_erase


def warningprint(*args, **kwargs):
    print(Fore.YELLOW + "\n⚠️ Warning : " +
          " ".join(map(str, args))+Style.RESET_ALL, **kwargs)


#
# MARK: - Class Test
#

class Test:

    def __init__(self, path):
        self.path = path

    def generate_result_file(self):
        def define_path(source_path):
            base, ext = os.path.splitext(source_path)
            timestamp = time.time()
            date = str(datetime.datetime.fromtimestamp(
                timestamp))
            date = date.replace(':', '.')
            date = date.replace(' ', '_')
            new_extension = ".txt"
            target_path = base + "_" + date + new_extension
            return target_path

        def create_file(path):
            try:
                file = open(path, "w")
            except OSError as e:
                print("Could not create or open file: " + path)
                print("Error: " + e)
                sys.exit(1)
            file.close()

        self.result_filepath = define_path(self.path)
        create_file(self.result_filepath)

    def edit_result_file(self, data):
        result_filepath = self.result_filepath
        try:
            with open(result_filepath, "a") as file:
                file.write(data)
        except FileNotFoundError as e:
            print("The file: " + result_filepath + "doesn\'t exist")
            print("Error: " + e)
            sys.exit(1)

    def print_result_file(self):
        result_filepath = self.result_filepath
        try:
            with open(result_filepath, "r") as file:
                data = file.read()
            if (data):
                print(data)
            else:
                warningprint("No data !")
        except FileNotFoundError as e:
            print("The file: " + result_filepath + "doesn\'t exist")
            print("Error: " + e)
            sys.exit(1)

    def flash(self):
        print(f"Flashing {self.path}...")
        CMD_FLASH = (f"openocd -f interface/stlink.cfg "
                     f"-c 'transport select hla_swd' "
                     f"-f target/stm32f7x.cfg "
                     f"-c 'program {self.path} 0x08000000' "
                     f"-c exit "
                     f">/dev/null 2>&1 ")
        flash = os.system(CMD_FLASH)

        sleep(1)

        CMD_RESET = ("openocd -f interface/stlink.cfg "
                     "-c 'transport select hla_swd' "
                     "-f target/stm32f7x.cfg "
                     "-c init -c 'reset run' "
                     "-c exit "
                     f">/dev/null 2>&1 ")
        reset = os.system(CMD_RESET)
        return flash or reset

    def run(self):
        self.generate_result_file()
        ret = self.flash()

        if ret:
            warningprint("Error flashing !")
            return ret
        else:
            while True:
                data = wait_for_response()
                if data is not None:
                    if data.strip() != ".":
                        self.edit_result_file(data)
                else:
                    return ret

    def check_status(self):

        def all_tests_passed(file):
            ploop = (".*All tests passed.+\(\d+ asserts in \d+ tests\)")
            pattern = re.compile(ploop)
            ret = False
            for line in file:
                match = pattern.search(line)
                if match is not None:
                    ret = True
                    break
            return ret

        def failure_lines(file):
            ploop = (".*\.cpp:[0-9].+:.*FAILED")
            pattern = re.compile(ploop)
            for line in file:
                match = pattern.search(line)
                if match is not None:
                    yield line.strip()

        ret = 0
        result_filepath = self.result_filepath

        try:
            with open(result_filepath, "r") as file:
                self.failures = list()
                for line in failure_lines(file):
                    self.failures.append(line)
                file.seek(0, 0)
                if len(self.failures) or not all_tests_passed(file):
                    print("Failures : " + str(self.failures))
                    print("All test passed : "+str(all_tests_passed(file)))
                    ret = 1

        except FileNotFoundError as e:
            print("The file: " + result_filepath + "doesn\'t exist")
            print("Error: " + e)
            sys.exit(1)

        return ret


def print_summary():

    if not RUN_TESTS:
        warningprint("No available set !")
    else:
        FAILS = list()
        print("\n")
        print("Results files :")
        for test in RUN_TESTS:
            print(test.result_filepath)
            fail = test.check_status()
            if fail:
                FAILS.append(test)

        print("\n")
        print("{:<100} {:<7}".format('BIN_PATH', 'STATUS'))
        for test in RUN_TESTS:
            path = test.path
            status = "❌" if test in FAILS else "✅"
            print("{:<100} {:^7}".format(path, status))

        print("\n")
        for test in FAILS:
            print(Fore.YELLOW + test.result_filepath + Style.RESET_ALL)
            test.print_result_file()

        if (FAILS):
            print(Fore.RED + " ❌ %d in %d suites have failed..." % (len(FAILS), len(RUN_TESTS)) +
                  Style.RESET_ALL)
        else:
            print(Fore.GREEN + " ✅ All the %d suites have passed !" % len(RUN_TESTS) +
                  Style.RESET_ALL)

        ret = len(FAILS)
        return ret


#
# MARK: - Main script
#

RUN_TESTS = list()


def flash_erase():
    ret = os.system("st-flash erase")
    return ret


def reset_buffer():
    BREAK_DELAY = 1
    com.reset_input_buffer()
    com.reset_output_buffer()
    com.send_break(BREAK_DELAY)
    sleep(BREAK_DELAY)


def main():
    ret = 0

    if not TESTS_FUNCTIONAL_BIN_FILES:
        warningprint("No exec !")
        sys.exit(1)

    if FLASH_ERASE_FLAG:
        flash_erase()
    reset_buffer()

    print("Running tests...")
    for filepath in TESTS_FUNCTIONAL_BIN_FILES:
        test = Test(filepath)
        error = test.run()
        if not error:
            RUN_TESTS.append(test)

    if FLASH_ERASE_FLAG:
        flash_erase()
    reset_buffer()

    fails = print_summary()
    if fails:
        ret = 1

    return ret


if __name__ == '__main__':
    sys.exit(main())
