# freertos_learning
```
$ arm-linux-gnueabi-gcc -mcpu=cortex-a8   -c -o test.o test.c
$ arm-linux-gnueabi-as -mcpu=cortex-a8  -o startup.o startup.s
$ arm-linux-gnueabi-gcc -nostdlib -T test.ld -Xlinker "--build-id=none"  test.o startup.o   -o test
```

https://balau82.wordpress.com/2010/12/05/using-ubuntu-arm-cross-compiler-for-bare-metal-programming/
