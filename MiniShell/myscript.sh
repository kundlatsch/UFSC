#!/usr/bin/env bash

EXEC_NAME=$(basename $0)
VERSION="v0.2"
USAGE="Usage: 
       $0 <mode> [...NAMES]
       $0 <flags>
Modes:
        hello    Say hello
        Bye      Say bye
Flags:
       --help      Show program usage
       --version   Show program version"

#Wildcard
OPERATIONS=":greet:help:"

greet() {
	for name in "$@"
	do
		if [ "$name" == "" ]
		then
			echo "${TEXT}, anonymous!"
		else
			echo "${TEXT}, ${name}zinho"
		fi
	done
}

show-version() {
	echo "$VERSION"
}

help() {
	echo "$USAGE"
}

die() {
	echo "$1"
	exit 1
}

OPERATION="$1"
TEXT=""
shift

case "$OPERATION" in
	--help)
		help
		exit 0
		;;
	--version)
		show-version
		exit 0
esac

if [ $OPERATION == "hello" ]
then
	TEXT="Hello"
	greet $*
elif [ $OPERATION == "bye" ]
then
	TEXT="Bye"
	greet $*
else
	die "Invalid Operation ${OPERATION}"
fi

