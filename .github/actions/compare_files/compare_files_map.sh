shopt -s xpg_echo
source ./.github/actions/compare_files/get_all_targets.sh

no_map_diff=true

echo 'MAP_DIFF_OUTPUT<<EOF_MAP_DIFF_OUTPUT' >> $GITHUB_ENV

for target in "${all_targets[@]}"; do
target_name=$target

	if ! [[ " ${added_targets[*]} " =~ " $target " ]] && ! [[ " ${deleted_targets[*]} " =~ " $target " ]]; then
		echo "$target not deleted nor new, running diff"

		if ! diff_output=$(diff --unified=150 _build_tmp/$BASE_SHA/$target_name.txt _build_tmp/$HEAD_SHA/$target_name.txt); then
			echo $diff_output

			echo "<details>" >> $GITHUB_ENV
			echo "<summary><b><code>$target_name</code></b> (click to expand)</summary>" >> $GITHUB_ENV
			echo "" >> $GITHUB_ENV

			echo "\`\`\`diff" >> $GITHUB_ENV
			echo "$diff_output" >> $GITHUB_ENV
			echo "\`\`\`" >> $GITHUB_ENV

			echo "" >> $GITHUB_ENV
			echo "</details>" >> $GITHUB_ENV
			echo "" >> $GITHUB_ENV

			no_map_diff=false
		fi
	elif [[ " ${added_targets[*]} " =~ " $target " ]]; then
		echo "$target added, showing map information"
		map_output=$(cat _build_tmp/$HEAD_SHA/$target_name.txt)

		echo "<details>" >> $GITHUB_ENV
		echo "<summary><b><code>$target_name</code></b> (click to expand)</summary>" >> $GITHUB_ENV
		echo "" >> $GITHUB_ENV

		echo "\`\`\`" >> $GITHUB_ENV
		echo "$map_output" >> $GITHUB_ENV
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
