#!/bin/sh

for dylib in `ls "$BUILT_PRODUCTS_DIR/$1/"*.dylib` ; do
	codesign --force --options runtime --timestamp --entitlements "$CODE_SIGN_ENTITLEMENTS" --sign "$EXPANDED_CODE_SIGN_IDENTITY" "$dylib" || exit 1
done

exit 0
