CC := gcc
CFLAGS := -I.
DEPS = limits.h execute.h parser.h
OBJECTS = mysh.o execute.o parser.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mysh: $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean: 
	rm -rf mysh $(OBJECTS)