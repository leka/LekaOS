#!/usr/bin/env python3

import os
import glob
import sys
import argparse

from time import sleep

import serial
import serial.tools.list_ports


#
# MARK: - argparse
#

parser = argparse.ArgumentParser(description='Control /fs content')

parser.add_argument('-p', '--port', metavar='PORT', default='/dev/tty.usbmodem*',
                    help='serial port path used for the robot')

parser.add_argument('-d', '--dir', metavar='DIR', default='fs',
                    help='directory to control, relative to LekaOS root')

parser.add_argument('--test', action=argparse.BooleanOptionalAction,
                    help='Run tests with /fs/usr/test')

args = parser.parse_args()


#
# MARK: - Serial
#

PORTS = glob.glob(args.port)
SERIAL_PORT = PORTS[0] if (len(PORTS) != 0) else args.port

REBOOT_TIMEOUT = 5  # in seconds
SERIAL_TIMEOUT = .1  # in seconds

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


def read_status():
    return com.readline().decode("utf-8")


def send_path(file_path):
    file_path = f"/{file_path}"
    print(os.path.basename(file_path) + " ... ", end='')
    sys.stdout.flush()
    com.write(file_path.encode("utf-8"))


def wait_for_status():
    status = ''
    no_response_counter = 0

    while(no_response_counter <= MAX_GET_LINE_RETRIES):
        sleep(.005)
        status = read_status()

        if (status):
            return status

        no_response_counter += 1

    return None


def check_status(file, status):
    if 'ACK_' in status:
        print("✅")
        return True

    if 'NOK_' in status:
        print("❌ ", end='')
        if 'MISSING' in status:
            print("file is missing")
            FILES_MISSING.append(file)
        if 'EMPTY' in status:
            print("file is empty")
            FILES_EMPTY.append(file)
        if 'NOT_OPEN' in status:
            print("file could not be open")
            FILES_BROKEN.append(file)


def reboot_device():
    com.send_break()
    REBOOT_SLEEP_DELAY = 3
    sleep(REBOOT_SLEEP_DELAY)


def create_missing_file():
    f = open("./fs/usr/test/file-missing.txt", "w")
    f.write("missing file created")
    f.close()


def delete_missing_file():
    os.remove("./fs/usr/test/file-missing.txt")


def print_report(broken, empty, missing, reboot):
    ret = 0

    if broken:
        print("\nBroken files:")
        ret = 1
        for f in broken:
            print("  - ./" + f)

    if empty:
        print("\nEmpty files:")
        ret = 1
        for f in empty:
            print("  - ./" + f)

    if missing:
        print("\nMissing files:")
        ret = 1
        for f in missing:
            print("  - ./" + f)

    if reboot:
        print("\nReboot files:")
        ret = 1
        for f in reboot:
            print("  - ./" + f)

    return ret


#
# MARK: - Main script
#

# Relative path from LekaOS root
ROOT_PATH = args.dir if not args.test else 'fs/usr/test'

FILES_BROKEN = list()
FILES_EMPTY = list()
FILES_MISSING = list()
FILES_REBOOT = list()


def main():
    number_of_dirs = 0

    create_missing_file()

    for (root, dirs, files) in os.walk(ROOT_PATH):
        files = [f for f in files if not f[0] == '.']
        dirs[:] = [d for d in dirs if not d[0] == '.']

        if not args.test:
            files = [f for f in files if not 'test' in f]
            dirs[:] = [d for d in dirs if not 'test' in d]

        if files:
            number_of_dirs += 1

    dir_index = 1

    for (root, dirs, files) in os.walk(ROOT_PATH):
        files = [f for f in files if not f[0] == '.']
        dirs[:] = [d for d in dirs if not d[0] == '.']

        if not args.test:
            files = [f for f in files if not 'test' in f]
            dirs[:] = [d for d in dirs if not 'test' in d]

        files.sort()
        dirs.sort()

        if not files:
            continue

        zero_fill = len(f"{number_of_dirs}")
        print(
            f"\n[{str(dir_index).zfill(zero_fill)}/{number_of_dirs}] /{root}")
        dir_index += 1

        for index, file in enumerate(files):
            file_path = os.path.join(root, file)
            zero_fill = len(f"{len(files)}")

            print(
                f"  [{str(index + 1).zfill(zero_fill)}/{len(files)}] ", end='')

            send_path(file_path)

            status = wait_for_status()

            if status is not None:
                check_status(file_path, status)
            else:
                print("⚠️  Device not responding, rebooting now...")
                FILES_REBOOT.append(file_path)
                reboot_device()

            LOOP_SLEEP_DELAY = 1  # in seconds
            sleep(LOOP_SLEEP_DELAY)

    delete_missing_file()

    ret = print_report(FILES_BROKEN, FILES_EMPTY, FILES_MISSING, FILES_REBOOT)
    return ret


if __name__ == '__main__':
    sys.exit(main())
