CC = gcc-9
CFLAGS = -g -lpmemobj -lpmem
DEPS = layout.h


default: write read tx_write tx_read rect_calc

write: $(DEPS) writer.c
	$(CC) -std=gnu99 writer.c -o write $(CFLAGS)

read: $(DEPS) reader.c
	$(CC) -std=gnu99 reader.c -o read $(CFLAGS)

tx_write: $(DEPS) tx_writer.c
	$(CC) -std=gnu99 tx_writer.c -o tx_write $(CFLAGS)

tx_read: $(DEPS) tx_reader.c
	$(CC) -std=gnu99 tx_reader.c -o tx_read $(CFLAGS)

rect_calc: rect_calc.c
	$(CC) -std=gnu99 rect_calc.c -o rect_calc $(CFLAGS)

clean:
	rm write read tx_write tx_read rect_calc
