#!/bin/bash
export DBG_PATH=$(dirname `which $0`)
export OSLAB_PATH="$DBG_PATH/.."
$OSLAB_PATH/bochs/bochs-dbg -q -f $OSLAB_PATH/bochs/bochsrc.bxrc
