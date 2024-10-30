# run this script at the root of the repository

./env/reset_img.sh
./mount.sh
cp exp5/home/* env/hdc/usr/root/
cp linux-0.11/include/unistd.h env/hdc/usr/include/
