#!/bin/sh

if [ -z "$1" -o -z "$2" ] ; then
	echo 'Usage: pkg-appimage.sh [source-directory] [build-directory] (cmake-generator)'
	exit 1
fi

SOURCE_DIRECTORY=`readlink -f "$1"`
BUILD_DIRECTORY=`readlink -f "$2"`

if [ -z "$3" ] ; then
	GENERATOR='Unix Makefiles'
else
	GENERATOR="$3"
fi

NAME="HeborisCE"

mkdir "$BUILD_DIRECTORY"
cd "$BUILD_DIRECTORY" || exit 1

cmake "$SOURCE_DIRECTORY" -G "$GENERATOR" -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_INSTALL_BINDIR=bin -DCMAKE_INSTALL_LIBDIR=lib -DCMAKE_INSTALL_DATAROOTDIR=share -DAPP_PACKAGE_TYPE=Installable -DAPP_VENDORED=ON || exit 1
cmake --build build --parallel || exit 1
DESTDIR=AppDir cmake --install build || exit 1

LINUXDEPLOY="linuxdeploy-$(uname -m).AppImage"
curl -L -O "https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/$LINUXDEPLOY" || exit 1
chmod +x "$LINUXDEPLOY" || exit 1

"./$LINUXDEPLOY" --appdir AppDir --deploy-deps-only "AppDir/usr/lib" --output appimage --icon-file "$SOURCE_DIRECTORY/pkg/linux/$NAME.png" || exit 1
