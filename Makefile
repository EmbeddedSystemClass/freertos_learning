INCLUDEFLAG = -I
FREERTOS_SRC = FreeRTOS/
INC_FREERTOS = $(FREERTOS_SRC)include/
FREERTOS_MEMMANG_SRC = $(FREERTOS_SRC)portable/MemMang/
APP_SRC = src/
#  Directory with platform specific source files
FREERTOS_PORT_SRC = $(FREERTOS_SRC)portable/GCC/ARM_CM0/
INC_FLAGS = $(INCLUDEFLAG)$(INC_FREERTOS) $(INCLUDEFLAG)$(FREERTOS_PORT_SRC) $(INCLUDEFLAG)$(APP_SRC)
all:
#  Main file compile
	arm-linux-gnueabi-gcc -Wall -O2  -nostartfiles -ffreestanding -mcpu=cortex-m0  -mthumb $(INC_FLAGS) -o  test.o -c $(APP_SRC)test.c     
#  Free RTOS library
	arm-linux-gnueabi-gcc -Wall -O2  -nostartfiles -ffreestanding -mcpu=cortex-m0  -mthumb $(INC_FLAGS) -o  list.o -c $(FREERTOS_SRC)list.c 
	arm-linux-gnueabi-gcc -Wall -O2  -nostartfiles -ffreestanding -mcpu=cortex-m0  -mthumb $(INC_FLAGS) -o  tasks.o -c $(FREERTOS_SRC)tasks.c 
	arm-linux-gnueabi-gcc -Wall -O2  -nostartfiles -ffreestanding -mcpu=cortex-m0  -mthumb $(INC_FLAGS) -o  timers.o -c $(FREERTOS_SRC)timers.c 
	arm-linux-gnueabi-gcc -Wall -O2  -nostartfiles -ffreestanding -mcpu=cortex-m0  -mthumb $(INC_FLAGS) -o  croutine.o -c $(FREERTOS_SRC)croutine.c 
	arm-linux-gnueabi-gcc -Wall -O2  -nostartfiles -ffreestanding -mcpu=cortex-m0  -mthumb $(INC_FLAGS) -o  event_groups.o -c $(FREERTOS_SRC)event_groups.c 
#  Port file
	arm-linux-gnueabi-gcc -Wall -O2  -nostartfiles -ffreestanding -mcpu=cortex-m0  -mthumb $(INC_FLAGS) -o  port.o -c $(FREERTOS_PORT_SRC)port.c 
#  Memmang file
	arm-linux-gnueabi-gcc -Wall -O2  -nostartfiles -ffreestanding -mcpu=cortex-m0  -mthumb $(INC_FLAGS) -o  heap_1.o -c $(FREERTOS_MEMMANG_SRC)heap_1.c 
#  Assembler code and linking
	arm-linux-gnueabi-as --warn -mcpu=cortex-m0 startup.s -o startup.o

	arm-linux-gnueabi-ld -o freertos_cortexm0.elf -T test.ld startup.o test.o list.o tasks.o  croutine.o event_groups.o port.o heap_1.o
	arm-linux-gnueabi-objcopy freertos_cortexm0.elf freertos_cortexm0.bin -O binary

clean:
	rm *.o
	rm *.bin