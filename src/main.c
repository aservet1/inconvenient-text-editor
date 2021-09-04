#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "dynamic_text_buffer.h"
#include "colors.h"

#define streq(s1,s2) !strcmp(s1,s2)

typedef struct cmd_t {
	char verb[BUFSIZ];
	char args[BUFSIZ];
} cmd_t;

static char* whitespace = " \n";

void
print_command(cmd_t cmd) {
	printf("verb: '%s'\nargs: '%s'\n", cmd.verb, cmd.args);
}

char*
gets(char *buf, int max) {
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

int
is_in(char c, char* str) {
	for(int i = 0; i < strlen(str); i++)
		if (c == str[i]) return 1;
	return 0;
}

int
iswhitespace(char c) {
	return is_in(c,whitespace);
}

char* strip(char* buf) {
	int i;
	i=strlen(buf)-1;
	while (1) {
		if (i == -1) break;
		if (!iswhitespace(buf[i])) break;
		i--;
	}
	buf[++i] = '\0';
	while(1) {
		if (
			!iswhitespace(*buf)
		) break;
		buf++;
	}
	return buf;
}

void
first_rest(char* src, char* first, char* rest) {
	src = strip(src);
	sscanf(src,"%s",first);
	int offset = strlen(first)+1;
	int i; for (i=0; src[i+offset]; i++) rest[i]=src[i+offset];
	rest[i]='\0';
}

void
eval(cmd_t cmd, DynamicTextBuffer* txt) {
	if (streq(cmd.verb,"show")){
		int start, stop;
		if (streq(cmd.args,"all") || strlen(cmd.args)==0 )
			start = stop = -1;
		else
			sscanf(cmd.args,"%d %d",&start,&stop);
		show_numbered(txt, start, stop);
	} else if (streq(cmd.verb,"insert")) {
		char line[BUFSIZ], line_number_buf[BUFSIZ];
		first_rest(cmd.args,line_number_buf,line);
		int line_number = atoi(line_number_buf); // TODO do the version of atoi that responds to invalid number input
		insert_line(txt,line_number,line);
	} else if (streq(cmd.verb,"append")) {
		char line[BUFSIZ]; strcpy(line,cmd.args);
		append_line(txt, line);
	} else if (streq(cmd.verb,"replace")) {
		char line[BUFSIZ], line_number_buf[BUFSIZ];
		first_rest(cmd.args,line_number_buf,line);
		int line_number = atoi(line_number_buf); // TODO do the version of atoi that responds to invalid number input
		replace_line(txt, line_number, line);
	} else if (streq(cmd.verb,"delete")) {
		int line_number = atoi(cmd.args); // TODO do the version of atoi that responds to invalid number input
		delete_line(txt, line_number);
	} else if (streq(cmd.verb,"deleterange")) {
		int start, stop;
		sscanf(cmd.args,"%d-%d",&start,&stop);
		delete_range(txt,start,stop);
	} else if (streq(cmd.verb,"save")) {
		char path[BUFSIZ];
		strcpy(path,cmd.args);
		if (!strlen(path)) printf("no save path provided\n");
		else dump_file(txt, path);
	}
}

void
prompt() {
	printf(GRN"editorcmd>> "RESET);
}

cmd_t
parsecmd(char* input) {
	cmd_t cmd;
	first_rest(input,cmd.verb,cmd.args);
	return cmd;
}

void repl(DynamicTextBuffer* main_txt) {
	setbuf(stdout,NULL);
	char* input = malloc(BUFSIZ*sizeof(char));
	while(1) {
		prompt(); gets(input, BUFSIZ);
		input = strip(input);
		if (streq(input,"exit"))
			break;
		eval(parsecmd(input), main_txt);
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

	repl(main_txt);

	freeDynamicTextBuffer(main_txt);
	main_txt = NULL;

	return 0;
}

