#!/bin/sh

MAIN_PATH="$1/fatool"
BIN_PATH="$1/fatool/bin"
ICONS_PATH="$1/fatool/icons"
LIB_PATH="$1/fatool/lib"

mkdir -p $MAIN_PATH
mkdir -p $BIN_PATH
mkdir -p $ICONS_PATH
mkdir -p $LIB_PATH

cp FATool.desktop $MAIN_PATH
cp install.sh $MAIN_PATH
cp 3rd_party/ubuntu/libs/* $LIB_PATH
cp icons/logo_*.png $ICONS_PATH
cp ../build-FATool-*/app/release/FATool $BIN_PATH

tar -czf "$1/fatool.tar.gz" $MAIN_PATH --remove-files

