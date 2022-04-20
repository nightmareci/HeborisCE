#!/bin/sh

if [ -z "$1" -o -z "$2" ] ; then
	echo 'Usage: pkg-appimage.sh [source-directory] [build-directory]'
	exit 1
fi

SOURCE_DIRECTORY=`readlink -f "$1"`
BUILD_DIRECTORY=`readlink -f "$2"`

NAME="HeborisC7EX-SDL2"

rm -rf "$BUILD_DIRECTORY" || exit 1
mkdir "$BUILD_DIRECTORY" || exit 1
cd "$BUILD_DIRECTORY" || exit 1

cmake "$SOURCE_DIRECTORY" -G Ninja -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -DPACKAGE_TYPE=Installable || exit 1
cmake --build build || exit 1
DESTDIR=AppDir cmake --install build || exit 1

LINUXDEPLOY="linuxdeploy-$(uname -m).AppImage"
curl -L -O "https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/$LINUXDEPLOY" || exit 1
chmod +x "$LINUXDEPLOY" || exit 1

"./$LINUXDEPLOY" --appdir AppDir --output appimage --icon-file "$SOURCE_DIRECTORY/pkg/linux/icon.png" --icon-filename "$NAME" --desktop-file "$BUILD_DIRECTORY/build/$NAME.desktop" || exit 1

rm -rf "$BUILD_DIRECTORY/AppDir" "$BUILD_DIRECTORY/build" "$BUILD_DIRECTORY/$LINUXDEPLOY" || exit 1
