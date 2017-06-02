INCLUDEFLAG = -I
FREERTOS_SRC = freertos_learning/FreeRTOS/
INC_FREERTOS = $(FREERTOS_SRC)include/
FREERTOS_MEMMANG_SRC = $(FREERTOS_SRC)portable/MemMang/
APP_SRC = freertos_learning/src/
#  Directory with platform specific source files
FREERTOS_PORT_SRC = $(FREERTOS_SRC)portable/GCC/ARM_CM3/
INC_FLAGS = $(INCLUDEFLAG)$(INC_FREERTOS) $(INCLUDEFLAG)$(FREERTOS_PORT_SRC) $(INCLUDEFLAG)$(APP_SRC)
all:
	arm-none-eabi-gcc -mthumb -march=armv7 -mfix-cortex-m3-ldrd -T lm3s6965.ld main.c reset.S syscalls.c -o main
	arm-none-eabi-objcopy -O binary main main.bin
	arm-none-eabi-gcc -mthumb -march=armv7 -mfix-cortex-m3-ldrd $(INC_FLAGS) -T lm3s6965.ld $(APP_SRC)test.c reset.S syscalls.c $(FREERTOS_SRC)list.c $(FREERTOS_SRC)tasks.c $(FREERTOS_PORT_SRC)port.c $(FREERTOS_MEMMANG_SRC)heap_1.c -o main_rtos
test_hello:
	qemu-system-arm -M lm3s6965evb --kernel main.bin --serial stdio

clean:
	rm main
	rm *.bin
	rm main_rtos
	
