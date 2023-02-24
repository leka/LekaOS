shopt -s xpg_echo

BASE_DIR=$1
HEAD_DIR=$2

source ./.github/actions/compare_base_head_changes/utils.sh
source ./.github/actions/compare_base_head_changes/get_all_targets.sh

MAP_DIFF_OUTPUT="$RUNNER_HOME/MAP_DIFF_OUTPUT.md"
touch $MAP_DIFF_OUTPUT

no_map_diff=true

for target in "${all_targets[@]}"; do
target_name=$target

	if ! [[ " ${added_targets[*]} " =~ " $target " ]] && ! [[ " ${deleted_targets[*]} " =~ " $target " ]]; then

		echo "$target not deleted nor new, running diff"

		if ! diff_map_output=$(diff --unified=150 $BASE_DIR/$target_name-map.txt $HEAD_DIR/$target_name-map.txt); then

			if ! diff_size_output=$(diff --unified=150 $BASE_DIR/$target_name-code_size.txt $HEAD_DIR/$target_name-code_size.txt); then
				diff_size_output=$(cat $HEAD_DIR/$target_name-code_size.txt)
			fi

			echo $diff_map_output
			echo $diff_size_output

			echo "<details>" >> $MAP_DIFF_OUTPUT
			echo "<summary><b><code>$target_name</code></b> (click to expand)</summary>" >> $MAP_DIFF_OUTPUT
			echo "" >> $MAP_DIFF_OUTPUT

			echo "\`\`\`diff" >> $MAP_DIFF_OUTPUT
			echo "$diff_map_output" >> $MAP_DIFF_OUTPUT
			echo "\`\`\`" >> $MAP_DIFF_OUTPUT

			echo "\`\`\`diff" >> $MAP_DIFF_OUTPUT
			echo "$diff_size_output" >> $MAP_DIFF_OUTPUT
			echo "\`\`\`" >> $MAP_DIFF_OUTPUT

			echo "" >> $MAP_DIFF_OUTPUT
			echo "</details>" >> $MAP_DIFF_OUTPUT
			echo "" >> $MAP_DIFF_OUTPUT

			no_map_diff=false
		fi

	elif [[ " ${added_targets[*]} " =~ " $target " ]]; then

		echo "$target added, showing map information"

		map_output=$(cat $HEAD_DIR/$target_name-map.txt)
		size_output=$(cat $HEAD_DIR/$target_name-code_size.txt)

		echo "<details>" >> $MAP_DIFF_OUTPUT
		echo "<summary><b><code>$target_name</code></b> (click to expand)</summary>" >> $MAP_DIFF_OUTPUT
		echo "" >> $MAP_DIFF_OUTPUT

		echo "\`\`\`" >> $MAP_DIFF_OUTPUT
		echo "$map_output" >> $MAP_DIFF_OUTPUT
		echo "\`\`\`" >> $MAP_DIFF_OUTPUT

		echo "\`\`\`" >> $MAP_DIFF_OUTPUT
		echo "$size_output" >> $MAP_DIFF_OUTPUT
		echo "\`\`\`" >> $MAP_DIFF_OUTPUT

		echo "" >> $MAP_DIFF_OUTPUT
		echo "</details>" >> $MAP_DIFF_OUTPUT
		echo "" >> $MAP_DIFF_OUTPUT

		no_map_diff=false

	fi

done

if $no_map_diff; then
	echo "No differenes where found in map files." >> $MAP_DIFF_OUTPUT
fi
