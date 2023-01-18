#include <stdarg.h>
#include <stdio.h>

#include "logging.h"

void setup_logger(FILE* logfile_handle) {
	if(logfile == NULL)
		logfile = logfile_handle;
}

void log(int log_level, char* log_string, ...) {
	switch(log_level) {
	case LOG_DEBUG:
		if(DEBUG_BUILD)
			fprintf(logfile, "DEBUG	: ");
		break;
	case LOG_NORMAL:
		fprintf(logfile, "INFO	: ");
		break;
	case LOG_WARNING:
		fprintf(logfile, "WARNING: ");
		break;
	case LOG_ERROR:
		fprintf(logfile, "ERROR	: ");
		break;

	default: break;
	}

	va_list args;
	va_start(args, log_string);
	vfprintf(logfile, log_string, args);
	va_end(args);
	fprintf(logfile, "\n");
}
