shopt -s xpg_echo
source ./.github/actions/compare_files/get_all_targets.sh

echo 'STATUS_DIFF_OUTPUT<<EOF_STATUS_DIFF_OUTPUT' >> $GITHUB_ENV

echo "| Target | Status | .bin | .map | Total Flash (base/head) | Total Flash Δ | Static RAM (base/head) | Static RAM Δ |" >> $GITHUB_ENV
echo "|-------|:------:|:------:|:------:|:------:|:------:|:------:|:------:|" >> $GITHUB_ENV

for target in "${all_targets[@]}"; do
target_name=$target

echo -n "| $target_name " >> $GITHUB_ENV

if [[ " ${added_targets[*]} " =~ " $target " ]]; then
	echo -n "| :sparkles: | - | - " >> $GITHUB_ENV

	python3 extern/mbed-os/tools/memap.py -t GCC_ARM _build_tmp/$HEAD_SHA/$target_name.map > _build_tmp/$HEAD_SHA/$target_name.txt
	head_ram=$(grep -Po '(?<=\(data \+ bss\):\s)[[:digit:]]*' _build_tmp/$HEAD_SHA/$target_name.txt)
	head_flash=$(grep -Po '(?<=\(text \+ data\):\s)[[:digit:]]*' _build_tmp/$HEAD_SHA/$target_name.txt)

	echo -n "| $head_ram | - | $head_flash | - |\n" >> $GITHUB_ENV
elif [[ " ${deleted_targets[*]} " =~ " $target " ]]; then
	echo -n "| :coffin: | - | - | - | - | - | - |\n" >> $GITHUB_ENV
else
	echo -n "| :heavy_check_mark: " >> $GITHUB_ENV

	if ! output=$(diff _build_tmp/$BASE_SHA/$target_name.bin _build_tmp/$HEAD_SHA/$target_name.bin 2>/dev/null); then
		echo -n "| :x: " >> $GITHUB_ENV
	else
		echo -n "| :white_check_mark: " >> $GITHUB_ENV
	fi

	python3 extern/mbed-os/tools/memap.py -t GCC_ARM _build_tmp/$BASE_SHA/$target_name.map > _build_tmp/$BASE_SHA/$target_name.txt
	python3 extern/mbed-os/tools/memap.py -t GCC_ARM _build_tmp/$HEAD_SHA/$target_name.map > _build_tmp/$HEAD_SHA/$target_name.txt

	if ! output=$(diff _build_tmp/$BASE_SHA/$target_name.txt _build_tmp/$HEAD_SHA/$target_name.txt 2>/dev/null); then
		echo -n "| :x: " >> $GITHUB_ENV
	else
		echo -n "| :white_check_mark: " >> $GITHUB_ENV
	fi

	base_flash=$(grep -Po '(?<=\(text \+ data\):\s)[[:digit:]]*' _build_tmp/$BASE_SHA/$target_name.txt)
	head_flash=$(grep -Po '(?<=\(text \+ data\):\s)[[:digit:]]*' _build_tmp/$HEAD_SHA/$target_name.txt)
	diff_flash=$(($head_flash - $base_flash))

	if [ $diff_flash -lt 0 ]; then
		diff_flash=":chart_with_downwards_trend:  $diff_flash"
	elif [ $diff_flash -gt 0 ]; then
		diff_flash=":chart_with_upwards_trend: $diff_flash"
	else
		diff_flash="ø"
	fi

	base_ram=$(grep -Po '(?<=\(data \+ bss\):\s)[[:digit:]]*' _build_tmp/$BASE_SHA/$target_name.txt)
	head_ram=$(grep -Po '(?<=\(data \+ bss\):\s)[[:digit:]]*' _build_tmp/$HEAD_SHA/$target_name.txt)
	diff_ram=$(($head_ram - $base_ram))

	if [ $diff_ram -lt 0 ]; then
		diff_ram=":chart_with_downwards_trend: $diff_ram"
	elif [ $diff_ram -gt 0 ]; then
		diff_ram=":chart_with_upwards_trend: $diff_ram"
	else
		diff_ram="ø"
	fi

	echo -n "| $base_flash / $head_flash | $diff_flash | $base_ram / $head_ram | $diff_ram " >> $GITHUB_ENV

	echo -n "|\n" >> $GITHUB_ENV
fi

done

echo 'EOF_STATUS_DIFF_OUTPUT' >> $GITHUB_ENV
