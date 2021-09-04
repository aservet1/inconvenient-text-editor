#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "dynamic_text_buffer.h"
#include "colors.h"

struct cmd {
	char cmd[BUFSIZ];
	char arg[BUFSIZ];
};

char* gets(char *buf, int max) {
	int i, cc;
	char c;
	
	for (i=0; i+i<max; ){
		cc = read(0, &c, 1);
		if(cc < 1 ) break;
		buf[i++] = c;
		if(c == '\n' || c == '\r')
			break;
	}

	buf[i] = '\0';
	return buf;
}

int is_in(char c, char* str) {
	for(int i = 0; i < strlen(str); i++)
		if (c == str[i]) return 1;
	return 0;
}

static char* whitespace = " \n";
int iswhitespace(char c) {
	return is_in(c,whitespace);
}

char* strip(char* buf) {
	int i;
	i=strlen(buf)-1;
	while (1) {
		if (i == 0) break;
		if (!iswhitespace(buf[i])) break;
		i--;
	}
	buf[++i] = '\0';
	for(i=0;iswhitespace(buf[i]);i++,buf++);
	return buf;
}

#define streq(s1,s2) !strcmp(s1,s2)
void evaluate(struct cmd cmd, DynamicTextBuffer* txt) {
	if (streq(cmd.cmd,"show")){

		if (streq(cmd.arg,"all")) {
			show_numbered(txt,-1,-1);
		} else {
			int start, stop;
			sscanf(cmd.arg,"%d-%d",&start,&stop);
			show_numbered(txt, start, stop);
		}

	}
}

void prompt() {
	printf(GRN"editorcmd>> "RESET);
}

struct cmd parsecmd(char* input) {
	struct cmd cmd;
	sscanf(input, "%s %s", cmd.cmd, cmd.arg);
	return cmd;
}

void REPL(DynamicTextBuffer* main_txt) {
	setbuf(stdout,NULL);
	char* input = malloc(BUFSIZ*sizeof(char));
	while(1) {
		prompt(); gets(input, BUFSIZ);
		input = strip(input);
		if (streq(input,"exit"))
			break;
		evaluate(parsecmd(input), main_txt);
	}
	free(input);
}

int main(int argc, const char* argv[]) {

	if (argc != 2) {
		fprintf (
			stderr,
			"usage: %s <source file>\n",
			argv[0]
		);
		exit(2);
	}

	const char* inpath = argv[1];

	DynamicTextBuffer* main_txt = NULL;
	main_txt = mallocDynamicTextBuffer(64);
	load_file(main_txt, inpath);

	REPL(main_txt);

	freeDynamicTextBuffer(main_txt);
	main_txt = NULL;

	return 0;
}

