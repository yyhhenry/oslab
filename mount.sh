#!/bin/bash
if [ -e "env/hdc/umounted" ]; then
    rm env/hdc/umounted
else
    echo "umount env/hdc first"
    exit
fi

# Try `libguestfs-test-tool` if you have any problem
guestmount -a hdc-0.11.img -m /dev/sda1 ./env/hdc
