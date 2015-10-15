/* logging functions */
/*
enum loglevel_t {
	DEBUG = 0,
	INFO,
	WARN,
	ERROR,
	FATAL,
	LOG_LEVEL = DEBUG
};

void logging(enum loglevel_t loglevel, char *format, ...)
{
	va_list arg;
	static const char *loglevel2str[] = {
		[DEBUG] = "DEBUG",
		[WARN]  = "WARN",
		[ERROR] = "ERROR",
		[FATAL] = "FATAL"
	};

	// debug message is available on verbose mode
	if (loglevel < LOG_LEVEL)
		return;

	fprintf(stderr, ">>%s<<\t", loglevel2str[loglevel]);

	va_start(arg, format);
	vfprintf(stderr, format, arg);
	va_end(arg);
}
*/

/* parse_arg functions */
enum parse_arg_misc_t {
	MAX_ARGS = 128,
};

struct parm_t {
    int argc;
    char *argv[MAX_ARGS];
};

void reset_parm(struct parm_t *pt)
{
    pt->argc = 0;
    for (int i = 0; i < MAX_ARGS; i++)
        pt->argv[i] = NULL;
}

void add_parm(struct parm_t *pt, char *cp)
{
    if (pt->argc >= MAX_ARGS)
        return;

    //logging(DEBUG, "argv[%d]: %s\n", pt->argc, (cp == NULL) ? "NULL": cp);

    pt->argv[pt->argc] = cp;
    pt->argc++;
}

void parse_arg(char *buf, struct parm_t *pt, int delim, int (is_valid)(int c))
{
    /*
        v..........v d           v.....v d v.....v ... d
        (valid char) (delimiter)
        argv[0]                  argv[1]   argv[2] ...   argv[argc - 1]
    */
    size_t length;
    char *cp, *vp;

    if (buf == NULL)
        return;

    length = strlen(buf);
    //logging(DEBUG, "parse_arg() length:%u\n", (unsigned) length);

    vp = NULL;
    for (size_t i = 0; i < length; i++) {
        cp = buf + i;

        if (vp == NULL && is_valid(*cp))
            vp = cp;

        if (*cp == delim) {
            *cp = '\0';
            add_parm(pt, vp);
            vp = NULL;
        }

        if (i == (length - 1) && (vp != NULL || *cp == '\0'))
            add_parm(pt, vp);
    }

    //logging(DEBUG, "argc:%d\n", pt->argc);
}

/* other functions */
int my_ceil(int val, int div)
{
    if (div == 0)
        return 0;
    else
        return (val + div - 1) / div;
}

int dec2num(char *str)
{
    if (str == NULL)
        return 0;

    return strtol(str, NULL, 10);
}

int hex2num(char *str)
{
    if (str == NULL)
        return 0;

    return strtol(str, NULL, 16);
}

int sum(struct parm_t *parm)
{
    int sum = 0;

    for (int i = 0; i < parm->argc; i++)
        sum += dec2num(parm->argv[i]);

    return sum;
}
