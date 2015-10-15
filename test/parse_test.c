#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "util.h"

#define TEST_MODE
#include "command.h"

enum {
	BUFSIZE   = 128,
};

int main()
{
	char *cp, lbuf[BUFSIZE];

	while (fgets(lbuf, BUFSIZE, stdin) != NULL) {
		if ((cp = strchr(lbuf, '\n')) != NULL)
			*cp = '\0';

		command_send(lbuf);
	}

	return 0;
}
