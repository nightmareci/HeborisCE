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
	ENTITLEMENTS=adhoc
else
	IDENTITY="$4"
	ENTITLEMENTS=identity-required
fi

OLD_PWD="$PWD"
cmake "$SOURCE_DIR" -B "$BUILD_DIR/build" -G Ninja -DCMAKE_BUILD_TYPE=Release -DUSE_VENDOR_LIBRARIES=1 -DPACKAGE_TYPE="$PACKAGE_TYPE" -DAPPLE_CERT_NAME="$IDENTITY" -DCMAKE_OSX_ARCHITECTURES='x86_64;arm64' -DCMAKE_OSX_DEPLOYMENT_TARGET=10.11 --install-prefix "$BUILD_DIR/install" || exit 1
cmake --build "$BUILD_DIR/build" || exit 1
cmake --install "$BUILD_DIR/build" || exit 1
cd "$BUILD_DIR/build" || exit 1
cpack || exit 1
mv *.dmg .. || exit 1
cd ..
cd "$OLD_PWD"
xcrun codesign -f -o runtime --timestamp -s "$IDENTITY" --entitlements "$(dirname "$BASH_SOURCE")/entitlements-$ENTITLEMENTS.xml" "$BUILD_DIR/"*.dmg || exit 1
