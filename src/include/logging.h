#ifndef _H_LOGGING
#define _H_LOGGING

#include <stdlib.h>

#include "config.h"

#define LOG_DEBUG 	0
#define LOG_NORMAL 	1
#define LOG_WARNING	2
#define LOG_ERROR	3

static FILE* logfile = NULL;

void setup_logger(FILE*);
void log(int, char*, ...);

#endif
