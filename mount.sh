#!/bin/bash
if [ -e "env/hdc/umounted" ]; then
    rm env/hdc/umounted
fi
sudo guestmount -a hdc-0.11.img -m /dev/sda1 ./env/hdc
