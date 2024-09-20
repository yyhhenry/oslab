#!/bin/bash
export ENV_PATH=$(dirname `which $0`)
export OSLAB_PATH="$ENV_PATH/.."

cp $ENV_PATH/hdc-0.11.img.bak $OSLAB_PATH/hdc-0.11.img
if [ ! -e "$OSLAB_PATH/env/hdc/umounted" ]; then
	echo umount env/hdc
	mkdir -p ./env/hdc
	guestunmount $OSLAB_PATH/env/hdc
	touch $OSLAB_PATH/env/hdc/umounted
	if [ "$?" != "0" ]; then
		exit
	fi
fi
