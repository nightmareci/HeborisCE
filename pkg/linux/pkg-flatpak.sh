#!/bin/sh

if [ -z "$1" -o -z "$2" ] ; then
	echo 'Usage: pkg-flatpak.sh [source-directory] [build-directory]'
	exit 1
fi

SOURCE_DIRECTORY=`readlink -f "$1"`
BUILD_DIRECTORY=`readlink -f "$2"`

NAME=HeborisCE

rm -rf "$BUILD_DIRECTORY" || exit 1
mkdir "$BUILD_DIRECTORY" || exit 1
cd "$BUILD_DIRECTORY" || exit 1

flatpak-builder --repo=repo app "$SOURCE_DIRECTORY/pkg/linux/pkg-flatpak.yml" || exit 1
flatpak build-bundle repo "$NAME-Linux.flatpak" "com.nightmareci.$NAME" || exit 1

rm -rf app repo || exit 1
