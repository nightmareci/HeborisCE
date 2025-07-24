#!/bin/sh

if [ -z "$1" -o -z "$2" ] ; then
	echo 'Usage: pkg-in-docker.sh [source-directory] [build-directory]'
	exit 1
fi

docker run -v "$1":/source -w /source appimagecrafters/docker-linuxdeploy /bin/sh -c '
cmake '$1' -B '$2' -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DAPP_VENDORED=1 || exit 1
DESTDIR=AppDir cmake --build '$2' -j$(nproc) || exit 1
linuxdeploy --appdir=AppDir --output appimage || exit 1
' || exit 1
