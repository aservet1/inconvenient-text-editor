#ifndef PANIC_H
#define PANIC_H

#include <stdio.h>
#include <stdlib.h>

#define panic(format, args...)    \
do{                               \
	char msg[1024];               \
	sprintf(msg, format, ##args); \
	fprintf (                     \
		stderr,                   \
		"PANIC message:"          \
		"\n]] %s"                 \
		"\n....]]_line___%d"      \
		"\n....]]_method_%s"      \
		"\n",                     \
		msg, __LINE__, __func__   \
	);                            \
	exit(1);                      \
} while(0);                       \

#endif // ifndef PANIC_H
