CC := gcc
CFLAGS := -I. -lpthread
DEPS = message_queue.h macros.h
OBJECTS = message_queue.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: default
default:
	make producer consumer

producer: producer.o $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

consumer: consumer.o $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean: 
	rm -rf producer consumer producer.o consumer.o $(OBJECTS)