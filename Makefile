all:
	arm-none-eabi-gcc -Wall -O2 -nostdlib -nostartfiles -ffreestanding -mcpu=cortex-m0  -mthumb -o  test.o -c test.c                                
	arm-none-eabi-as --warn -mcpu=cortex-m0 startup.s -o startup.o
	arm-none-eabi-ld -o uart01.elf -T test.ld startup.o test.o
	arm-none-eabi-objcopy uart01.elf uart01.bin -O binary