#!/usr/bin/env ruby

# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

require 'csv'

#
# MARK: - Deal with argv
#

def puts_help
	puts ""
	puts "Choose one of:"
	puts "   --check-signals <path/to/target/directory>"
	puts "   --fix-signals <path/to/target/directory>"
	puts "   --check-labels <path/to/target/directory>"
	puts "   --fix-labels <path/to/target/directory>"
	puts "   --rm-scripts <path/to/target/directory>"
	puts ""
	puts "Example:"
	puts "   ruby tools/ioc_editor.rb --check-labels targets/TARGET_LEKA_V1_2_DEV"
end

arguments_available = ["--check-signals", "--check-labels", "--fix-signals", "--fix-labels", "--rm-scripts", "--generate-csv"]
arguments_passed = ARGV

$arg_action = arguments_passed[0]
$arg_path   = arguments_passed[1]

if arguments_passed.include? "--help"
	puts_help
	exit
end

if arguments_passed.length != 2
	puts ""
	puts "❌ The script needs 2 arguments: <action> and <path>"
	puts_help
	exit
end

if !arguments_available.include? $arg_action
	puts ""
	puts "❌ #{$arg_action} not available..."
	puts_help
	exit
end

$check_signals = false
$check_labels  = false

(arguments_passed.include? "--check-signals") ? $check_signals = true : $check_signals = false
(arguments_passed.include? "--check-labels") ? $check_labels = true : $check_labels = false

$fix_signals   = false
$fix_labels    = false

(arguments_passed.include? "--fix-signals") ? $fix_signals = true : $fix_signals = false
(arguments_passed.include? "--fix-labels") ? $fix_labels = true : $fix_labels = false

$rm_scripts    = false
(arguments_passed.include? "--rm-scripts") ? $rm_scripts = true : $rm_scripts = false

$generate_csv = false
(arguments_passed.include? "--generate-csv") ? $generate_csv = true : $generate_csv = false


#
# MARK: - Variables
#

# .ioc File

$target_dir       = $arg_path
$ioc_file_prefix  = "Leka-MCU_Pins"
$ioc_file_version = $target_dir.scan(/(?<=V)[0-9]*_[0-9]*/)[0].sub!("_", ".")
$ioc_dir_path     = "#{$target_dir}/pins"
$ioc_file_name    = "#{$ioc_file_prefix}-v#{$ioc_file_version}.ioc"
$ioc_file_path    = "#{$ioc_dir_path}/#{$ioc_file_name}"

if !File.file?($ioc_file_path)
	puts ""
	puts "❌ #{$ioc_file_path} does not exist..."
	puts_help
	exit
end

puts ""
puts "Getting ioc file information..."
puts "   ioc file directory: #{$ioc_dir_path}"
puts "   ioc file name:      #{$ioc_file_name}"
puts "   ioc file version:   #{$ioc_file_version}"
puts "Getting ioc file information... ✅"

# CSV Files

$pin_csv_reference_file_path = "#{$ioc_dir_path}/#{$ioc_file_prefix}-v#{$ioc_file_version}.reference.csv"
$pin_csv_generated_file_path = "#{$ioc_dir_path}/#{$ioc_file_name}.csv"

if !File.exist?($pin_csv_generated_file_path) or $generate_csv
	puts ""
	puts "Generating .csv file from #{$ioc_file_path}..."
	IO.popen(['stm32cubemx', '-i'], 'w+') do |io|
		io.puts "config load #{$ioc_file_path}"
		io.puts "config load #{$ioc_file_path}"
		io.puts "csv pinout #{$pin_csv_generated_file_path}"
		io.puts "exit"
		io.close
	end
	puts "Generating .csv file from #{$ioc_file_path}... ✅"
end


$pin_csv_reference = CSV.parse(File.read($pin_csv_reference_file_path), headers: true)
$pin_csv_generated = CSV.parse(File.read($pin_csv_generated_file_path), headers: true)

# CSV Headers - Position, Name, Type, Signal, Label

$header_position = "Position"
$header_name     = "Name"
$header_type     = "Type"
$header_signal   = "Signal"
$header_label    = "Label"

# Script files

$fix_signals_file = "#{$ioc_dir_path}/fix_signals.txt"
$fix_labels_file  = "#{$ioc_dir_path}/fix_labels.txt"

#
# MARK: - Helper functions
#

def not_power_or_reset ( string )
	if string == "Power" || string == "Reset_State"
		return false
	end
	return true
end

def puts_error(pin_number, pin_name, expected, found)
	puts "#{pin_number} / #{pin_name} - expected: #{expected} -- found: #{found.empty? ? "empty" : found}"
end

def puts_okay(pin_number, pin_name, expected, found)
	puts "#{pin_number} / #{pin_name} - expected: #{expected} -- found: #{found.empty? ? "empty" : found}"
end

def puts_script_instructions(file)
	puts "Generating script to fix pins' labels... ✅"
	puts ""
	puts "You can run the fix signal script with the following:"
	puts ""
	puts "      $ stm32cubemx -i"
	puts ""
	puts "    MX> config load #{$ioc_file_path}"
	puts "    MX> script #{$ioc_dir_path}/#{File.basename(file)}"
	puts "    MX> config save"
	puts "    MX> csv pinout #{$pin_csv_generated_file_path}"
	puts ""
end

def puts_fix(fix)
	puts ""
	puts "Generating script to fix pins' #{fix.downcase}..."
end

def puts_check(check)
	puts ""
	puts "Checking pins' #{check.downcase}..."
end

def puts_check_ok(check)
	puts "Checking pins' #{check.downcase}... ✅"
end

def puts_check_error(check)
	puts "Checking pins' #{check.downcase}... ❌"
end

#
# MARK: - Checker functions
#

def compare_ref_and_ioc
	puts ""
	puts "Checking reference file and .ioc file have the same number of pins..."
	puts "   ref: #{$pin_csv_reference.length}"
	puts "   ioc: #{$pin_csv_generated.length}"
	if $pin_csv_reference.length == $pin_csv_generated.length
		puts "Checking reference file and .ioc file have the same number of pins... ✅"
	else
		puts "Checking reference file and .ioc file have the same number of pins... ❌"
		exit
	end
end

def check_name_and_position
	error_name_position = false

	def check ( header_name, row, index )
		pin_number = index + 1
		if row[header_name] != $pin_csv_generated[index][header_name]
			puts "#{pin_number} - ❌ #{header_name.downcase} - reference: #{row[header_name]} -- ioc: #{$pin_csv_generated[index][header_name]}"
			error_name_position = true
		end
	end

	puts_check "positions & names"

	$pin_csv_reference.each_with_index do |row, index|
		check $header_position, row, index
		check $header_name, row, index
	end

	if error_name_position
		puts_check_error "positions & names"
		exit
	else
		puts_check_ok "positions & names"
	end
end

def check_signals_or_labels(signals_or_labels)
	puts_check signals_or_labels
	error = false

	$pin_csv_reference.each_with_index do |row, index|
		pin_number = index + 1
		if row[signals_or_labels] != $pin_csv_generated[index][signals_or_labels] && not_power_or_reset(row[$header_signal])
			puts_error(pin_number, row[$header_name], row[signals_or_labels], $pin_csv_generated[index][signals_or_labels])
			error = true
		end
	end

	if error
		puts_check_error signals_or_labels
	else
		puts_check_ok signals_or_labels
	end

	return error
end

def fix_signals_or_labels(signals_or_labels)

	# Fix signal
	# set pin <pin name> <signal name>
	# set pin PA12 UART4_TX

	# Fix label
	# set gpio parameters <matching pin> <param name> <param value>
	# set gpio parameters PE4 GPIO_Label MOTOR_RIGHT_DIRECTION

	puts_fix signals_or_labels

	if signals_or_labels == $header_signal
		file = File.new($fix_signals_file, "w")
	elsif signals_or_labels == $header_label
		file = File.new($fix_labels_file, "w")
	end

	$pin_csv_reference.each_with_index do |row, index|
		pin_number = index + 1
		if row[signals_or_labels] != $pin_csv_generated[index][signals_or_labels] && not_power_or_reset(row[$header_signal])
			fix = "set pin #{row[$header_name]} #{row[signals_or_labels]}"
			puts fix
			file.puts fix
		end
	end

	file.close

	puts_script_instructions(file)
end

#
# MARK: - Main script
#

# Check for the basics
compare_ref_and_ioc
check_name_and_position

# Check  & fix signals
if $check_signals
	check_signals_or_labels $header_signal
	return
end

if $fix_signals
	if check_signals_or_labels $header_signal
		fix_signals_or_labels $header_signal
	else
		puts "No need to generate a script to fix pins' signals... 🎉"
	end
	return
end

if $check_labels
	check_signals_or_labels $header_label
	return
end

if $fix_labels
	if check_signals_or_labels $header_label
		fix_signals_or_labels $header_label
	else
		puts "No need to generate a script to fix pins' labels... 🎉"
	end
	return
end

# Remove files
if $rm_scripts
	puts ""
	puts "Removing script files..."
	File.delete($fix_labels_file) if File.exist?($fix_labels_file)
	File.delete($fix_signals_file) if File.exist?($fix_signals_file)
	puts "Removing script files... ✅"
end
