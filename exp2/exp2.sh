#!/bin/bash
export EXP2_PATH=$(dirname `which $0`)
export OSLAB_PATH="$EXP2_PATH/.."

cp $OSLAB_PATH/env/hdc-0.11.img.bak $OSLAB_PATH/hdc-0.11.img
$OSLAB_PATH/mount.sh
cp $OSLAB_PATH/linux-0.11/include/unistd.h $OSLAB_PATH/env/hdc/usr/include/
cp $OSLAB_PATH/exp2/home/* $OSLAB_PATH/env/hdc/usr/root/
