#/bin/bash
export ENV_PATH=$(dirname `which $0`)
export OSLAB_PATH="$ENV_PATH/.."

if mountpoint -q $ENV_PATH/hdc; then
    echo "umount env/hdc"
    sudo umount $ENV_PATH/hdc
fi
