target = text-editor
cflags = -g
cc = gcc

cfiles = $(target).c dynamic_text_buffer.c

all: $(target).c panic.h
	$(cc) $(cflags) $(cfiles) -o $(target)

clean:
	rm -f $(target)

