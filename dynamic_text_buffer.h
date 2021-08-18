#ifndef DYNAMIC_TEXT_BUFFER_H
#define DYNAMIC_TEXT_BUFFER_H

typedef struct DynamicTextBuffer {
	size_t capacity;
	char** lines;
	int used;
} DynamicTextBuffer;

void debugprint(DynamicTextBuffer* text, FILE* out);

DynamicTextBuffer* mallocDynamicTextBuffer(size_t capacity);
void freeDynamicTextBuffer(DynamicTextBuffer* txt) ;
void flush(DynamicTextBuffer* txt) ;
void insert_line(DynamicTextBuffer* txt, char* line, int n) ;
void append_line(DynamicTextBuffer* txt, char* line) ;
void show_numbered(DynamicTextBuffer* txt) ;
void load_file(DynamicTextBuffer* txt, const char* filename) ;

#endif // DYNAMIC_TEXT_BUFFER_H
