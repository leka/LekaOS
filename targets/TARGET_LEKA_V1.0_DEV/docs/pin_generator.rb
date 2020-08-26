#!/usr/bin/env ruby

require 'csv'

#
# MARK:- Deal with argv
#

def puts_help
	puts "Choose one of:"
	puts "   --check-signals"
	puts "   --fix-signals"
	puts "   --check-labels"
	puts "   --fix-labels"
	puts "   --rm-scripts"
end

arguments_available = ["--check-signals", "--check-labels", "--fix-signals", "--fix-labels", "--rm-scripts"]
arguments_passed = ARGV

if arguments_passed.length > 1
	puts "❌ Too many arguments..."
	puts_help
	exit
end

arguments_passed.each do |arg|
	if !arguments_available.include? arg
		puts "❌ #{arg} not available..."
		puts_help
		exit
	end
end

if arguments_passed.include? "--help"
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

#
# MARK:- Variables
#

# .ioc File

$mcu_file_prefix  = "Leka-MCU_Pins"
$mcu_file_version = "1.0.0"
$ioc_file         = "#{$mcu_file_prefix}-v#{$mcu_file_version}.ioc"

# CSV Files

pin_csv_reference_file = "#{$mcu_file_prefix}-v#{$mcu_file_version}-reference.csv"
pin_csv_generated_file = "#{$mcu_file_prefix}-v#{$mcu_file_version}-generated.csv"

$pin_csv_reference = CSV.parse(File.read(pin_csv_reference_file), headers: true)
$pin_csv_generated = CSV.parse(File.read(pin_csv_generated_file), headers: true)

# CSV Headers - Position, Name, Type, Signal, Label

$header_position = "Position"
$header_name     = "Name"
$header_type     = "Type"
$header_signal   = "Signal"
$header_label    = "Label"

# Script files

$fix_signals_file = "fix_signals.txt"
$fix_labels_file  = "fix_labels.txt"

# Fix needed



#
# MARK:- Functions
#

# Helpers

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
	puts "    MX> config load #{$ioc_file}"
	puts "    MX> script #{File.basename(file)}"
	puts "    MX> config save"
	puts "    MX> csv pinout #{$pin_csv_generated_file}"
	puts ""
end

# Checkers

def check_name_and_position
	error_name_position = false

	puts ""
	puts "Checking pins' positions & names..."

	$pin_csv_reference.each_with_index do |row, index|
		pin_number = index + 1

		# Check positions
		if row[$header_position] != $pin_csv_generated[index][$header_position]
			puts "#{pin_number} - ❌ position - reference: #{row[$header_position]} -- ioc: #{$pin_csv_generated[index][$header_position]}"
			error_name_position = true
		end

		# Check names
		if row[$header_name] != $pin_csv_generated[index][$header_name]
			puts "#{pin_number} - ❌ name - reference: #{row[$header_name]} -- ioc: #{$pin_csv_generated[index][$header_name]}"
			error_name_position = true
		end
	end

	if error_name_position
		puts "Checking pins' positions & names... ❌"
		exit
	else
		puts "Checking pins' positions & names... ✅"
	end
end

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

#
# MARK:- Compare reference file and .ioc file
#

compare_ref_and_ioc

#
# MARK:- Check pins and positions are correct
#

check_name_and_position

#
# MARK:- Check signals
#

if $check_signals || $fix_signals
	puts ""
	puts "Checking pins' signals..."

	$error_signals = false

	$pin_csv_reference.each_with_index do |row, index|
		pin_number = index + 1
		if row[$header_signal] != $pin_csv_generated[index][$header_signal] && not_power_or_reset(row[$header_signal])
			puts_error(pin_number, row[$header_name], row[$header_signal], $pin_csv_generated[index][$header_signal])
			$error_signals = true
		end
	end

	if $error_signals
		puts "Checking pins' signals... ❌"
		$fix_signals_needed = true
	else
		puts "Checking pins' signals... ✅"
	end
end

#
# MARK:- Fix signals
#

# set pin <pin name> <signal name>
# set pin PA12 UART4_TX
# def puts_error(pin_number, pin_name, expected, found)

if $fix_signals
	puts ""
	puts "Generating script to fix pins' signals..."

	if !$fix_signals_needed
		puts "No need to generate a script to fix pins' signals... 🎉"
		return
	end

	file = File.new($fix_signals_file, "w")

	$pin_csv_reference.each_with_index do |row, index|
		pin_number = index + 1
		if row[$header_signal] != $pin_csv_generated[index][$header_signal] && not_power_or_reset(row[$header_signal])
			fix = "set pin #{row[$header_name]} #{row[$header_signal]}"
			puts fix
			file.puts fix
		end
	end

	file.close

	puts_script_instructions(file)
end

#
# MARK:- Check labels
#

if $check_labels || $fix_labels
	puts ""
	puts "Checking pins' labels..."

	$error_labels = false

	$pin_csv_reference.each_with_index do |row, index|
		pin_number = index + 1
		if row[$header_label] != $pin_csv_generated[index][$header_label] && not_power_or_reset(row[$header_signal])
			puts_error(pin_number, row[$header_name], row[$header_label], $pin_csv_generated[index][$header_label])
			$error_labels = true
		end
	end

	if $error_labels
		puts "Checking pins' labels... ❌"
		$fix_labels_needed = true
	else
		puts "Checking pins' labels... ✅"
	end
end

#
# MARK:- Fix labels
#

# set gpio parameters <matching pin> <param name> <param value>
# set gpio parameters PE4 GPIO_Label MOTOR_RIGHT_DIRECTION

if $fix_labels
	puts ""
	puts "Generating script to fix pins' labels..."

	if !$fix_labels_needed
		puts "No need to generate a script to fix pins' labels... 🎉"
		return
	end

	file = File.new($fix_labels_file, "w")

	$pin_csv_reference.each_with_index do |row, index|
		pin_number = index + 1
		if row[$header_label] != $pin_csv_generated[index][$header_label] && not_power_or_reset(row[$header_signal])
			fix = "set gpio parameters #{row[$header_name]} GPIO_Label #{row[$header_label]}"
			puts fix
			file.puts fix
		end
	end

	file.close

	puts_script_instructions(file)
end

#
# MARK:- Remove scripts
#

if $rm_scripts
	puts ""
	puts "Removing script files..."
	File.delete($fix_labels_file) if File.exist?($fix_labels_file)
	File.delete($fix_signals_file) if File.exist?($fix_signals_file)
	puts "Removing script files... ✅"
end
