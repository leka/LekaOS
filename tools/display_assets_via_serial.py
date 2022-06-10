#!/usr/bin/env python3

import os
import sys
import getopt

from time import sleep
import serial


#
# MARK: - Variables
#

serial_port = '/dev/tty.usbmodem14103'

source_directory = 'fs/home/img'  # Relative path from LekaOS root

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

com_timeout = .1

try:
    com = serial.Serial(serial_port, 115200, timeout=com_timeout)
except serial.serialutil.SerialException as error:
    print(f"Device not connected to {serial_port}, {error}\n")
    printHelp()
    sys.exit(1)

line = ''
iter_without_response = 0
no_response_limit = delay_to_reboot / com_timeout


#
# MARK: - Images
#

destination_directory = '/' + source_directory  # Absolute path from SD root

index = start_from_index
image_files = os.listdir(source_directory)
image_files.sort()


#
# MARK: - Functions
#

def getLine():
    return com.readline().decode("utf-8")


def sendLine(filename):
    full_path = destination_directory + '/' + filename
    print(full_path)
    com.write(full_path.encode("utf-8"))


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

# def main():
while((index < len(image_files))):
    print(f"{index + 1:04d} / {len(image_files):04d} : ", end='')
    sendLine(image_files[index])

    waitResponse()

    if(needReboot()):
        if(delete_file_occuring_reboot):
            deleteFile(image_files[index])
        continue
    if(responseIsValid()):
        index += 1
    sleep(interval_between_two_response)


# if __name__ == '__main__':
#     sys.exit(main())

# main()
