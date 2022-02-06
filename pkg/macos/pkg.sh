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
BUILD_DIR="$3"
if [ -z "$4" ] ; then
	IDENTITY=-
	ENTITLEMENTS=adhoc
else
	IDENTITY="$4"
	ENTITLEMENTS=identity-required
fi

realpath()
{
python - <<EOF "$1" || exit 1
import ctypes, sys

libc = ctypes.CDLL('libc.dylib')
libc.realpath.restype = ctypes.c_char_p
libc.__error.restype = ctypes.POINTER(ctypes.c_int)
libc.strerror.restype = ctypes.c_char_p

def realpath(path):
    buffer = ctypes.create_string_buffer(1024) # PATH_MAX
    libc.realpath(path, buffer)
    return buffer.value

if __name__ == '__main__':
    print realpath(sys.argv[1])
EOF
}

OLD_PWD="$PWD"
rm -rf "$BUILD_DIR" || exit 1
cmake "$SOURCE_DIR" -B "$BUILD_DIR/build" -G Ninja -DCMAKE_BUILD_TYPE=Release -DPACKAGE_TYPE="$PACKAGE_TYPE" -DAPPLE_CERT_NAME="$IDENTITY" -DCMAKE_OSX_ARCHITECTURES='x86_64;arm64' --install-prefix "$(realpath $BUILD_DIR)/install" || exit 1
cmake --build "$BUILD_DIR/build" || exit 1
cmake --install "$BUILD_DIR/build" || exit 1
cd "$BUILD_DIR/build" || exit 1
cpack || exit 1
mv *.dmg .. || exit 1
cd ..
rm -rf build install
cd "$OLD_PWD"
xcrun codesign -f -o runtime --timestamp -s "$IDENTITY" --entitlements "$(dirname "$BASH_SOURCE")/entitlements-$ENTITLEMENTS.xml" "$BUILD_DIR/"*.dmg || exit 1
