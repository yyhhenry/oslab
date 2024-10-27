#!/bin/bash
export KEYBOARD_SRC_PATH=$(dirname `which $0`)

source="$KEYBOARD_SRC_PATH/keyboard.S"
expanded="$KEYBOARD_SRC_PATH/keyboard.s"
target="$KEYBOARD_SRC_PATH/../keyboard.s"

echo "expanding $source"

gcc-3.4 -m32 -E -nostdinc -Ilinux-0.11/include $source -o $expanded
if [ $? -ne 0 ]; then
    echo "failed to expand $source"
    exit 1
fi

if [ ! -f "$target" ]; then
    echo "keyboard.s does not exist"
    cp "$expanded" "$target"
elif cmp -s "$expanded" "$target"; then
    echo "keyboard.s is already up to date"
else
    echo "updating keyboard.s"
    cp "$expanded" "$target"
fi

rm -f "$expanded"
