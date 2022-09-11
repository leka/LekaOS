#!/usr/bin/env python3

from colorama import Fore
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

parser = argparse.ArgumentParser(description='Read serial output')

parser.add_argument('-p', '--port', metavar='PORT', default='/dev/tty.usbmodem*',
                    help='serial port path used for the robot')

parser.add_argument('-l', '--loop-delay', metavar='SLEEP_DELAY', default=0.8,
                    help='time delay')

parser.add_argument('-f', '--fast', metavar='FAST', action=argparse.BooleanOptionalAction,
                    help='fast check with 0.2s loop delay')


args = parser.parse_args()

#
# MARK: - Serial
#

PORTS = glob.glob(args.port)
SERIAL_PORT = PORTS[0] if (len(PORTS) != 0) else args.port

REBOOT_TIMEOUT = 5.0  # in seconds
SERIAL_TIMEOUT = 0.1  # in seconds

MAX_GET_LINE_RETRIES = REBOOT_TIMEOUT / SERIAL_TIMEOUT

try:
    com = serial.Serial(SERIAL_PORT, 115200, timeout=SERIAL_TIMEOUT)
except serial.serialutil.SerialException as error:
    print(f"{error}")
    parser.print_help()
    sys.exit(1)

print(f"Connected to {com.name}")

#
# MARK: - Functions
#

filepath = "./fs/usr/test/file_saving.txt"


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


def create_saving_file():
    f = open(filepath, "w")
    f.close()


def write_saving_file(data):
    file = open(filepath, "a")
    file.write(data + '\n')
    file.close()


def print_saving_file():
    file = open(filepath, "r")
    content = file.read()
    print(content)
    file.close()


def delete_saving_file():
    os.remove(filepath)


def FilterOnRegEx(file):
    ploop = ("\s\smain\.cpp:[0-9].+:FAILED")
    pattern = re.compile(ploop)
    for line in file:
        match = pattern.search(line)
        if match is not None:
            yield line.strip()


def check_boost_ut_status():
    failed_tests = list()
    file = open(filepath, "r")
    for line in FilterOnRegEx(file):
        failed_tests.append(line)
    file.close()
    return failed_tests


def get_status(failed):
    ret = 0
    if (failed):
        ret = 1
        for f in failed:
            print("  -  " + f)

    return ret


def reboot_device():
    com.send_break()
    REBOOT_SLEEP_DELAY = 3
    sleep(REBOOT_SLEEP_DELAY)


#
# MARK: - Main script
#


LOOP_SLEEP_DELAY = float(args.loop_delay) if not args.fast else 0.2
FAILED_TESTS = list()
NO_RESPONSE_STATUS = True

FAILED_STR = Fore.RED + "❌FAILED"
SUCCESS_STR = Fore.GREEN + "✅SUCCESS"


def main():

    create_saving_file()

    global NO_RESPONSE_STATUS
    while True:
        data = wait_for_response()
        if data is not None:
            NO_RESPONSE_STATUS = False
            write_saving_file(data)
        elif NO_RESPONSE_STATUS:
            print("⚠️  Device not responding, rebooting now...")
            reboot_device()
        else:
            break
        sleep(LOOP_SLEEP_DELAY)

    FAILED_TESTS = check_boost_ut_status()

    delete_saving_file()

    ret = get_status(FAILED_TESTS)
    if (ret):
        print(FAILED_STR)
    else:
        print(SUCCESS_STR)
    return ret


if __name__ == '__main__':
    sys.exit(main())
