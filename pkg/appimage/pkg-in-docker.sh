#!/bin/sh

if [ -z "$1" ] ; then
	echo 'Usage: pkg-in-docker.sh [source-directory]'
	exit 1
fi

docker run -v "$1":/source -w /source appimagecrafters/docker-linuxdeploy /bin/sh -c '
mkdir build-appimage
cd build-appimage

SDL2=SDL2-2.0.16
curl https://libsdl.org/release/$SDL2.tar.gz -O
tar xzf $SDL2.tar.gz
cd $SDL2
./configure
make -j$(nproc)
make install
cd ..

cmake .. -B build -DCMAKE_INSTALL_PREFIX=/usr || exit 1
DESTDIR=AppDir cmake --build build -j$(nproc) || exit 1
linuxdeploy --appdir=AppDir --output appimage || exit 1
' || exit 1
