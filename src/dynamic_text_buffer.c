#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include "panic.h"
#include "colors.h"
#include "dynamic_text_buffer.h"

static void
replace_char(char* buf, char replacee, char replacer) {
	for (int i = 0; i < strlen(buf); i++)
		if (buf[i] == replacee) buf[i] = replacer;
}

void
debugprint(DynamicTextBuffer* txt, FILE* out) {
	fprintf (
		out,
		"{"
		"\n  \"capacity\": %ld,"
		"\n  \"used\": %d,"
		"\n  \"lines\": ["
		"\n    ",
		txt->capacity, txt->used
	);
	int i; for (i = 0; i < txt->used-1; i++) {
		fprintf (
			out,
			"\"%s\",\n    ",
			txt->lines[i]
		);
	}
	fprintf (
		out,
		"\"%s\"\n  ",
		txt->lines[i]
	);
	fprintf(
		out,
		"]"
		"\n}"
		"\n"
	);
}

DynamicTextBuffer*
mallocDynamicTextBuffer(size_t capacity) {
	DynamicTextBuffer* txt = (DynamicTextBuffer*)malloc(sizeof(DynamicTextBuffer));
	txt->capacity = capacity;
	txt->lines = malloc(sizeof(char*)*txt->capacity);
	txt->used = 0;
	return txt;
}
void
freeDynamicTextBuffer(DynamicTextBuffer* txt) {
	for (int i = 0; i < txt->used; i++) {
		free(txt->lines[i]);
		txt->lines[i] = NULL;
	} free(txt->lines);
	   txt->lines=NULL;
	    txt->used = 0;
	     free(txt);
}

void
flush(DynamicTextBuffer* txt) {
	for (int i = 0; i < txt->used; i++)
		free(txt->lines[i]);
	txt->used = 0;
}

void
insert_line(DynamicTextBuffer* txt, char* line, int n) {
	assert(n >= 0 && "you asked to insert the line at a negative index");
	assert(n <= txt->used && "you asked to insert a line beyond the bounds of the array");
	if (txt->used == txt->capacity) {
		txt->capacity *= 2 ;
		txt->lines = realloc (
			txt->lines,
			sizeof(char*) * txt->capacity
		);
	} else if (txt->used > txt->capacity) {
		panic (
			"lines used greater than capacity: %d/%ld",
			txt->used, txt->capacity
		);
	}
	// shift aside a spot

	// insert at spot
	char* copy_line = malloc(sizeof(char*)*(strlen(line)+1));
	strcpy(copy_line, line);
	txt->lines[n] = copy_line;

	txt->used += 1;
}

void
append_line(DynamicTextBuffer* txt, char* line) {
	insert_line(txt, line, txt->used);
}

void
delete_line(DynamicTextBuffer* txt, int n) {
	if (n > -1 || n < txt->used) panic("index out of bounds error: %d", n);
}

void show_numbered(DynamicTextBuffer* txt) {
	const char* prefix = "  %d\t";
	for (int i = 0; i < txt->used; i++) {
		printf("  %d\t%s\n", i, txt->lines[i] );
	}
}

static int
max_line_length(char buf[], int n) { // assume a buffer with a lot of lines delimited by a '\n'
	int max = 0;
	int count = 0;
	int offset = 0;
	while (offset < n) {
		for (int i = offset; i < n && buf[i] != '\n'; i++, count++);
		if (count > max) max = count;
		offset += count + 1;
		count = 0;
	}
	return max;
}

static int
next_line(char* line_buf, char* buf) {
	int i = 0;
	while( buf[i] != '\n' && buf[i] != '\0') {
		line_buf[i] = buf[i];
		i++;
	}
	line_buf[i] = '\0';
	return i;	
}

void
load_file(DynamicTextBuffer* txt, const char* filepath) {
	FILE* fp = fopen(filepath, "r");
	char c;
	fseek(fp, 0L, SEEK_END);
	size_t nbytes = ftell(fp);
	rewind(fp);
	// printf(">> loading file of size %ld bytes\n", nbytes);
	char buf[nbytes]; {
		int i = 0; while ( (c = fgetc(fp)) != EOF ) {
			buf[i++] = c;
		}
	}
	// printf(">> loaded buffer:\n%s\n", buf);
	size_t maxlen = max_line_length(buf, nbytes);
	// printf(">> max_line_length = %ld\n", maxlen);
	char line_buf[maxlen];
	char* scanner = &(buf[0]);
	while (true) {
		scanner += next_line(line_buf, scanner);
		if (*scanner == '\0') {
			append_line(txt, line_buf);
			break;
		}
		else if (*buf = '\n') {
			append_line(txt, line_buf);
			scanner++;
			continue;
		}
		else {
			panic("expected '\\n' or '\\0' but found: %c", *scanner);
		}
	}
	fclose(fp);
}

void
dump_file(DynamicTextBuffer* txt, const char* filepath) {
	FILE* fp = 
		(filepath != NULL)
		? fopen(filepath, "w")
		: NULL ;
	

	for (int i = 0; i < txt->used; i++) {
		fprintf(fp ? fp : stdout, "%s\n", txt->lines[i]);
	}

	if (filepath != NULL) {
		fclose(fp);
	}
}
