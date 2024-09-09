#!/bin/bash
export OSLAB_PATH=$(dirname `which $0`)
export BOCHS_PATH="$OSLAB_PATH/env/bochs"

make -C $OSLAB_PATH

if [ ! -e "$OSLAB_PATH/env/hdc/umounted" ]; then
	echo umount env/hdc first
	sudo umount $OSLAB_PATH/env/hdc
	if [ "$?" != "0" ]; then
		exit
	fi
fi

$BOCHS_PATH/bochs-gdb -q -f $BOCHS_PATH/bochsrc.bxrc
