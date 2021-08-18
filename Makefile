target = text-editor
cflags = -g
cc = gcc

cfiles = src/$(target).c src/dynamic_text_buffer.c

all:
	mkdir -p bin
	$(cc) $(cflags) $(cfiles) -o bin/$(target)

clean:
	rm -rfv bin/

