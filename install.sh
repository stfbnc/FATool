#!/bin/sh

copy_icon() {
    ICONS_PATH="$HOME/.local/share/icons/hicolor"
    mkdir -p "$ICONS_PATH/$1x$1/apps"
    cp "icons/logo_$1.png" "$ICONS_PATH/$1x$1/apps/fatool.png"
}

# binary
cp bin/FATool "$HOME/.local/bin"

# libraries
mkdir -p "$HOME/.local/lib/fatool"
cp lib/* "$HOME/.local/lib/fatool"

# icons
copy_icon 16
copy_icon 24
copy_icon 32
copy_icon 48
copy_icon 64
copy_icon 128
copy_icon 256
copy_icon 512

# .desktop file
cp FATool.desktop "$HOME/.local/share/applications"
sed 's@~@'"$HOME"'@g' FATool.desktop > "$HOME/.local/share/applications/FATool.desktop"

