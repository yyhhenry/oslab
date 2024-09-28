#!/bin/bash
export OSLAB_PATH=$(dirname `which $0`)
export BOCHS_PATH="$OSLAB_PATH/env/bochs"

xmake build image
if [ "$?" != "0" ]; then
	echo "xmake build image failed"
	exit
fi

$OSLAB_PATH/env/umount_if_needed.sh

$BOCHS_PATH/bochs-gdb -q -f $BOCHS_PATH/bochsrc.bxrc
