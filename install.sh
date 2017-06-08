#!/bin/bash
##  Setting the error catch
set -e
sudo apt-get update 

echo "################################################################"
echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
echo "QEMU and Code Sourcery toolchain to build cortexm3 projects"
echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
echo "################################################################"

##  Clone the  Qemu reository in the home folder
echo "################################################################"
echo "Cloning the qemu for STM32 in home directory"
echo "################################################################"

cd ~
git clone https://github.com/beckus/qemu_stm32
cd qemu_stm32

##  Building the Qemu for STM32
echo "################################################################"
echo "Fetching the DTC module"
echo "################################################################"
git submodule update --init dtc

echo "################################################################"
echo "Fetching the pixman module"
echo "################################################################"
git submodule update --init pixman

echo "################################################################"
echo "Checking/Installing dependencies"
echo "################################################################"
sudo apt-get install libglib2.0-dev zlib1g-dev
sudo apt-get install dh-autoreconf
sudo apt-get install lib32stdc++6

echo "################################################################"
echo "Configure"
echo "################################################################"
./configure --disable-werror --target-list="arm-softmmu"

echo "################################################################"
echo "Building Qemu"
echo "################################################################"
make


echo "################################################################"
echo "Downloading the CodeSourcery toolchain installer" 
echo "################################################################"
mkdir -p ~/stm32/CodeBench/
cd ~/stm32/CodeBench/
wget https://sourcery.mentor.com/sgpp/lite/arm/portal/package8736/public/arm-none-eabi/arm-2011.03-42-arm-none-eabi.bin

echo "################################################################"
echo "Codebench has a problem with dash, please select no"
echo "################################################################"
sleep 5
sudo dpkg-reconfigure -plow dash
ls -lh /bin/sh
chmod +x arm-2011.03-42-arm-none-eabi.bin

echo "################################################################"
echo "Starting the installer, follow the instructions"
echo "################################################################"
/bin/sh arm-2011.03-42-arm-none-eabi.bin
echo "################################################################"
echo "Adding Qemu and toolchain to PATH"
echo "################################################################"
echo "export PATH=$HOME/CodeSourcery/Sourcery_G++_Lite/bin:$HOME/qemu_stm32/arm-softmmu:$PATH" >> ~/.bashrc

echo "################################################################"
echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
echo "Installation is complete "
echo "Please run the following command in your terminal"
echo "source ~/.bashrc"
echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
echo "################################################################"


#fi
