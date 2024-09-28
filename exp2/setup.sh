#!/bin/bash
export EXP2_PATH=$(dirname `which $0`)
export OSLAB_PATH="$EXP2_PATH/.."

$OSLAB_PATH/env/reset_img.sh
$OSLAB_PATH/mount.sh
cp $OSLAB_PATH/linux-0.11/include/unistd.h $OSLAB_PATH/env/hdc/usr/include/
cp $OSLAB_PATH/exp2/home/* $OSLAB_PATH/env/hdc/usr/root/
