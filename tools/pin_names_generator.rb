#!/usr/bin/env ruby

# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

require 'csv'
require 'fileutils'


#
# MARK:- Deal with argv
#

def puts_help
	puts ""
end

arguments_passed = ARGV

if arguments_passed.length != 2
	puts ""
	puts "❌ The script needs 2 arguments: <input csv file> <output directory>"
	puts_help
	exit
end


#
# MARK:- Variables
#

# CSV File

$csv_file_path    = arguments_passed[0]
$csv_file_prefix  = "Leka-MCU_Pins"
$csv_file_version = File.basename(File.dirname($csv_file_path))
$csv_file_name    = File.basename($csv_file_path)
$csv_dir_path     = File.dirname($csv_file_path)

if !$csv_file_name.include? "reference"
	puts ""
	puts "❌ The file chosen is not a reference file..."
	puts_help
	exit
end

# CSV Headers - Position, Name, Type, Signal, Label

$header_position = "Position"
$header_name     = "Name"
$header_type     = "Type"
$header_signal   = "Signal"
$header_label    = "Label"

# Leka Pin Names File

$target_directory_path = arguments_passed[1].chomp("/")
$pin_names_file_name   = "PinNames.h"
$pin_names_file_path   = $target_directory_path + "/" + $pin_names_file_name

$pin_names_orig_file_name   = "PinNames.orig"
$pin_names_orig_file_path   = $target_directory_path + "/" + $pin_names_orig_file_name


#
# MARK:- Process
#

# Output files information

puts ""
puts "Getting files information..."
puts "   csv file directory:  #{$csv_dir_path}"
puts "   csv file name:       #{$csv_file_name}"
puts "   csv file version:    #{$csv_file_version}"
puts "   pin names directory: #{$target_directory_path}"
puts "   pin names file name: #{$pin_names_file_name}"
puts "   pin names orig name: #{$pin_names_orig_file_name}"
puts "Getting file information... ✅"

# Parse CSV file

puts ""
puts "Parsing #{$csv_file_name} file..."

$pins = CSV.parse(File.read($csv_file_path), headers: true)

### Create a sorted 2D array with the label and pin name: ["pin label", "pin name"]

$pins_array = []

$pins.each_with_index do |pin, index|
	unless pin[$header_label].to_s.strip.empty?
		label = pin[$header_label]
		name  = pin[$header_name].insert(2, "_")
		if index = name.index("/")
			name = pin[$header_name][0..index-1]
		end
		output = [label, name]
		$pins_array.append(output)
	end
end

$pins_array.sort!

### Group pins by function (BLE, BT, SENSORS, etc.)

$grouped_pins_array = []

$pins_array.each_with_index do |pin, index|
	$grouped_pins_array.append(pin)
	if index + 1 < $pins_array.length
		unless pin[0].to_s[0..1] == $pins_array[index + 1][0].to_s[0..1]
			$grouped_pins_array.append(["", ""])
		end
	end
end

puts "Parsing #{$csv_file_name} file... ✅"

# Create output data to be inserted

puts ""
puts "Creating pin names data from parsed #{$csv_file_name} file..."

$pin_names_output_data = []

$pin_names_output_data.append("\n")
$pin_names_output_data.append("    // Leka Pin Names")

output = ""
$grouped_pins_array.each do |label, name|
	if [label, name] == ["", ""]
		unless output.empty?
			output = ""
			$pin_names_output_data.append output
		end
	else
		unless label.include? "MIPI" or label.include? "FMC"
			output = label + " = " + name + ","
			$pin_names_output_data.append output
		end
	end
end

$pin_names_output_data.append("\n")
$pin_names_output_data.append("LED1 = DEBUG_LED,")
$pin_names_output_data.append("LED2 = NC,")
$pin_names_output_data.append("LED3 = NC,")
$pin_names_output_data.append("\n")
$pin_names_output_data.append("USER_BUTTON = NC,")
$pin_names_output_data.append("\n")

puts "Creating pin names data from parsed #{$csv_file_name} file... ✅"

puts ""
puts "Parsing original #{$pin_names_orig_file_path} file..."

$pin_names_file_data = File.readlines($pin_names_orig_file_path).map(&:chomp)
$insert_at_index     = 0

$pin_names_file_data.each_with_index do |line, index|
	regex = /(\bSTDIO_|LED[0-9]|USB_OTG|ETH_|= D[0-9]|ARDUINO_UNO_|QSPI_FLASH1_|BUTTON)/
	if line.match regex
		$pin_names_file_data[index] = "// #{$pin_names_file_data[index]}"
	end
	if line.include? "} PinName;"
		$insert_at_index = index - 1
		break
	end
end

$pin_names_file_data.insert($insert_at_index, $pin_names_output_data)

puts "Parsing original #{$pin_names_orig_file_path} file... ✅"

puts ""
puts "Writting final output to #{$pin_names_file_name} file..."

File.open($pin_names_file_path, 'w') do |file|
	file.puts $pin_names_file_data
end

puts "Writting final output to #{$pin_names_file_name} file... ✅"

puts ""
puts "Running clang-format to format #{$pin_names_file_name} file..."

system("clang-format", "-i", "-style=file", "#{$pin_names_file_path}")

puts "Running clang-format to format #{$pin_names_file_name} file... ✅"
