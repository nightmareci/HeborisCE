#!/bin/sh

filename="gamecontrollerdb.txt"
if [ -z "$1" ] ; then
	path=""
else
	path="$1/"
fi
curl -L -o "$(echo "$path")gamecontrollerdb.txt" 'https://raw.githubusercontent.com/gabomdq/SDL_GameControllerDB/master/gamecontrollerdb.txt'