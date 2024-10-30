# run this script at the root of the repository

./mount.sh
sudo cp env/hdc/usr/root/sem.log exp4/sem.log
sudo chmod +rw exp4/sem.log
python3 resolve_sem_log.py
