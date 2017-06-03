#  Source file
src = src/
#  Linker and assembler desgined for emulating RSL10 Ram and Flash
qemu = qemu/
#  Binary files
bin = bin/

all:
	arm-none-eabi-gcc -mthumb -march=armv7 -mfix-cortex-m3-ldrd -T $(qemu)lm3s6965.ld $(src)main.c $(qemu)reset.S $(src)syscalls.c -o $(bin)main
	arm-none-eabi-objcopy -O binary $(bin)main $(bin)main.bin
	
test_hello:
	qemu-system-arm -M lm3s6965evb -m 88K --kernel $(bin)main.bin --serial stdio

clean:
	rm $(bin)*

	
