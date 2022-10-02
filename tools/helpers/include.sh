#!/usr/bin/env zsh

#
# Helpers
#

function array_is_empty {
	arr=($@)
	if [ ${#arr[@]} -eq 0 ] ; then
		return 0
	else
		return 1
	fi
}

function print_section {
	echo "\n"
	echo "👷 $@ 🚧"
}

function print_action {
	echo ""
	echo "› $@"
}

function fake_try {
	echo "Running $@ ... ✅ (-)"
}

function is_ci {
	if [[ -n $CI ]]; then
		return 0
	else
		return 1
	fi
}

function is_dry_run {
	if [[ $ARG_ARRAY =~ "--dry-run" || -n $DRY_RUN ]]; then
		return 0
	else
		return 1
	fi
}

function args_contain {
	if [[ $ARG_ARRAY =~ $@ ]]; then
		return 0
	else
		return 1
	fi
}

function ask_for_sudo {
	echo "Please enter your password."
	sudo -v
	if [ ! $? -eq 0 ]; then
		echo ""
		echo "Goodbye, come again!..."
		exit 0
	fi
}

function list_failed_commands {
	ret=0
	echo ""
	if array_is_empty $FAILED_COMMANDS && array_is_empty $CAN_FAIL_COMMANDS ; then
		echo "🎉 All the commands were run successfully 💪"
	else
		if ! array_is_empty $CAN_FAIL_COMMANDS ; then
			echo "⚠️ The following commands have failed but it's okay: ⚠️"
			for cmd in $CAN_FAIL_COMMANDS; do
				echo "\t- $cmd"
			done
		fi

		if ! array_is_empty $FAILED_COMMANDS ; then
			echo "💥 The following commands have failed: 💥"
			for cmd in $FAILED_COMMANDS; do
				echo "\t- $cmd"
			done
			ret=1
		fi
	fi

	exit $ret
}
