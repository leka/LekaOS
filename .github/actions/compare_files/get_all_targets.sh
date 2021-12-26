shopt -s xpg_echo

base_targets=($(echo $(find _build_tmp/$BASE_SHA -name '*.bin' -execdir basename -s '.bin' {} +) | tr ' ' '\n' | sort -du | tr '\n' ' '))
head_targets=($(echo $(find _build_tmp/$HEAD_SHA -name '*.bin' -execdir basename -s '.bin' {} +) | tr ' ' '\n' | sort -du | tr '\n' ' '))
all_targets=($(echo "${base_targets[@]} ${head_targets[@]}" | tr ' ' '\n' | sort -du | tr '\n' ' '))

echo "Set added/deleted targets"

added_targets=()
deleted_targets=()

for target in "${all_targets[@]}"; do
	if [[ ${base_targets[*]} =~ "$target" ]] && ! [[ ${head_targets[*]} =~ "$target" ]]; then
		deleted_targets+=($target)
	elif ! [[ ${base_targets[*]} =~ "$target" ]] && [[ ${head_targets[*]} =~ "$target" ]]; then
		added_targets+=($target)
	fi
done

echo "all:     ${all_targets[*]}"
echo "base:    ${base_targets[*]}"
echo "head:    ${head_targets[*]}"
echo "added:   ${added_targets[*]}"
echo "deleted: ${deleted_targets[*]}"
