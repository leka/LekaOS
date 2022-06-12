#!/usr/bin/env python3

import os
import glob
import sys
import getopt

from time import sleep
import serial
import serial.tools.list_ports


#
# MARK: - Variables
#

ports = glob.glob('/dev/tty.usbmodem*')
serial_port = ports[0] if (len(ports) != 0) else ''

source_directory = 'fs'  # Relative path from LekaOS root

interval_between_two_response = 1  # in seconds
delay_to_reboot = 5  # in seconds

start_from_index = 0
delete_file_occuring_reboot = False


#
# MARK: - Help
#

def printHelp():
    print("./tools/display_image_via_serial.py\n\t"
          "--help\n\t"
          "--serial_port <serial_port>\n\t"
          "--are_images\n\t"
          "--are_videos\n\t"
          "--source_directory <source_directory>\n\t"
          "--interval_between_two_response <interval_between_two_response>,\n\t"
          "--delay_to_reboot <delay_to_reboot>,\n\t"
          "--start_from_index <start_from_index>\n\t"
          "--delete_file_occuring_reboot\n\t")


#
# MARK: - Handle args
#

argv = sys.argv[1:]
try:
    opts, args = getopt.getopt(argv, "", ["help",
                                          "serial_port=",
                                          "are_images",
                                          "are_video",
                                          "source_directory=",
                                          "interval_between_two_response=",
                                          "delay_to_reboot=",
                                          "start_from_index=",
                                          "delete_file_occuring_reboot"])
except getopt.GetoptError:
    printHelp()
    sys.exit(2)
for opt, arg in opts:
    if opt in ("--help"):
        printHelp()
        sys.exit(0)
    elif opt in ("--serial_port"):
        serial_port = arg
    elif opt in ("--are_images"):
        source_directory = "fs/home/img"
    elif opt in ("--are_videos"):
        source_directory = "fs/home/vid"
    elif opt in ("--source_directory"):
        source_directory = arg
    elif opt in ("--interval_between_two_response"):
        interval_between_two_response = float(arg)
    elif opt in ("--delay_to_reboot"):
        delay_to_reboot = float(arg)
    elif opt in ("--start_from_index"):
        start_from_index = int(arg)
    elif opt in ("--delete_file_occuring_reboot"):
        delete_file_occuring_reboot = True


#
# MARK: - Serial
#

if serial_port == "":
    print("serial_port cannot be empty.\n")
    printHelp()
    sys.exit(1)

com_timeout = .1  # in seconds

try:
    com = serial.Serial(serial_port, 115200, timeout=com_timeout)
except serial.serialutil.SerialException as error:
    print(f"{error}")
    printHelp()
    sys.exit(1)

print(f"Connected to {com.name}")

line = ''
iter_without_response = 0
no_response_limit = delay_to_reboot / com_timeout


#
# MARK: - Images
#

# destination_directory = '/' + source_directory  # Absolute path from SD root


#
# MARK: - Functions
#

def getLine():
    return com.readline().decode("utf-8")


def sendLine(file_path):
    # full_path = destination_directory + '/' + filename
    file_path = f"/{file_path}"
    print(os.path.basename(file_path))
    com.write(file_path.encode("utf-8"))


def waitResponse():
    global line
    global iter_without_response
    line = ''
    iter_without_response = 0
    while(not(line) and (iter_without_response < no_response_limit)):
        sleep(.005)
        line = getLine()
        iter_without_response += 1


def responseIsValid():
    return line.find('ACK') != -1


def needReboot():
    must_reboot = iter_without_response >= no_response_limit
    if(must_reboot):
        com.send_break()
        sleep(5)
        return True


def deleteFile(filename):
    os.remove(source_directory + '/' + filename)


#
# MARK: - Main script
#

def main():
    number_of_dirs = 0

    for (root, dirs, files) in os.walk(source_directory):
        files = [f for f in files if not f[0] == '.']
        dirs[:] = [d for d in dirs if not d[0] == '.']

        if files:
            number_of_dirs = number_of_dirs + 1

    dir_index = 1

    for (root, dirs, files) in os.walk(source_directory):
        files = [f for f in files if not f[0] == '.']
        dirs[:] = [d for d in dirs if not d[0] == '.']

        if not files:
            continue

        zero_fill = len(f"{number_of_dirs}")
        print(
            f"\n[{str(dir_index).zfill(zero_fill)}/{number_of_dirs}] /{root}")
        dir_index = dir_index + 1

        for index, file in enumerate(files):
            file_path = os.path.join(root, file)
            zero_fill = len(f"{len(files)}")

            print(
                f"  [{str(index + 1).zfill(zero_fill)}/{len(files)}] ", end='')

            sendLine(file_path)

            waitResponse()

            if(responseIsValid()):
                print("file exist")
            else:
                print("file not exit")
                continue

            if file_path.endswith(('.jpg', '.avi')):
                waitResponse()

                if(responseIsValid()):
                    print("file can be opened")

            # if(needReboot()):
            #     if(delete_file_occuring_reboot):
            #         deleteFile(image_files[index])
            #     continue
            # print('ret' + line)
            # if(responseIsValid()):
            #     print("file exist")
                # index += 1

            sleep(interval_between_two_response)

    return 0


if __name__ == '__main__':
    sys.exit(main())
