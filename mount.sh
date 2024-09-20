#!/bin/bash
export OSLAB_PATH=$(dirname `which $0`)
export MNT_PATH="$OSLAB_PATH/env/hdc"

if [ -e "$MNT_PATH/umounted" ]; then
    rm $MNT_PATH/umounted
else
    echo "umount env/hdc first"
    exit
fi

# Try `libguestfs-test-tool` if you have any problem
guestmount -a $OSLAB_PATH/hdc-0.11.img -m /dev/sda1 $MNT_PATH
