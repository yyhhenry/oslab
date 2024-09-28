#!/bin/bash
export OSLAB_PATH=$(dirname `which $0`)
export MNT_PATH="$OSLAB_PATH/env/hdc"

if [ -e "$MNT_PATH/umounted" ]; then
    rm $MNT_PATH/umounted
else
    echo "umount env/hdc first"
    exit
fi

sudo mount -o loop,offset=1024 $OSLAB_PATH/hdc-0.11.img $MNT_PATH
