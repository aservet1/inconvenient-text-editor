#include <stdio.h>
#include <stdlib.h>
#include "dynamic_text_buffer.h"

int main(int argc, const char* argv[]) {

	if (argc == 1) {
		fprintf (
			stderr,
			"usage: %s <file name to run the sample text edit commmands on>\n",
			argv[0]
		);
		exit(2);
	}

	DynamicTextBuffer* main_txt = NULL;
	main_txt = mallocDynamicTextBuffer(64);

	load_file( main_txt, argv[1]);
	show_numbered( main_txt); printf("\n");
	int N;
	fscanf(stdin, "%d", &N);
	insert_line( main_txt, "hello", N);
	printf(" ----\n");
	show_numbered( main_txt); printf("\n");

	// debugprint( main_txt, stdout);

	freeDynamicTextBuffer(main_txt);
	main_txt = NULL;

	return 0;
}

