#!/bin/bash
export OSLAB_PATH=$(dirname `which $0`)
export BOCHS_PATH="$OSLAB_PATH/env/bochs"

xmake build image

if [ ! -e "$OSLAB_PATH/env/hdc/umounted" ]; then
	echo umount env/hdc first
	guestunmount $OSLAB_PATH/env/hdc
	touch $OSLAB_PATH/env/hdc/umounted
	if [ "$?" != "0" ]; then
		exit
	fi
fi

$BOCHS_PATH/bochs-gdb -q -f $BOCHS_PATH/bochsrc.bxrc
