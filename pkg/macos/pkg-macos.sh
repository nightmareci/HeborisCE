#!/bin/bash

NAME="$1"
BASE_DIR="build-macos-release-$NAME"
BUILD_DIR="$BASE_DIR/build-$(uname -m)"

rm -rf "$BASE_DIR"
cmake -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release -DBUILD_MACOS_BUNDLE=1
cmake --build "$BUILD_DIR" -j`sysctl -n hw.cpu`
#arch -arch x86_64 zsh --login -c 'cmake -B "$BASE_DIR/build-x86_64" -DCMAKE_BUILD_TYPE=Release -DBUILD_MACOS_BUNDLE=1 ; cmake --build "$BASE_DIR/build-x86_64" -j`sysctl -n hw.cpu`'

SRC_FOLDER="$BASE_DIR/srcfolder"

rm -rf "$SRC_FOLDER"
mkdir -p "$SRC_FOLDER/$NAME"
mkdir -p "$SRC_FOLDER/$NAME/config/data"
mkdir -p "$SRC_FOLDER/$NAME/replay"

cp -r "$BUILD_DIR/$NAME.app" "$SRC_FOLDER/$NAME"
mkdir -p "$SRC_FOLDER/$NAME/$NAME.app/Contents/libs"
dylibbundler -x "$SRC_FOLDER/$NAME/$NAME.app/Contents/MacOS/$NAME" -d "$SRC_FOLDER/$NAME/$NAME.app/Contents/libs" -b -i /usr/lib

cp -r "config/mission" "$SRC_FOLDER/$NAME/config"
cp -r "config/stage" "$SRC_FOLDER/$NAME/config"
cp -r "res" "$SRC_FOLDER/$NAME"
cp README.md changelog.txt heboris.ini heboris.txt "$SRC_FOLDER/$NAME"

for lib in `ls "$SRC_FOLDER/$NAME/$NAME.app/Contents/libs/"` ; do codesign --remove-signature "$SRC_FOLDER/$NAME/$NAME.app/Contents/libs/$lib" ; done
codesign --remove-signature "$SRC_FOLDER/$NAME/$NAME.app"

IDENTITY="$2"

for lib in `ls "$SRC_FOLDER/$NAME/$NAME.app/Contents/libs/"` ; do codesign -f -o runtime --timestamp -s "$IDENTITY" --entitlements "$(dirname "$BASH_SOURCE")/entitlements.xml" "$SRC_FOLDER/$NAME/$NAME.app/Contents/libs/$lib" ; done
codesign -f -o runtime --timestamp -s "$IDENTITY" --entitlements "$(dirname "$BASH_SOURCE")/entitlements.xml" "$SRC_FOLDER/$NAME/$NAME.app"

TMP_DMG_DIR="$BASE_DIR/dmg"

rm -rf "$TMP_DMG_DIR"
mkdir -p "$TMP_DMG_DIR"
hdiutil create "$TMP_DMG_DIR/tmp.dmg" -ov -volname "$NAME" -fs HFS+ -srcfolder "$SRC_FOLDER"
hdiutil convert "$TMP_DMG_DIR/tmp.dmg" -format UDZO -o "$TMP_DMG_DIR/$NAME.dmg"
rm "$TMP_DMG_DIR/tmp.dmg"

mv "$TMP_DMG_DIR/$NAME.dmg" "$BASE_DIR"
rm -r "$BUILD_DIR"
rm -r "$SRC_FOLDER"
rm -r "$TMP_DMG_DIR"
codesign -f -o runtime --timestamp -s "$IDENTITY" --entitlements "$(dirname "$BASH_SOURCE")/entitlements.xml" "$BASE_DIR/$NAME.dmg"
