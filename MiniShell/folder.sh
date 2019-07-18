#!/usr/bin/env bash
# Bem importante

COMMAND="$1"
FOLDER="$2"

if [[ "$COMMAND" == "create" ]];
then
    mkdir $FOLDER
    touch ${FOLDER}/index.html
    ls $FOLDER
elif [[ "$COMMAND" == "remove" ]];
then
	rm -r $FOLDER
else
    echo "Missing argument!"
fi