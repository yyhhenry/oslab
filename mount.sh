#!/bin/bash
rm env/hda/unmounted
guestmount -a hdc-0.11.img -m /dev/sda1 ./env/hdc
