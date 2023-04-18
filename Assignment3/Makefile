CC := gcc
CFLAGS := -I.
DEPS = 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: default
default:
	make producer consumer

producer: producer.o
	$(CC) -o $@ $^ $(CFLAGS)

consumer: consumer.o
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean: 
	rm -rf producer producer.o consumer consumer.o