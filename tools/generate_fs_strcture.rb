#!/usr/bin/env ruby

# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

require 'pathname'

$ROOT_PATH = Pathname.new('fs')
$OUTPUT_PATH = 'include/fs_structure.hpp'
$DIR_LEVEL = 0

$output = File.new($OUTPUT_PATH, "w")

def level
	return " " * $DIR_LEVEL * 2
end

def uninteresting?(file)
	name = "#{file.basename}"

	if name[0] == '.' or name == "README.md"
		return true
	else
		return false
	end
end

def make_var_name(name)
	raw = "#{name.sub_ext('').basename}"
	var = raw.gsub(/[\-]/, '_')
	return var
end

def write(str)
	puts str
	$output.puts str
end

def is_id_dir?(path)
	parent = "#{path.parent.basename}"
	return parent == "id"
end

def recurse(dir)

	dir.each_child do |child|

		next if "#{child.basename}"[0] == '.'

		if child.directory? and not child.empty?

			next if child.children.length() == 1 and uninteresting?(child.children[0])

			write "#{level}namespace #{child.basename} {\n\n"

			$DIR_LEVEL += 1
			recurse(child)
			$DIR_LEVEL -= 1

			write "\n#{level}} // namespace #{child}\n\n"

		else

			if is_id_dir?(child)
				write "#{level}inline constexpr auto _#{make_var_name(child)} = std::string_view {\"/#{child}\"};" unless uninteresting?(child)
			else
				write "#{level}inline constexpr auto #{make_var_name(child)} = std::string_view {\"/#{child}\"};" unless uninteresting?(child)
			end

		end

	end

end


write <<~EOS
// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <filesystem>

// NOLINTBEGIN(modernize-concat-nested-namespaces)

namespace leka::fs {

EOS

recurse($ROOT_PATH)

write <<~EOS

} // namespace leka::fs

// NOLINTEND(modernize-concat-nested-namespaces)

EOS

$output.close

system("clang-format -i #{$OUTPUT_PATH}")
