#!/bin/bash
sudo rmmod cdc_acm
echo "blacklist cdc_acm" | sudo tee -a /etc/modprobe.d/blacklist.conf
sudo update-initramfs -u
sudo reboot