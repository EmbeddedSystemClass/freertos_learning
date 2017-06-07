#!/bin/bash


echo "################################################################"
echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
echo "QEMU and Code Sourcery toolchain to build cortexm3 projects"
echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
echo "################################################################"

#echo "This script will install Qemu and Code Sourcery toolchain"
#read -r -p "Continue (Y/n)?" choice
#choice=${choice,,} # tolower
# if [[ $choice =~ ^(yes|y| ) ]] | [ -z $choice ]; then
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
echo "Configure"
echo "################################################################"
./configure --enable-debug --target-list="arm-softmmu"

echo "################################################################"
echo "Building Qemu"
echo "################################################################"
make

echo "################################################################"
echo "Adding Qemu to PATH"
echo "################################################################"
echo "export PATH= $HOME/qemu_stm32/arm-softmmu:$PATH" >> ~/.bashrc
source ~/.bashrc

echo "################################################################"
echo "Downloading the CodeSourcery toolchain installer" 
echo "################################################################"
mkdir -p ~/stm32/CodeBench/
cd ~/stm32/CodeBench/
wget https://sourcery.mentor.com/sgpp/lite/arm/portal/package8736/public/arm-none-eabi/arm-2011.03-42-arm-none-eabi.bin

echo "################################################################"
echo "Codebench has a problem with dash, please select no"
echo "################################################################"
sudo dpkg-reconfigure -plow dash
ls -lh /bin/sh

echo "################################################################"
echo "Starting the installer, follow the instructions"
echo "################################################################"
/bin/sh arm-2011.03-42-arm-none-eabi.bin
echo "################################################################"
echo "Adding toolchain to PATH"
echo "################################################################"
echo "export PATH=$HOME/CodeSourcery/Sourcery_G++_Lite/bin:$PATH" >> ~/.bashrc

echo "################################################################"
echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
echo "Installation is complete "
echo "Please run the following command in your terminal"
echo "source ~/.bashrc"
echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
echo "################################################################"


#fi