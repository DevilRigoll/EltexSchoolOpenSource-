Test
sudo insmod ioctl.ko
sudo su
echo 1 > /sys/kernel/kbledctrl/ledcod
echo 3 > /sys/kernel/kbledctrl/ledcod
echo 6 > /sys/kernel/kbledctrl/ledcod
exit
sudo rmmod ioctl.ko
