CC := gcc
CFLAGS := -I.. -lpthread
DEPS = ../message_queue.h
OBJECTS = message_queue_test.o ../message_queue.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: test
test:
	make message_queue_test
	bash -c '(diff -w <(./message_queue_test) message_queue_test.out && echo "✅ TEST PASSED") || (echo "❌ TEST FAILED")'

message_queue_test: $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean: 
	rm -rf message_queue_test $(OBJECTS)