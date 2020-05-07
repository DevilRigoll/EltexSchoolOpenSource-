Test:
make
sudo insmod chardev.ko
sudo su
echo "hello" > /dev/chardrv
dmesg | tail -2
cat /dev/chardrv
dmesg | tail -2
exit
sudo rmmod chardev.ko
make clean
