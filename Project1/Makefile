CC := gcc
CFLAGS := -I.
DEPS = 
OBJECTS = mysh.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mysh: $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean: 
	rm -rf mysh $(OBJECTS)