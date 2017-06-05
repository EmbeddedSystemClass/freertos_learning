volatile unsigned int * const UART0DR = (unsigned int *)0x4000C000;

void print_uart0(const char *s) {
	while(*s != '\0') { /* Loop until end of string */
		*UART0DR = *s; /* Transmit char */
		s++; /* Next char */
	}
}

void add() {
	print_uart0("\r15");
}

void c_entry() {
	for(;;) {
		add();
		print_uart0("Hello world!\n");
	}
}
