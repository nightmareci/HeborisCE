#!/bin/bash

if [ `uname -m` != arm64 ] ; then
	echo 'Sorry, building of Universal Binary packages for arm64 and x86_64 with this script is only supported on arm64 Macs' ;
	exit 1 ;
fi

if [ "$1" = Installable -o "$1" = Portable ] ; then
	PACKAGE_TYPE="$1" ;
else
	echo "Requested package type \"$1\" is invalid; valid options are Installable or Portable" ;
	echo "Usage: $0 package_type [codesign_identity]" ;
	exit 1 ;
fi

NAME="HeborisC7EX-SDL2"

BASE_DIR="build-macos-$PACKAGE_TYPE-$NAME"
rm -rf "$BASE_DIR" || exit 1
mkdir -p "$BASE_DIR" || exit 1

BUILD_DIR="$BASE_DIR/build"

# Sometimes, "codesign --remove-signature ..." in an arm64 shell fails, but for some reason running it in an x86_64 shell fixes it. So run it here to force that fix.
touch "$BASE_DIR/codesign-dummy" || exit 1
arch -arch x86_64 bash --login -c "codesign --remove-signature \"$BASE_DIR/codesign-dummy\"" || exit 1
rm "$BASE_DIR/codesign-dummy" || exit 1

# Build app for each architecture that will be in the Universal Binary app.
for arch in arm64 x86_64 ; do
	# Build app for current arch.
	ARCH_BUILD_DIR="$BUILD_DIR-$arch" ;
	arch -arch "$arch" zsh --login -c "cmake -G Ninja -B \"$ARCH_BUILD_DIR\" -DCMAKE_BUILD_TYPE=Release -DPACKAGE_TYPE=\"$PACKAGE_TYPE\" || exit 1 ; cmake --build \"$ARCH_BUILD_DIR\" -j$(expr 2 + $(sysctl -n hw.ncpu))" || exit 1

	# Copy current arch app out of build dir and bundle libs.
	ARCH_APP_DIR="$BASE_DIR/app-$arch" ;
	mkdir -p "$ARCH_APP_DIR" || exit 1 ;
	cp -r "$ARCH_BUILD_DIR/$NAME.app" "$ARCH_APP_DIR" || exit 1 ;
	mkdir -p "$ARCH_APP_DIR/$NAME.app/Contents/libs" || exit 1 ;
	dylibbundler -x "$ARCH_APP_DIR/$NAME.app/Contents/MacOS/$NAME" -d "$ARCH_APP_DIR/$NAME.app/Contents/libs" -b -i /usr/lib || exit 1 ;

	# Remove code signatures.
	for lib in `ls "$ARCH_APP_DIR/$NAME.app/Contents/libs/"` ; do codesign --remove-signature "$ARCH_APP_DIR/$NAME.app/Contents/libs/$lib" || exit 1 ; done ;
	codesign --remove-signature "$ARCH_APP_DIR/$NAME.app" || exit 1 ;
done

SRC_FOLDER="$BASE_DIR/srcfolder"
if [ "$PACKAGE_TYPE" = Installable ] ; then
	APP_DIR="$SRC_FOLDER" ;
elif [ "$PACKAGE_TYPE" = Portable ] ; then
	APP_DIR="$SRC_FOLDER/$NAME" ;
fi
mkdir -p "$APP_DIR" || exit 1
cp -r "$BASE_DIR/app-arm64/$NAME.app" "$APP_DIR" || exit 1

# Copy resources into place.
if [ "$PACKAGE_TYPE" = Installable ] ; then
	cp README.md changelog.txt heboris.txt "$APP_DIR" || exit 1 ;
	RESOURCES_DIR="$APP_DIR/$NAME.app/Contents/Resources" ;
	mkdir -p "$RESOURCES_DIR/config" || exit 1 ;
	cp -r "config/mission" "$RESOURCES_DIR/config" || exit 1 ;
	cp -r "config/stage" "$RESOURCES_DIR/config" || exit 1 ;
	cp -r "res" "$RESOURCES_DIR" || exit 1 ;
	cp heboris.ini "$RESOURCES_DIR" || exit 1 ;
elif [ "$PACKAGE_TYPE" = Portable ] ; then
	cp README.md changelog.txt heboris.txt heboris.ini "$APP_DIR" || exit 1 ;
	mkdir -p "$APP_DIR/config" || exit 1 ;
	cp -r "config/mission" "$APP_DIR/config" || exit 1 ;
	cp -r "config/stage" "$APP_DIR/config" || exit 1 ;
	cp -r "res" "$APP_DIR" || exit 1 ;
fi

# Use lipo to generate the Universal Binary code files.
for lib in `ls "$APP_DIR/$NAME.app/Contents/libs/"` ; do
	rm "$APP_DIR/$NAME.app/Contents/libs/$lib" || exit 1 ;
	lipo "$BASE_DIR/app-arm64/$NAME.app/Contents/libs/$lib" "$BASE_DIR/app-x86_64/$NAME.app/Contents/libs/$lib" -create -output "$APP_DIR/$NAME.app/Contents/libs/$lib" || exit 1 ;
done
rm "$APP_DIR/$NAME.app/Contents/MacOS/$NAME" || exit 1
lipo "$BASE_DIR/app-arm64/$NAME.app/Contents/MacOS/$NAME" "$BASE_DIR/app-x86_64/$NAME.app/Contents/MacOS/$NAME" -create -output "$APP_DIR/$NAME.app/Contents/MacOS/$NAME" || exit 1 ;

# Codesign.

if [ "$2" ] ; then
	IDENTITY="$2" ;
else
	IDENTITY="-" ;
fi

for lib in `ls "$APP_DIR/$NAME.app/Contents/libs/"` ; do
	codesign -f -o runtime --timestamp -s "$IDENTITY" --entitlements "$(dirname "$BASH_SOURCE")/entitlements.xml" "$APP_DIR/$NAME.app/Contents/libs/$lib" || exit 1 ;
done
codesign -f -o runtime --timestamp -s "$IDENTITY" --entitlements "$(dirname "$BASH_SOURCE")/entitlements.xml" "$APP_DIR/$NAME.app" || exit 1

TMP_DMG_DIR="$BASE_DIR/dmg"

hdiutil create "$BASE_DIR/tmp.dmg" -ov -volname "$NAME" -fs HFS+ -srcfolder "$SRC_FOLDER" || exit 1
hdiutil convert "$BASE_DIR/tmp.dmg" -format UDZO -o "$BASE_DIR/$NAME.dmg" || exit 1
rm "$BASE_DIR/tmp.dmg" || exit 1

for arch in arm64 x86_64 ; do
	rm -r "$BUILD_DIR-$arch" || exit 1 ;
	rm -r "$BASE_DIR/app-$arch" || exit 1 ;
done
rm -r "$SRC_FOLDER" || exit 1

codesign -f -o runtime --timestamp -s "$IDENTITY" --entitlements "$(dirname "$BASH_SOURCE")/entitlements.xml" "$BASE_DIR/$NAME.dmg" || exit 1
