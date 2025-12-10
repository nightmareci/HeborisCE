#!/bin/bash

USAGE="pkg.sh [package-type] [source-directory] [build-directory] [codesign-identity]"

if [ -z "$1" -o -z "$2" -o -z "$3" ] ; then
	echo "$USAGE"
	exit 1
fi

PACKAGE_TYPE="$1"
if [ "$PACKAGE_TYPE" != 'Installable Mac App' -a "$PACKAGE_TYPE" != 'Portable Mac App' -a "$PACKAGE_TYPE" != 'Portable' ] ; then
	echo "$USAGE"
	exit 1
fi

SOURCE_DIR="$2"

BUILD_DIR_ARG="$3"
mkdir -p "$BUILD_DIR_ARG" || exit 1
BUILD_DIR=`realpath "$BUILD_DIR_ARG"`
if [ $? -ne 0 ] ; then
	exit 1
fi

if [ -z "$4" ] ; then
	IDENTITY=-
else
	IDENTITY="$4"
fi

cmake "$SOURCE_DIR" -B "$BUILD_DIR" -G Xcode \
	-DAPP_VENDORED=ON \
	-DAPP_PACKAGE_TYPE="$PACKAGE_TYPE" \
	-DCMAKE_OSX_ARCHITECTURES='x86_64;arm64' \
	-DCMAKE_OSX_DEPLOYMENT_TARGET=10.13 \
	-DCPACK_BUNDLE_APPLE_CERT_APP="$IDENTITY" \
	-DSDLIMAGE_AVIF=OFF \
	|| exit 1
cmake --build "$BUILD_DIR" --config Release --target package || exit 1
