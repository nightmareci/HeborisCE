#!/bin/sh

LINUXDEPLOY="$1"

if [ -z "$LINUXDEPLOY" ] ; then
	echo 'Usage: pkg.sh [linuxdeploy-path]'
	echo 'Download latest version of linuxdeploy here: https://github.com/linuxdeploy/linuxdeploy/releases/'
	exit 1
fi

rm -r AppDir build-appimage || exit 1
cmake -B build-appimage -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -DPACKAGE_TYPE=Installable || exit 1
cmake --build build-appimage -j`expr $(nproc) + 2` || exit 1
DESTDIR=AppDir cmake --install build-appimage || exit 1
"$LINUXDEPLOY" --appdir AppDir --output appimage --icon-file pkg/appimage/HeborisC7EX-SDL2.png --icon-filename HeborisC7EX-SDL2 --desktop-file pkg/appimage/HeborisC7EX-SDL2.desktop || exit 1
