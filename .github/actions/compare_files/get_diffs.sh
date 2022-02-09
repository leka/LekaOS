shopt -s xpg_echo

BASE_DIR=$1
HEAD_DIR=$2

source ./.github/actions/compare_files/utils.sh
source ./.github/actions/compare_files/get_all_targets.sh

no_map_diff=true

echo 'MAP_DIFF_OUTPUT<<EOF_MAP_DIFF_OUTPUT' >> $GITHUB_ENV

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

			echo "<details>" >> $GITHUB_ENV
			echo "<summary><b><code>$target_name</code></b> (click to expand)</summary>" >> $GITHUB_ENV
			echo "" >> $GITHUB_ENV

			echo "\`\`\`diff" >> $GITHUB_ENV
			echo "$diff_map_output" >> $GITHUB_ENV
			echo "\`\`\`" >> $GITHUB_ENV

			echo "\`\`\`diff" >> $GITHUB_ENV
			echo "$diff_size_output" >> $GITHUB_ENV
			echo "\`\`\`" >> $GITHUB_ENV

			echo "" >> $GITHUB_ENV
			echo "</details>" >> $GITHUB_ENV
			echo "" >> $GITHUB_ENV

			no_map_diff=false
		fi

	elif [[ " ${added_targets[*]} " =~ " $target " ]]; then

		echo "$target added, showing map information"

		map_output=$(cat $HEAD_DIR/$target_name-map.txt)
		size_output=$(cat $HEAD_DIR/$target_name-code_size.txt)

		echo "<details>" >> $GITHUB_ENV
		echo "<summary><b><code>$target_name</code></b> (click to expand)</summary>" >> $GITHUB_ENV
		echo "" >> $GITHUB_ENV

		echo "\`\`\`" >> $GITHUB_ENV
		echo "$map_output" >> $GITHUB_ENV
		echo "\`\`\`" >> $GITHUB_ENV

		echo "\`\`\`" >> $GITHUB_ENV
		echo "$size_output" >> $GITHUB_ENV
		echo "\`\`\`" >> $GITHUB_ENV

		echo "" >> $GITHUB_ENV
		echo "</details>" >> $GITHUB_ENV
		echo "" >> $GITHUB_ENV

		no_map_diff=false

	fi

done

if $no_map_diff; then
	echo "No differenes where found in map files." >> $GITHUB_ENV
fi

echo 'EOF_MAP_DIFF_OUTPUT' >> $GITHUB_ENV
