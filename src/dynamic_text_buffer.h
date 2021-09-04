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
void insert_line(DynamicTextBuffer* txt, int n, char* line) ;
void replace_line(DynamicTextBuffer* txt, int n, char* line) ;
void append_line(DynamicTextBuffer* txt, char* line) ;
void delete_line(DynamicTextBuffer* txt, int n) ;
void delete_range(DynamicTextBuffer* txt, int start, int stop) ;
void show_numbered(DynamicTextBuffer* txt, int start, int stop) ;
void load_file(DynamicTextBuffer* txt, const char* filepath) ;
void dump_file(DynamicTextBuffer* txt, const char* filepath) ;

#endif // DYNAMIC_TEXT_BUFFER_H
