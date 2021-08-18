#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynamic_text_buffer.h"
#include "colors.h"

int main(int argc, const char* argv[]) {

	if (argc != 3) {
		fprintf (
			stderr,
			"usage: %s <file name to read from> <file name to dump to>\n",
			argv[0]
		);
		exit(2);
	}

	const char* inpath = argv[1];
	const char* outpath = 
		strcmp (
			argv[2],
			"stdout"
		)
		? argv[2]
		: NULL;

	DynamicTextBuffer* main_txt = NULL;
	main_txt = mallocDynamicTextBuffer(64);

	load_file( main_txt, inpath);
	show_numbered( main_txt); printf("\n");
	int N;
	fscanf(stdin, "%d", &N);
	insert_line( main_txt, GRN"hello"RESET, N);
	printf(" ----\n");
	show_numbered( main_txt); printf("\n");

	dump_file( main_txt, outpath);

	freeDynamicTextBuffer(main_txt);
	main_txt = NULL;

	return 0;
}

