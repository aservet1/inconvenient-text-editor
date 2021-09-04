target = text-editor
cflags = -g
cc = gcc

cfiles = src/main.c src/dynamic_text_buffer.c
hfiles = src/colors.h src/dynamic_text_buffer.h src/panic.h

all:
	mkdir -p bin
	$(cc) $(cflags) $(cfiles) -o bin/$(target)

clean:
	rm -rfv bin/

