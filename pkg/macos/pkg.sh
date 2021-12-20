#!/bin/bash

USAGE="pkg.sh [package-type] [source-directory] [build-directory] [codesign-identity]"

if [ -z "$1" -o -z "$2" -o -z "$3" ] ; then
	echo "$USAGE"
	exit 1
fi

PACKAGE_TYPE="$1"
if [ "$PACKAGE_TYPE" != Installable -a "$PACKAGE_TYPE" != Portable ] ; then
	echo "$USAGE"
	exit 1
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
SOURCE_DIRECTORY=`realpath "$2"`
BASE_DIRECTORY=`realpath "$3"`

if [ -z "$SOURCE_DIRECTORY" -o -z "$BASE_DIRECTORY" ] ; then
	echo "$USAGE"
	exit 1
fi

NAME="HeborisC7EX-SDL2"

rm -rf "$BASE_DIRECTORY" || exit 1
mkdir -p "$BASE_DIRECTORY" || exit 1

BUILD_DIRECTORY="$BASE_DIRECTORY/build"

# Build the app.
cmake "$SOURCE_DIRECTORY" -G Ninja -B "$BUILD_DIRECTORY" -DCMAKE_BUILD_TYPE=Release -DPACKAGE_TYPE="$PACKAGE_TYPE Mac App" -DCMAKE_OSX_ARCHITECTURES='x86_64;arm64' || exit 1
cmake --build "$BUILD_DIRECTORY" || exit 1

# Copy built app into the source folder.
SRC_FOLDER="$BASE_DIRECTORY/srcfolder"
if [ "$PACKAGE_TYPE" = Installable ] ; then
	APP_FOLDER="$SRC_FOLDER"
elif [ "$PACKAGE_TYPE" = Portable ] ; then
	APP_FOLDER="$SRC_FOLDER/$NAME"
fi
mkdir -p "$APP_FOLDER" || exit 1
cp -r "$BUILD_DIRECTORY/$NAME.app" "$APP_FOLDER" || exit 1

# Bundle libraries.
dylibbundler -x "$APP_FOLDER/$NAME.app/Contents/MacOS/$NAME" -cd -d "$APP_FOLDER/$NAME.app/Contents/libs" -p @rpath/../libs/ -b -i /usr/lib || exit 1
# Annoyingly, dylibbundler doesn't offer a way to directly set the
# rpath of the executable separately from the -p option. But it at
# least always sets the rpath to the -p option's value, so we can
# reliably change it to the correct value here.
xcrun install_name_tool -rpath @rpath/../libs/ @executable_path "$APP_FOLDER/$NAME.app/Contents/MacOS/$NAME" || exit 1

# Copy resources into place.
# Copy resources into place.
if [ "$PACKAGE_TYPE" = Installable ] ; then
	cp README.md changelog.txt heboris.txt "$APP_FOLDER" || exit 1
	RESOURCES_DIRECTORY="$APP_FOLDER/$NAME.app/Contents/Resources"
	mkdir -p "$RESOURCES_DIRECTORY/config" || exit 1
	cp -r "config/mission" "$RESOURCES_DIRECTORY/config" || exit 1
	cp -r "config/stage" "$RESOURCES_DIRECTORY/config" || exit 1
	cp -r "res" "$RESOURCES_DIRECTORY" || exit 1
elif [ "$PACKAGE_TYPE" = Portable ] ; then
	cp README.md changelog.txt heboris.txt "$APP_FOLDER" || exit 1
	mkdir -p "$APP_FOLDER/config" || exit 1
	cp -r "config/mission" "$APP_FOLDER/config" || exit 1
	cp -r "config/stage" "$APP_FOLDER/config" || exit 1
	cp -r "res" "$APP_FOLDER" || exit 1
fi

# Add link to "/Applications".
ln -s /Applications "$SRC_FOLDER/Applications" || exit 1

# Codesign.
for lib in `ls "$APP_FOLDER/$NAME.app/Contents/libs/"` ; do xcrun codesign --remove-signature "$APP_FOLDER/$NAME.app/Contents/libs/$lib" || exit 1 ; done
xcrun codesign --remove-signature "$APP_FOLDER/$NAME.app" || exit 1
if [ -z "$4" ] ; then
	IDENTITY="-"
else
	IDENTITY="$4"
fi
if [ "$IDENTITY" = "-" ] ; then
	ENTITLEMENTS="entitlements-adhoc.xml"
else
	ENTITLEMENTS="entitlements-identity-required.xml"
fi
for lib in `ls "$APP_FOLDER/$NAME.app/Contents/libs/"` ; do
	xcrun codesign -f -o runtime --timestamp -s "$IDENTITY" --entitlements "$(dirname "$BASH_SOURCE")/$ENTITLEMENTS" "$APP_FOLDER/$NAME.app/Contents/libs/$lib" || exit 1
done
xcrun codesign -f -o runtime --timestamp -s "$IDENTITY" --entitlements "$(dirname "$BASH_SOURCE")/$ENTITLEMENTS" "$APP_FOLDER/$NAME.app" || exit 1

# Generate final DMG package.
hdiutil create "$BASE_DIRECTORY/tmp.dmg" -ov -volname "$NAME" -fs HFS+ -srcfolder "$SRC_FOLDER" || exit 1
hdiutil convert "$BASE_DIRECTORY/tmp.dmg" -format UDZO -o "$BASE_DIRECTORY/$NAME-$PACKAGE_TYPE-macOS.dmg" || exit 1
rm "$BASE_DIRECTORY/tmp.dmg" || exit 1
rm -r "$BUILD_DIRECTORY"
rm -r "$SRC_FOLDER" || exit 1
xcrun codesign -f -o runtime --timestamp -s "$IDENTITY" --entitlements "$(dirname "$BASH_SOURCE")/$ENTITLEMENTS" "$BASE_DIRECTORY/$NAME-$PACKAGE_TYPE-macOS.dmg" || exit 1
