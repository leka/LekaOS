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
end

arguments_available = ["--check-signals", "--check-labels", "--fix-signals", "--fix-labels"]
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

#
# MARK:- Variables
#

# CSV Files

$table_file_name = "Leka-MCU_Pins"
$table_version   = "1.0.0"
$table_reference = CSV.parse(File.read("#{$table_file_name}-v#{$table_version}-reference.csv"), headers: true)
$table_generated = CSV.parse(File.read("#{$table_file_name}-v#{$table_version}-generated.csv"), headers: true)

# CSV Headers - Position, Name, Type, Signal, Label

$header_position = "Position"
$header_name     = "Name"
$header_type     = "Type"
$header_signal   = "Signal"
$header_label    = "Label"

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

# Checkers

def check_name_and_position
	error_name_position = false

	puts ""
	puts "Checking pins' positions & names..."

	$table_reference.each_with_index do |row, index|
		pin_number = index + 1

		# Check positions
		if row[$header_position] != $table_generated[index][$header_position]
			puts "#{pin_number} - ❌ position - reference: #{row[$header_position]} -- ioc: #{$table_generated[index][$header_position]}"
			error_name_position = true
		end

		# Check names
		if row[$header_name] != $table_generated[index][$header_name]
			puts "#{pin_number} - ❌ name - reference: #{row[$header_name]} -- ioc: #{$table_generated[index][$header_name]}"
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
	puts "   ref: #{$table_reference.length}"
	puts "   ioc: #{$table_generated.length}"
	if $table_reference.length == $table_generated.length
		puts "Checking reference file and .ioc file have the same number of pins... ✅"
	else
		puts "Checking reference file and .ioc file have the same number of pins... ❌"
		exit
	end
end

# Edit .ioc

# set pin <pin name> <signal name>
# set pin PA12 UART4_TX

# set gpio parameters <matching pin> <param name> <param value>
# set gpio parameters PE4 GPIO_Label MOTOR_RIGHT_DIRECTION

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

	$table_reference.each_with_index do |row, index|
		pin_number = index + 1
		if row[$header_signal] != $table_generated[index][$header_signal] && not_power_or_reset(row[$header_signal])
			puts_error(pin_number, row[$header_name], row[$header_signal], $table_generated[index][$header_signal])
			$error_signals = true
		end
	end

	if $error_signals
		puts "Checking pins' signals... ❌"
	else
		puts "Checking pins' signals... ✅"
	end
end

#
# MARK:- Check labels
#

if $check_labels || $fix_labels
	puts ""
	puts "Checking pins' labels..."

	$error_labels = false

	$table_reference.each_with_index do |row, index|
		pin_number = index + 1
		if row[$header_label] != $table_generated[index][$header_label] && not_power_or_reset(row[$header_signal])
			puts_error(pin_number, row[$header_name], row[$header_label], $table_generated[index][$header_label])
			$error_labels = true
		end
	end

	if $error_labels
		puts "Checking pins' labels... ❌"
		exit
	else
		puts "Checking pins' labels... ✅"
	end
end
