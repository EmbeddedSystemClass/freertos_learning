build_hello:
	cd hello_world && $(MAKE) all

test_hello_world:
	cd hello_world && $(MAKE) test_hello

clean:
	cd hello_world && $(MAKE) clean
