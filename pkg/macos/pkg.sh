#!/bin/bash

USAGE="pkg.sh [package-type] [source-directory] [build-directory] [codesign-identity]"

if [ `uname -m` != arm64 ] ; then
	echo 'Sorry, building of Universal Binary packages for arm64 and x86_64 with this script is only supported on arm64 Macs'
	exit 1
fi

if [ "$1" = Installable -o "$1" = Portable ] ; then
	PACKAGE_TYPE="$1"
else
	echo "Requested package type \"$1\" is invalid; valid options are \"Installable\" or \"Portable\""
	echo "$USAGE"
	exit 1
fi

if [ -z "$2" -o -z "$3" ] ; then
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

# Sometimes, "codesign --remove-signature ..." in an arm64 shell fails, but for
# some reason running it in an x86_64 shell fixes it. So run it here to force
# that fix.
touch "$BASE_DIRECTORY/codesign-dummy" || exit 1
arch -arch x86_64 bash --login -c "codesign --remove-signature \"$BASE_DIRECTORY/codesign-dummy\"" || exit 1
rm "$BASE_DIRECTORY/codesign-dummy" || exit 1

# Build app for each architecture that will be in the Universal Binary app.
for arch in arm64 x86_64 ; do
	# Build app for current arch.
	ARCH_BUILD_DIRECTORY="$BUILD_DIRECTORY-$arch"
	arch -arch "$arch" zsh --login -c "cmake \"$SOURCE_DIRECTORY\" -G Ninja -B \"$ARCH_BUILD_DIRECTORY\" -DCMAKE_BUILD_TYPE=Release -DPACKAGE_TYPE=\"$PACKAGE_TYPE Mac App\" || exit 1 ; cmake --build \"$ARCH_BUILD_DIRECTORY\"" || exit 1

	# Copy current arch app out of build dir and bundle libs.
	ARCH_APP_DIRECTORY="$BASE_DIRECTORY/app-$arch"
	mkdir -p "$ARCH_APP_DIRECTORY" || exit 1
	cp -r "$ARCH_BUILD_DIRECTORY/$NAME.app" "$ARCH_APP_DIRECTORY" || exit 1
	mkdir -p "$ARCH_APP_DIRECTORY/$NAME.app/Contents/libs" || exit 1
	dylibbundler -x "$ARCH_APP_DIRECTORY/$NAME.app/Contents/MacOS/$NAME" -cd -d "$ARCH_APP_DIRECTORY/$NAME.app/Contents/libs" -p @rpath/../libs/ -b -i /usr/lib || exit 1
	# Annoyingly, dylibbundler doesn't offer a way to directly set the
	# rpath of the executable separately from the -p option. But it at
	# least always sets the rpath to the -p option's value, so we can
	# reliably change it to the correct value here.
	install_name_tool -rpath @rpath/../libs/ @executable_path "$ARCH_APP_DIRECTORY/$NAME.app/Contents/MacOS/$NAME" || exit 1

	# Remove code signatures.
	for lib in `ls "$ARCH_APP_DIRECTORY/$NAME.app/Contents/libs/"` ; do codesign --remove-signature "$ARCH_APP_DIRECTORY/$NAME.app/Contents/libs/$lib" || exit 1 ; done
	codesign --remove-signature "$ARCH_APP_DIRECTORY/$NAME.app" || exit 1
done

# Copy a built app into the source folder, that will have its code files
# replaced with codesigned, universal binaries.
SRC_FOLDER="$BASE_DIRECTORY/srcfolder"
if [ "$PACKAGE_TYPE" = Installable ] ; then
	APP_DIRECTORY="$SRC_FOLDER"
elif [ "$PACKAGE_TYPE" = Portable ] ; then
	APP_DIRECTORY="$SRC_FOLDER/$NAME"
fi
mkdir -p "$APP_DIRECTORY" || exit 1
cp -r "$BASE_DIRECTORY/app-arm64/$NAME.app" "$APP_DIRECTORY" || exit 1

# Copy resources into place.
if [ "$PACKAGE_TYPE" = Installable ] ; then
	cp README.md changelog.txt heboris.txt "$APP_DIRECTORY" || exit 1
	RESOURCES_DIRECTORY="$APP_DIRECTORY/$NAME.app/Contents/Resources"
	mkdir -p "$RESOURCES_DIRECTORY/config" || exit 1
	cp -r "config/mission" "$RESOURCES_DIRECTORY/config" || exit 1
	cp -r "config/stage" "$RESOURCES_DIRECTORY/config" || exit 1
	cp -r "res" "$RESOURCES_DIRECTORY" || exit 1
	cp heboris.ini "$RESOURCES_DIRECTORY" || exit 1
elif [ "$PACKAGE_TYPE" = Portable ] ; then
	cp README.md changelog.txt heboris.txt heboris.ini "$APP_DIRECTORY" || exit 1
	mkdir -p "$APP_DIRECTORY/config" || exit 1
	cp -r "config/mission" "$APP_DIRECTORY/config" || exit 1
	cp -r "config/stage" "$APP_DIRECTORY/config" || exit 1
	cp -r "res" "$APP_DIRECTORY" || exit 1
fi

# Use lipo to generate the Universal Binary code files.
for lib in `ls "$APP_DIRECTORY/$NAME.app/Contents/libs/"` ; do
	rm "$APP_DIRECTORY/$NAME.app/Contents/libs/$lib" || exit 1
	lipo "$BASE_DIRECTORY/app-arm64/$NAME.app/Contents/libs/$lib" "$BASE_DIRECTORY/app-x86_64/$NAME.app/Contents/libs/$lib" -create -output "$APP_DIRECTORY/$NAME.app/Contents/libs/$lib" || exit 1
done
rm "$APP_DIRECTORY/$NAME.app/Contents/MacOS/$NAME" || exit 1
lipo "$BASE_DIRECTORY/app-arm64/$NAME.app/Contents/MacOS/$NAME" "$BASE_DIRECTORY/app-x86_64/$NAME.app/Contents/MacOS/$NAME" -create -output "$APP_DIRECTORY/$NAME.app/Contents/MacOS/$NAME" || exit 1

# Codesign.
if [ "$2" ] ; then
	IDENTITY="$2"
else
	IDENTITY="-"
fi
if [ "$IDENTITY" = "-" ] ; then
	ENTITLEMENTS="entitlements-adhoc.xml"
else
	ENTITLEMENTS="entitlements-identity-required.xml"
fi
for lib in `ls "$APP_DIRECTORY/$NAME.app/Contents/libs/"` ; do
	codesign -f -o runtime --timestamp -s "$IDENTITY" --entitlements "$(dirname "$BASH_SOURCE")/$ENTITLEMENTS" "$APP_DIRECTORY/$NAME.app/Contents/libs/$lib" || exit 1
done
codesign -f -o runtime --timestamp -s "$IDENTITY" --entitlements "$(dirname "$BASH_SOURCE")/$ENTITLEMENTS" "$APP_DIRECTORY/$NAME.app" || exit 1

# Generate final DMG package.
hdiutil create "$BASE_DIRECTORY/tmp.dmg" -ov -volname "$NAME" -fs HFS+ -srcfolder "$SRC_FOLDER" || exit 1
hdiutil convert "$BASE_DIRECTORY/tmp.dmg" -format UDZO -o "$BASE_DIRECTORY/$NAME-macOS-$PACKAGE_TYPE.dmg" || exit 1
rm "$BASE_DIRECTORY/tmp.dmg" || exit 1
for arch in arm64 x86_64 ; do
	rm -r "$BUILD_DIRECTORY-$arch" || exit 1
	rm -r "$BASE_DIRECTORY/app-$arch" || exit 1
done
rm -r "$SRC_FOLDER" || exit 1
codesign -f -o runtime --timestamp -s "$IDENTITY" --entitlements "$(dirname "$BASH_SOURCE")/$ENTITLEMENTS" "$BASE_DIRECTORY/$NAME-macOS-$PACKAGE_TYPE.dmg" || exit 1
