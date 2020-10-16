#!/bin/sh

QT_DIR="$1"
BUILD_DIR="$2"

# in case it does not exist
mkdir -p "$BUILD_DIR/app/release/FATool.app/Contents/Frameworks"

# copy 3rd party libraries
cp ./3rd_party/macos/libs/* "$BUILD_DIR/app/release/FATool.app/Contents/Frameworks"

# qt frameworks
cd "$QT_DIR/bin"
./macdeployqt "$BUILD_DIR/app/release/FATool.app" -dmg
