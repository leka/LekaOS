#!/usr/bin/env zsh

trap "exit 1" INT

typeset -x  DOTFILES_DIR=$(pwd)
typeset -Ux FAILED_COMMANDS=()
typeset -Ux CAN_FAIL_COMMANDS=()
typeset -x  ARG_ARRAY=()

# create tmp file & schedule delete if error
typeset -x TEMP_FILE=$(mktemp)
trap "rm -f $TEMP_FILE" 0 2 3 15

THIS_DIR=$(dirname $0)
source $THIS_DIR/include.sh

verbose=true
super_verbose=false
can_fail=false

# set output level
if [[ $1 =~ "-v" || $1 =~ "--verbose" ]]; then
	verbose=true
	if [[ $1 =~ "-vv" ]]; then
		verbose=true
		super_verbose=true
	fi
	shift
fi

# set set can
if [[ $1 =~ "-x" ]]; then
	can_fail=true
	shift
fi

# if [[ ! -n $DRY_RUN ]]; then
# 	if [[ $1 =~ "sudo" ]]; then
# 		shift
# 		sudo script -q $TEMP_FILE $@ > /dev/null 2>&1
# 	else
# 		script -q $TEMP_FILE $@ > /dev/null 2>&1
# 	fi
# 	cmd_result=$?
# fi

echo -ne "Running $@ ... "

if is_dry_run ; then
	echo "✅ (dry run)"
else
	cmd_result=0
	start=$(date +%s.%N)

	# run command
	if [[ $1 =~ "sudo" ]]; then
		shift
		sudo script -q $TEMP_FILE $@ > /dev/null 2>&1
	else
		script -q $TEMP_FILE $@ > /dev/null 2>&1
	fi

	cmd_result=$?
	end=$(date +%s.%N)

	# calculate duration
	duration="$(printf %.2f $(echo "$end-$start" | bc -l))"

	# output result
	if [ $cmd_result -eq 0 ]; then
		echo "✅ ($duration)"
		if [[ $super_verbose == true ]]; then
			cat $TEMP_FILE
		fi
	else
		echo "❌ ($duration)"
		if [[ $can_fail == true ]]; then
			CAN_FAIL_COMMANDS+=$@
		else
			FAILED_COMMANDS+=$@
		fi
		if [[ $verbose == true ]]; then
			cat $TEMP_FILE
		fi
	fi
fi
