#!/bin/sh

if [ -z "$1" -o -z "$2" ] ; then
	echo 'Usage: pkg.sh [source-directory] [build-directory]'
	exit 1
fi

if [ -z "$VITASDK" ] ; then
	echo 'The environment variable VITASDK must be set to the root of the Vita SDK installation.'
	exit 1
fi

mkdir "$2"

SOURCE_DIRECTORY=`readlink -f "$1"`
BUILD_DIRECTORY=`readlink -f "$2"`

cd "$SOURCE_DIRECTORY"
cmake -B "$BUILD_DIRECTORY" -G Ninja -DCMAKE_BUILD_TYPE=Release --toolchain "$VITASDK/share/vita.toolchain.cmake" -DAPP_VENDORED=ON -DAPP_RESOURCE_DAT=ON \
	-DSDLIMAGE_AVIF=OFF \
	-DSDLIMAGE_BMP=OFF \
	-DSDLIMAGE_GIF=OFF \
	-DSDLIMAGE_JPG=OFF \
	-DSDLIMAGE_JXL=OFF \
	-DSDLIMAGE_LBM=OFF \
	-DSDLIMAGE_PCX=OFF \
	-DSDLIMAGE_PNG=ON \
	-DSDLIMAGE_PNM=OFF \
	-DSDLIMAGE_QOI=OFF \
	-DSDLIMAGE_SVG=OFF \
	-DSDLIMAGE_TGA=OFF \
	-DSDLIMAGE_TIF=OFF \
	-DSDLIMAGE_WEBP=OFF \
	-DSDLIMAGE_XCF=OFF \
	-DSDLIMAGE_XPM=OFF \
	-DSDLIMAGE_XV=OFF \
	|| exit 1
cmake --build "$BUILD_DIRECTORY" --parallel || exit 1
