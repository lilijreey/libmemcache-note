OBJECTS=test
all: 

test: test.c
	gcc -Wall $^ -lmemcached  -o $@

clear:
	@rm -rf $(OBJECTS)
