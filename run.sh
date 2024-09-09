#!/bin/bash
export OSLAB_PATH=$(dirname `which $0`)

if [ ! $1 -e "skip-build" ]; then
	make all -C $OSLAB_PATH
fi

if [ ! -e "$OSLAB_PATH/hdc/umounted" ]; then
	echo umount hdc first
	sudo umount $OSLAB_PATH/hdc
	if [ "$?" != "0" ]; then
		exit
	fi
fi

$OSLAB_PATH/bochs/bochs-gdb -q -f $OSLAB_PATH/bochs/bochsrc.bxrc
