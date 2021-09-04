#include <stdio.h>
#include <string.h>

typedef struct cmd_t {
	char verb[BUFSIZ];
	char args[BUFSIZ];
} cmd_t;

// ----------------------------------------------
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
		if (i == -1) break;
		if (!iswhitespace(buf[i])) break;
		i--;
	}
	buf[++i] = '\0';
	while(1) {
		if(!iswhitespace(*buf)) break;
		buf++;
	}
	return buf;
}
// ----------------------------------------------

cmd_t
parsecmd(char* input) {
	cmd_t cmd;

	input = strip(input);
	sscanf(input, "%s", cmd.verb);
	int offset = strlen(cmd.verb)+1;

	int i;
	for (i = 0; input[i+offset] != '\0'; i++) {
		cmd.args[i] = input[i+offset];
	}
	cmd.args[i] = '\0';

	return cmd;
}

void
print_command(cmd_t cmd) {
	printf("verb: '%s'\nargs: '%s'\n", cmd.verb, cmd.args);
}

int main(int argc, const char* argv[]) {

	char input[BUFSIZ];
	int offset = 0;
	for (int i = 1; i < argc; i++)
		offset += sprintf(input + offset, "%s ", argv[i]);

	cmd_t cmd = parsecmd(input);
	print_command(cmd);

	return 0;
}

