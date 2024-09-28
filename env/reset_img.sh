#!/bin/bash
export ENV_PATH=$(dirname `which $0`)
export OSLAB_PATH="$ENV_PATH/.."

$ENV_PATH/umount_if_needed.sh
cp $ENV_PATH/hdc-0.11.img.bak $OSLAB_PATH/hdc-0.11.img
