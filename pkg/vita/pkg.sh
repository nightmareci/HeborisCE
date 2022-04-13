#!/bin/sh

if [ -z "$1" -o -z "$2" ] ; then
	echo 'Usage: pkg.sh [source-directory] [build-directory]'
	exit 1
fi

SOURCE_DIRECTORY="$1"
BUILD_DIRECTORY="$2"

rm -rf "$BUILD_DIRECTORY" || exit 1
mkdir "$BUILD_DIRECTORY" || exit 1

cmake "$SOURCE_DIRECTORY" -B "$BUILD_DIRECTORY" -G Ninja -DCMAKE_BUILD_TYPE=Release -DBUILD_TARGET=Vita --toolchain "$VITASDK/share/vita.toolchain.cmake" || exit 1
cmake --build "${BUILD_DIRECTORY}" || exit 1