#!/bin/bash

NAME="HeborisC7EX-SDL2"

BASE_DIR="build-macos-portable-release-$NAME"
rm -rf "$BASE_DIR"

BUILD_DIR="$BASE_DIR/build-$(uname -m)"
PACKAGE_TYPE="Portable"

cmake -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release -DPACKAGE_TYPE="$PACKAGE_TYPE"
cmake --build "$BUILD_DIR" -j`sysctl -n hw.cpu`
#arch -arch x86_64 bash --login -c 'cmake -B "$BASE_DIR/build-x86_64" -DCMAKE_BUILD_TYPE=Release -DPACKAGE_TYPE="$PACKAGE_TYPE" ; cmake --build "$BASE_DIR/build-x86_64" -j`sysctl -n hw.cpu`'

SRC_FOLDER="$BASE_DIR/srcfolder"

mkdir -p "$SRC_FOLDER/$NAME"
mkdir -p "$SRC_FOLDER/$NAME/replay"
mkdir -p "$SRC_FOLDER/$NAME/config/data"
cp README.md changelog.txt heboris.ini heboris.txt "$SRC_FOLDER/$NAME"
cp -r "config/mission" "$SRC_FOLDER/$NAME/config"
cp -r "config/stage" "$SRC_FOLDER/$NAME/config"
cp -r "res" "$SRC_FOLDER/$NAME"

cp -r "$BUILD_DIR/$NAME.app" "$SRC_FOLDER/$NAME"
mkdir -p "$SRC_FOLDER/$NAME/$NAME.app/Contents/libs"
dylibbundler -x "$SRC_FOLDER/$NAME/$NAME.app/Contents/MacOS/$NAME" -d "$SRC_FOLDER/$NAME/$NAME.app/Contents/libs" -b -i /usr/lib

for lib in `ls "$SRC_FOLDER/$NAME/$NAME.app/Contents/libs/"` ; do codesign --remove-signature "$SRC_FOLDER/$NAME/$NAME.app/Contents/libs/$lib" ; done
codesign --remove-signature "$SRC_FOLDER/$NAME/$NAME.app"

if [ "$1" ] ; then
	IDENTITY="$1" ;
else
	IDENTITY="-" ;
fi

for lib in `ls "$SRC_FOLDER/$NAME/$NAME.app/Contents/libs/"` ; do codesign -f -o runtime --timestamp -s "$IDENTITY" --entitlements "$(dirname "$BASH_SOURCE")/entitlements.xml" "$SRC_FOLDER/$NAME/$NAME.app/Contents/libs/$lib" ; done
codesign -f -o runtime --timestamp -s "$IDENTITY" --entitlements "$(dirname "$BASH_SOURCE")/entitlements.xml" "$SRC_FOLDER/$NAME/$NAME.app"

TMP_DMG_DIR="$BASE_DIR/dmg"

hdiutil create "$BASE_DIR/tmp.dmg" -ov -volname "$NAME" -fs HFS+ -srcfolder "$SRC_FOLDER"
hdiutil convert "$BASE_DIR/tmp.dmg" -format UDZO -o "$BASE_DIR/$NAME.dmg"
rm "$BASE_DIR/tmp.dmg"

rm -r "$BUILD_DIR"
rm -r "$SRC_FOLDER"

codesign -f -o runtime --timestamp -s "$IDENTITY" --entitlements "$(dirname "$BASH_SOURCE")/entitlements.xml" "$BASE_DIR/$NAME.dmg"
