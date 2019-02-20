#!/bin/bash
#
#   validation.sh - run a set of validation test scripts
#
#   Execute executable provided on the command line or located in the
#   validation test directory in a (relatively) safe environment, collect the
#   log and the result and indicate if the tests are passing or not.
#

# Name of the script without the path.
#
SCRIPT_NAME="${0##*/}"

# Current working directory at the script start.
# Useful later when child processes change their cwd.
#
SCRIPT_PWD="$PWD"

# Root directory of test cases.
# This script is expected to be under this directory.
#
TEST_ROOT="${0%/*}"

# Root directory for validation test cases.
# This is where the script looks for if no script is provided on command line.
#
VALIDATION_ROOT="${TEST_ROOT}/validation"

# What to add to the PATH of executed test scripts.
# Each component (separated by ':') is made absolute before to be added to the
# PATH variable.
#
VALIDATION_PATH="${TEST_ROOT}/../bin":"${TEST_ROOT}/tools"


# What the user indicates for the --color option on the command line.
#
OPTION_COLOR=

# What the user indicates for the --quiet option on the command line.
#
OPTION_QUIET=

# What the user indicates for the --silent option on the command line.
#
OPTION_SILENT=

# What the user indicates for the --timeout option on the command line.
#
OPTION_TIMEOUT=

# What the user indicates for the --executable option on the command line.
#
OPTION_EXECUTABLE=


# A list of commands to execute in reverse order when this script exits.
# An empty line means the next non-empty, non-skipped line must be skipped.
# See push_atexit() and pop_atexit().
#
ATEXIT_SCRIPT=

# The total number of test to do.
#
TEST_DONE=

# The number of failed tests.
#
TEST_FAILED=

# The name of the failed tests.
# The name of a test is either the file name (without the path) of the script
# if taken automatically from VALIDATION_ROOT, or what has been passed on the
# command line.
#
TEST_FAILED_NAMES=()

# Use colored output.
# 0 = yes / 1 = no
#
COLOR=1

# Timeout to use for test cases.
# 0 = no timeout
#
TIMEOUT=0


# Print a message on the standard error and exit with failure.
# Arg0: the message to print.
#
fatal() {
    local msg="$1" ; shift

    exec >&2
    printf "%s: %s\n" "${SCRIPT_NAME}" "$msg"
    printf "Please type '%s --help' for more information\n" "$0"

    exit 1
}

# Indicate if to use colored output.
# Return: 0 = yes / 1 = no
#
use_color() {
    return $COLOR
}

# Setup the colored output, given the --color option value and the default
# choice.
#
setup_color() {
    case "x${OPTION_COLOR}" in
	'x'|'xauto') test -t 1; COLOR=$? ;;
	'xyes')      COLOR=0 ;;
	'xno')       COLOR=1 ;;
    esac
}


# Setup the timeout for test cases, given the --timeout option and the default
# value.
#
setup_timeout() {
    if [ "x${OPTION_TIMEOUT}" != 'x' ] ; then
	TIMEOUT=${OPTION_TIMEOUT}
    fi
}

# Indicate if a timeout must be used for test cases.
# Return: 0 = yes / 1 = no
#
has_timeout() {
    test $TIMEOUT -gt 0
}


# Indicate if the script is in quiet mode.
# Return: 0 = yes / 1 = no
#
is_quiet() {
    test "x${OPTION_QUIET}" != 'x'
}

# Indicate if the script is in silent mode.
# Return: 0 = yes / 1 = no
#
is_silent() {
    test "x${OPTION_SILENT}" != 'x'
}


# Print the name of the test case currently executed.
# Arg0: name of the test
#
print_test() {
    local name="$1" ; shift

    if is_quiet ; then
	return 0
    elif is_silent ; then
	return 0
    fi

    if use_color ; then
	printf "\033[34;1m==>\033[0;1m %s : \033[0m" "$name"
    else
	printf "==> %s : " "$name"
    fi
}

# Print the name and the result of the test case executed.
# Arg0: name of the test
# Arg1: exit value of the test
#
print_test_result() {
    local name="$1" ; shift
    local ret=$1 ; shift

    if is_silent ; then
	return 0
    fi

    if use_color ; then
	if [ $ret -eq 0 ] ; then
	    if is_quiet ; then
		return 0
	    fi
	    printf "\r\033[32;1m==>\033[0;1m %s : \033[32;1msuccess\033[0m\n" "$name"
	else
	    printf "\r\033[31;1m==>\033[0;1m %s : \033[31;1mfailure\033[0m\n" "$name"
	fi
    else
	if [ $ret -eq 0 ] ; then
	    if is_quiet ; then
		return 0
	    fi
	    echo 'success'
	else
	    if is_quiet ; then
		printf "==> %s : failure\n" "$name"
	    else
		echo 'failure'
	    fi
	fi
    fi
}

# Print the log of the test case executed.
# Arg0: path of the test log
#
print_test_log() {
    local log="$1" ; shift

    if is_silent ; then
	return 0
    fi

    if use_color ; then
	perl -wple 's/^/  \033[35m>\033[0m /' "$log"
    else
	perl -wple 's/^/  > /' "$log"
    fi
}

# Print the summary of the test cases, with the number of failed tests.
# Return: 0 = no failed tests / 1 = at least one failed test.
#
print_test_summary() {
    local name

    if [ ${TEST_FAILED} -eq 0 ] ; then
	if is_quiet ; then
	    return 0
	elif is_silent ; then
	    return 0
	fi

	echo
	if use_color ; then
	    printf "\033[32;1m::\033[0;1m All tests succeed\033[0m\n"
	else
	    printf ":: All tests succeed\n"
	fi

	return 0
    else
	if is_quiet ; then
	    return 1
	elif is_silent ; then
	    return 1
	fi

	echo
	if use_color ; then
	    printf "\033[31;1m::\033[0;1m Test failed : %d / %d\033[0m\n" ${TEST_FAILED} ${TEST_DONE}
	    for name in "${TEST_FAILED_NAMES[@]}" ; do
		printf "  \033[31;1m->\033[0m %s\n" "$name"
	    done
	else
	    printf ":: Test failed : %d / %d\n" ${TEST_FAILED} ${TEST_DONE}
	    for name in "${TEST_FAILED_NAMES[@]}" ; do
		printf "  -> %s\n" "$name"
	    done
	fi

	return 1
    fi
}


# Print the absolute version of the given path.
# The printed path is absolute but not canonical.
# Do nothing if the path is already absolute.
# Arg0: path to make absolute
#
absolute_path() {
    local path="$1" ; shift

    if [ "x$path" = 'x' -o "${path:0:1}" != '/' ] ; then
	path="$SCRIPT_PWD/$path"
    fi

    echo "$path"
}

# Print the name of the validation test scripts under the VALIDATION_ROOT.
# Print one name per line.
#
list_default_validation_cases() {
    local case

    ls -1 "${VALIDATION_ROOT}" | while read case ; do
	if [ ! -f "${VALIDATION_ROOT}/$case" ] ; then
	    continue
	elif [ ! -r "${VALIDATION_ROOT}/$case" ] ; then
	    continue
	elif [ ! -x "${VALIDATION_ROOT}/$case" ] ; then
	    continue
	fi

	echo "$case"
    done
}


# Called when the script exits to execute the pushed cleanng actions.
#
clean_atexit() {
    local skip i

    tac "${ATEXIT_SCRIPT}" | while read line ; do
	skip=0

	while [ "x$line" = 'x' ] ; do
	    read line
	    skip=$(( skip + 1 ))
	done

	while [ $skip -gt 0 ] ; do
	    read line
	    skip=$(( skip - 1 ))
	done

	echo "$line"
    done | bash

    rm "${ATEXIT_SCRIPT}"
}

# Setup the cleaning system.
#
setup_atexit() {
    ATEXIT_SCRIPT="$(mktemp -t --suffix='.sh' "${SCRIPT_NAME}.atexit.XXXXXX")"
    trap 'clean_atexit' 'EXIT'
}

# Push one bash statement to be executed at exit.
# At exit, the statements are executed in a LIFO fashion.
# Arg0: bash one-line statement to execute
#
push_atexit() {
    local stmt="$1" ; shift

    if [ "x$stmt" = 'x' ] ; then
	stmt='true'
    fi

    echo "$stmt" >> "${ATEXIT_SCRIPT}"
}

# Pop the last pushed cleaning statement out of the stack.
# The popped statement is executed.
#
pop_atexit() { 
    local skip i

    tac "${ATEXIT_SCRIPT}" | while read line ; do
	skip=0

	while [ "x$line" = 'x' ] ; do
	    read line
	    skip=$(( skip + 1 ))
	done

	while [ $skip -gt 0 ] ; do
	    read line
	    skip=$(( skip - 1 ))
	done

	echo "$line"
	break
    done | bash

    echo "" >> "${ATEXIT_SCRIPT}"
}


# Create a temporary directory for a test case to execute.
# Print the path of this temporary directory.
#
create_case_sandbox() {
    local path=$(mktemp -d --suffix='.d' "${SCRIPT_NAME}.sandbox.XXXXXX")

    push_atexit "rm -rf '$path'"
    push_atexit "chmod -R 777 '$path'"

    echo "$path"
}

# Delete the given temporary test sandbox.
# Arg0: the path of the sandbox, as given by create_case_sandbox().
#
delete_case_sandbox() {
    local path="$1" ; shift

    pop_atexit
    pop_atexit
}


setup_executable() {
    local epath

    if [ "x${OPTION_EXECUTABLE}" != 'x' ] ; then
	epath=$(mktemp -d --suffix='.d' "${SCRIPT_NAME}.epath.XXXXXX")

	push_atexit "rm -rf '$epath'"

	ln -s "$(absolute_path "${OPTION_EXECUTABLE}")" "$epath/synctl"

        VALIDATION_PATH="$epath:${VALIDATION_PATH}"
    fi
}

# Setup the environment variables of the current process so it can execute a
# test script.
#
setup_test_environment() {
    local comp apath

    while [ "x${VALIDATION_PATH}" != 'x' ] ; do
	comp=$(echo "${VALIDATION_PATH}" | cut -d':' -f1)
	VALIDATION_PATH="${VALIDATION_PATH#$comp}"
	VALIDATION_PATH="${VALIDATION_PATH#:}"

	if [ "x$apath" != 'x' ] ; then
	    apath="$apath:"
	fi

	comp="$(absolute_path "$comp")"
	apath="${apath}${comp}"
    done

    PATH="$apath:$PATH"
}


# Execute a validation test case in a (relatively) secure environment with
# possibility of a timeout and account for the test results.
# Arg0: the path prefix to add to the test name to form the test path.
# Arg1: the test name
#
execute_validation_case() {
    local prefix="$1" ; shift
    local name="$1" ; shift
    local path=$(absolute_path "${prefix}${name}")
    local sandbox=$(create_case_sandbox)
    local log=$(mktemp --suffix='.log' "${SCRIPT_NAME}.XXXXXX")
    local vcpid skpid vcret

    push_atexit "rm '$log'"

    print_test "$name"

    set -m
    (
	cd "$sandbox"
	setup_test_environment
	exec "$path"
    ) > "$log" 2>&1 &
    vcpid=$!
    set +m

    if has_timeout ; then
	set -m
	(
	    sleep $timeout
	    kill -TERM -$vcpid
	    sleep $TIMEOUT
	    kill -KILL -$vcpid
	) &
	skpid=$!
	set +m
    fi

    wait $vcpid
    vcret=$?

    if has_timeout ; then
	kill -TERM -$skpid
	wait $skpid
    fi

    print_test_result "$name" $vcret
    if [ $vcret -ne 0 ] ; then
	print_test_log "$log"
	TEST_FAILED=$(( TEST_FAILED + 1 ))
	TEST_FAILED_NAMES+=("$name")
    fi

    pop_atexit

    delete_case_sandbox "$sandbox"
}

# Print usage information on stdout.
#
usage() {
    cat <<EOF
Usage: ${SCRIPT_NAME} [<options...>] [<scripts...>]

Execute validation test cases and report test logs and results.
If no <script> is provided, execute all the executable files in
'${VALIDATION_ROOT}'.
If at least one <script> is provided, execute only these scripts.

Options:

  -c, --color [yes|no|auto]   Enable or disable the colorized output.

  -h, --help                  Print this message and exit.

  -q, --quiet                 Print only failure reports.

  -s, --silent                Do not print anything except for errors.

  -t, --timeout <sec>         Set a timeout for each validation test case.

  -x, --executable <path>     Alias synctl as <path>. 

EOF
}

# Set the --color option value.
# If called more than once, then exit with fatal message
# Arg0: the option value
#
set_color() {
    local arg="$1" ; shift

    if [ "x$arg" = 'x' ] ; then
	fatal "invalid empty value for color option"
    elif [ "$arg" != 'yes' -a "$arg" != 'no' -a "$arg" != 'auto' ] ; then
	fatal "invalid value for color option: '$arg'"
    elif [ "x${OPTION_COLOR}" != 'x' ] ; then
	fatal "option color set twice: '${OPTION_COLOR}' , '$arg'"
    fi

    OPTION_COLOR="$arg"
}

# Set the --quiet option.
# If called more than once, then exit with fatal message
#
set_quiet() {
    if [ "x${OPTION_QUIET}" != 'x' ] ; then
	fatal "option quiet set twice"
    fi

    OPTION_QUIET=1
}

# Set the --silent option.
# If called more than once, then exit with fatal message
#
set_silent() {
    if [ "x${OPTION_SILENT}" != 'x' ] ; then
	fatal "option silent set twice"
    fi

    OPTION_SILENT=1
}

# Set the --timeout option value.
# If called more than once, then exit with fatal message
# Arg0: the option value
#
set_timeout() {
    local arg="$1" ; shift

    if [ "x$arg" = 'x' ] ; then
	fatal "invalid empty value for timeout option"
    elif ! echo "$arg" | grep -Pq '^\d+$' ; then
	fatal "invalid value for timeout option: '$arg'"
    elif [ "x${OPTION_TIMEOUT}" != 'x' ] ; then
	fatal "option timeout set twice: '${OPTION_TIMEOUT}' , '$arg'"
    fi

    OPTION_TIMEOUT=$arg
}

set_executable() {
    local arg="$1" ; shift

    if [ "x$arg" = 'x' ] ; then
	fatal "invalid empty value for executable option"
    elif [ ! -f "$arg" -o ! -r "$arg" -o ! -x "$arg" ] ; then
	fatal "invalid value for executable option: '$arg'"
    elif [ "x${OPTION_EXECUTABLE}" != 'x' ] ; then
	fatal "option executable set twice: '${OPTION_EXECUTABLE}' , '$arg'"
    fi

    OPTION_EXECUTABLE="$arg"
}

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#                           Main script starts here
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

# Parse command line arguments.
# To keep it simple, only accept non bundled short options and separated option
# value.
#
while [ $# -gt 0 ] ; do
    case "$1" in
	'-c'|'--color')       shift; set_color "$1" ;;
	'-h'|'--help')        usage; exit 0 ;;
	'-q'|'--quiet')       set_quiet ;;
	'-s'|'--silent')      set_silent ;;
	'-t'|'--timeout')     shift; set_timeout "$1" ;;
	'-x'|'--executable')  shift; set_executable "$1" ;;
	'--')                 shift; break ;;
	*)                    break ;;
    esac
    shift
done

# Make all the necessary setup before to start tests.
#
setup_atexit
setup_color
setup_timeout
setup_executable

TEST_FAILED=0
TEST_FAILED_NAMES=()

if [ $# -eq 0 ] ; then
    # No test path has been specified on the command line.
    # Take the test cases from the VALIDATION_ROOT.
    #
    TEST_DONE=$(list_default_validation_cases | wc -l)
    for case in $(list_default_validation_cases) ; do
	    execute_validation_case "${VALIDATION_ROOT}/" "$case"
    done
else
    # Some test cases has been specified on the command line.
    # Execute them without asking questions.
    #
    TEST_DONE=$#
    for case in "$@" ; do
	    execute_validation_case "" "$case"
    done
fi

# Print the test summary.
# The print_test_summary() function exits with 0 if no test failed and 1
# otherwise, so we can simply call it at the very end of the script to have
# this return value as the exit value.
print_test_summary
