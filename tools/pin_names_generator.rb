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

$leka_pinmap_path      = arguments_passed[1].chomp("/")
$leka_pinmap_file_name = "LekaPinNames.h"
$leka_pinmap_file_path = $leka_pinmap_path + "/" + $leka_pinmap_file_name


#
# MARK:- Process
#

# Output files information

puts ""
puts "Getting files information..."
puts "   csv file directory: #{$csv_dir_path}"
puts "   csv file name:      #{$csv_file_name}"
puts "   csv file version:   #{$csv_file_version}"
puts "   pinmap directory:   #{$leka_pinmap_path}"
puts "   pinmap file name:   #{$leka_pinmap_file_name}"
puts "Getting file information... ✅"

# Parse CSV file

$pins = CSV.parse(File.read($csv_file_path), headers: true)

# Create a sorted 2D array with the label and pin name: ["pin label", "pin name"]

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

# Group pins by function (BLE, BT, SENSORS, etc.)

$grouped_pins_array = []

$pins_array.each_with_index do |pin, index|
	$grouped_pins_array.append(pin)
	if index + 1 < $pins_array.length
		unless pin[0].to_s[0..1] == $pins_array[index + 1][0].to_s[0..1]
			$grouped_pins_array.append(["", ""])
		end
	end
end

# Create LekaPinNames.h and create back-up if needed

puts ""
puts "Checking if #{$leka_pinmap_file_path} already exists..."

if(File.exist?($leka_pinmap_file_path))
	puts "   #{$leka_pinmap_file_path} already exists, creating a backup..."
	FileUtils.cp $leka_pinmap_file_path, "#{$leka_pinmap_file_path}.#{Time.now.strftime("%Y%m%d%H%M%S")}.bak"
	puts "   #{$leka_pinmap_file_path} already exists, creating a backup... ✅"
else
	puts "   #{$leka_pinmap_file_path} doesn't exist, creating file..."
	FileUtils.touch $leka_pinmap_file_path
	puts "   #{$leka_pinmap_file_path} doesn't exist, creating file... ✅"
end

puts "Checking if #{$leka_pinmap_file_path} already exists... ✅"

# Add content to LekaPinNames.h

puts ""
puts "Writing content to #{$leka_pinmap_file_path}..."

file = File.new($leka_pinmap_file_path, "w")

file.puts """
// Leka - LekaOS
// Copyright #{Time.now.strftime("%Y")} APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LEKA_PIN_NAMES_H_
#define _LEKA_OS_LEKA_PIN_NAMES_H_

#include \"PinNames.h\"

// LekaPinNames.h v#{$csv_file_version}
// Generated on #{Time.now.strftime("%Y/%m/%d")}

"""

output = ""
$grouped_pins_array.each do |label, name|
	if [label, name] == ["", ""]
		unless output.empty?
			output = ""
			file.puts output
		end
	else
		unless label.include? "MIPI" or label.include? "FMC"
			output = "constexpr PinName " + label.ljust(28) + " = " + name + ";"
			file.puts output
		end
	end
end

file.puts """
#endif // _LEKA_OS_LEKA_PIN_NAMES_H_
"""

puts "Writing content to #{$leka_pinmap_file_path}... ✅"
