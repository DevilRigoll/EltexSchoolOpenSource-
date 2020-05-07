Test:
sudo insmod proc.ko
sudo su
cat /proc/myproc
echo "Hi man!" > /proc/myproc
cat /proc/myproc
exit
sudo rmmod proc.ko
