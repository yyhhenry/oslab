#!/bin/bash
export DBG_PATH=$(dirname `which $0`)
export OSLAB_PATH="$DBG_PATH/.."

if [ ! -e "$OSLAB_PATH/hdc/umounted" ]; then
	echo umount hdc first
	sudo umount $OSLAB_PATH/hdc
	if [ "$?" != "0" ]; then
		exit
	fi
fi

$DBG_PATH/gdb -x $DBG_PATH/gdb-cmd.txt $OSLAB_PATH/linux-0.11/tools/system
