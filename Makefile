all:
	cd hello_world && $(MAKE) all
	cd freertos_learning_2 && $(MAKE) all

build_hello:
	cd hello_world && $(MAKE) all

build_qemu:
	cd freertos_learning_2 && $(MAKE) all

test_hello_world:
	cd hello_world && $(MAKE) test_hello

test_rtos:
	cd freertos_learning_2 && $(MAKE) test_qemu

clean:
	cd hello_world && $(MAKE) clean
	cd freertos_learning_2 && $(MAKE) clean