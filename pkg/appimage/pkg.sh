#!/bin/sh

LINUXDEPLOY="$1"
SOURCE_DIR="$2"
BUILD_DIR="$3"

if [ -z "$LINUXDEPLOY" -o -z "$SOURCE_DIR" -o -z "$BUILD_DIR" ] ; then
	echo 'Usage: pkg.sh [linuxdeploy-path] [source-dir] [build-dir]'
	echo 'Download latest version of linuxdeploy here: https://github.com/linuxdeploy/linuxdeploy/releases/'
	exit 1
fi

APP_NAME="HeborisC7EX-SDL2"

rm -rf "$BUILD_DIR" || exit 1
cmake -B "$BUILD_DIR/build" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -DPACKAGE_TYPE=Installable || exit 1
cmake --build "$BUILD_DIR/build" -j`expr $(nproc) + 2` || exit 1
DESTDIR="$BUILD_DIR/AppDir" cmake --install "$BUILD_DIR/build" || exit 1
cd "$BUILD_DIR"
"$LINUXDEPLOY" --appdir "$BUILD_DIR/AppDir" --output appimage --icon-file "$SOURCE_DIR/pkg/appimage/$APP_NAME.png" --icon-filename "$APP_NAME" --desktop-file "$SOURCE_DIR/pkg/appimage/$APP_NAME.desktop" || exit 1
