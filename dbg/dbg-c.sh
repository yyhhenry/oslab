#!/bin/bash
export DBG_PATH=$(dirname `which $0`)
export OSLAB_PATH="$DBG_PATH/.."
$OSLAB_PATH/env/bochs/bochs-gdb -q -f $OSLAB_PATH/env/bochs/bochsrc-gdb.bxrc
