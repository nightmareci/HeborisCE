#!/bin/sh

if [ -z "$1" -o -z "$2" ] ; then
	echo 'Usage: pkg-appimage.sh [source-dir] [build-dir]'
	exit 1
fi

SOURCE_DIR=`readlink -f "$1"`
BUILD_DIR=`readlink -f "$2"`

NAME="HeborisC7EX-SDL2"

rm -rf "$BUILD_DIR" || exit 1
mkdir "$BUILD_DIR" || exit 1
cd "$BUILD_DIR" || exit 1

cmake "$SOURCE_DIR" -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -DPACKAGE_TYPE=Installable || exit 1
cmake --build build -j`expr $(nproc) + 2` || exit 1
DESTDIR=AppDir cmake --install build || exit 1

LINUXDEPLOY="linuxdeploy-$(uname -m).AppImage"
curl -L -O "https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/$LINUXDEPLOY" || exit 1
chmod +x "$LINUXDEPLOY" || exit 1

"./$LINUXDEPLOY" --appdir AppDir --output appimage --icon-file "$SOURCE_DIR/pkg/linux/$NAME.png" --icon-filename "$NAME" --desktop-file "$BUILD_DIR/build/$NAME.desktop" || exit 1
