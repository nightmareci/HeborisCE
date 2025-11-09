#!/bin/sh

if [ -z "$1" -o -z "$2" ] ; then
	echo 'Usage: pkg.sh [source-directory] [build-directory]'
	exit 1
fi

if [ -z "$VITASDK" ] ; then
	echo 'The environment variable VITASDK must be set to the root of the Vita SDK installation.'
	exit 1
fi

mkdir "$2"

SOURCE_DIRECTORY=`readlink -f "$1"`
BUILD_DIRECTORY=`readlink -f "$2"`

cd "$SOURCE_DIRECTORY"
cmake -B "$BUILD_DIRECTORY" -G Ninja -DCMAKE_BUILD_TYPE=Release --toolchain "$VITASDK/share/vita.toolchain.cmake" || exit 1
cmake --build "$BUILD_DIRECTORY" || exit 1
