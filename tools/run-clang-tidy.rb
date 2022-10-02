#!/usr/bin/env ruby

# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

SIS="#include <...> search starts here:"
SIE="End of search list."

$DEBUG = false

def log_debug(message)
	if $DEBUG
		puts message
	end
end

$tmp_search_dirs = []

# Previous trials, for posterity
# search_dirs_cmds = ["arm-none-eabi-g++ -E -Wp,-v -xc++ /dev/null 2>&1", "arm-none-eabi-gcc -E -Wp,-v -xc /dev/null 2>&1"]
search_dirs_cmds = ["arm-none-eabi-g++ -E -Wp,-v -xc++ /dev/null 2>&1"]

search_dirs_cmds.each do |cmd|
	log_debug "Find search dirs for #{cmd}"
	raw_search_dirs = `#{cmd}`
	search_dirs = raw_search_dirs.match(/#{SIS}(.*?)#{SIE}/m)[1]

	log_debug "Remove starting empty space"
	search_dirs = search_dirs.gsub(/^\s*/, '')

	log_debug "Appending -extra-arg"
	search_dirs = search_dirs.gsub!(/^/, '-extra-arg=-isystem\&')

	$tmp_search_dirs += search_dirs.split("\n")
end

# At the moment this is not needed but might in the future, so we keep it here
#
# print_search_dirs_cmds = ["arm-none-eabi-gcc -print-search-dirs", "arm-none-eabi-g++ -print-search-dirs"]
# print_search_dirs_cmds.each do |cmd|
# 	log_debug "Find search dirs for #{cmd}"
# 	raw_search_dirs = `#{cmd}`

# 	log_debug "Remove prefixes"
# 	search_dirs = raw_search_dirs.gsub('install: ', '').gsub('programs: =', '').gsub('libraries: =', '')

# 	log_debug "Split lines"
# 	search_dirs = search_dirs.gsub('/:/', "/\n/")

# 	log_debug "Appending -extra-arg"
# 	search_dirs = search_dirs.gsub!(/^/, '-extra-arg=-isystem\&')

# 	$tmp_search_dirs += search_dirs.split("\n")
# end

log_debug "Remove duplicates"
$tmp_search_dirs = $tmp_search_dirs.uniq

log_debug "Find clang-tidy"
$CLANG_TIDY_EXEC = ""

if system("which clang-tidy > /dev/null")
	$CLANG_TIDY_EXEC = "clang-tidy"
elsif system("which clang-tidy-13 > /dev/null")
	$CLANG_TIDY_EXEC = "clang-tidy-13"
elsif system("which /opt/homebrew/opt/llvm/bin/clang-tidy > /dev/null")
	$CLANG_TIDY_EXEC = "/opt/homebrew/opt/llvm/bin/clang-tidy"
elsif system("which /usr/local/opt/llvm/bin/clang-tidy > /dev/null")
	$CLANG_TIDY_EXEC = "/usr/local/opt/llvm/bin/clang-tidy"
end

puts "Clang-tidy found at: #{$CLANG_TIDY_EXEC}"

puts "Standard headers search dirs:"
puts $tmp_search_dirs.map { |d| "    #{d}" }.join("\n")
puts ""

$BUILD_DIR = ARGV.shift
$SEARCH_DIRS = $tmp_search_dirs.join("\s")
$FILES = ARGV.join(" ")
$FILES_ARR = ARGV

puts "Running clang-tidy on:"
puts $FILES_ARR.map { |f| "    #{f}" }.join("\n")

RUN_CLANG_TIDY_CMD = <<EOL
#{$CLANG_TIDY_EXEC} -p=#{$BUILD_DIR} #{$SEARCH_DIRS} --quiet #{$FILES}
EOL

puts ""
puts RUN_CLANG_TIDY_CMD
puts ""

if !system(RUN_CLANG_TIDY_CMD)
	exit 1
end
