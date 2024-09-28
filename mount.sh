#!/bin/bash
export OSLAB_PATH=$(dirname `which $0`)
export MNT_PATH="$OSLAB_PATH/env/hdc"

$OSLAB_PATH/env/umount_if_needed.sh
echo "mount env/hdc"
sudo mount -o loop,offset=1024 $OSLAB_PATH/hdc-0.11.img $MNT_PATH
