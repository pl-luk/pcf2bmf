#include <stdio.h>
#include <argp.h>
#include <stdbool.h>
#include <stdlib.h>

#include "config.h"
#include "logging.h"

/* GNU Argparse related definitions */

const char *argp_program_version = "pcf2bmf 1.0.0";

// Define program options
static struct argp_option options[] = {
		{"width",		'w', "DIM", 			0, 	"Control the dimensions of the output atlas (as PNG) [default: 1024x1024]", 0},
		{"height",		'h', "",				OPTION_ALIAS,	"", 0},
		{"atlas",		'n', "FILE",			0, "Specify the filename of the output XML atlas description file and the bitmap [default: font.xml and atlas.png]", 1},
		{"xml",			'x', "",				OPTION_ALIAS,	"", 1},
		{"unicode",		'c', 0, 				0, "Translate unicode to multi-bytes based on the Active Code Page of current OS before querying to PCF for glyph [default: false]", 2},
		{"char",		'i', "FILE",			0, "A text file in UTF-8 listing all needed chars. [default: chars.txt]", 3},
		{"logfile",		'l', "FILE",			0, "A file to store log output [default: stdout]", 4},
		{0}
};

// define argument
static char args_doc[] = "pcf_file.pcf";

// define docstring
static char docstring[] = "Generate a bitmap font (in the format comply to BMFont) from given PCF file";

// argument struct for access
struct arguments {
	int width;
	int height;
	char* atlas_file;
	char* xml_file;
	bool unicode;
	char* char_file;
	char* log_file;
	char* pcf_file;
};

// parsing function
static error_t parse_opt(int key, char* arg, struct argp_state* state) {

	struct arguments* arguments = state->input;
	switch(key) {

	case 'w':
		int width = atoi(arg);

		// atoi outputs 0 if error, also 0 or lower is not allowed
		if(width <= 0) {
			printf("Width should be a number greater than zero.\n");
			argp_usage(state);
			break;
		}

		arguments->width = width;
		break;

	case 'h':
		int height = atoi(arg);

		// atoi outputs 0 if error, also 0 or lower is not allowed
		if(height <= 0) {
			printf("Height should be a number greater than zero.\n");
			argp_usage(state);
			break;
		}

		arguments->height = height;
		break;

	case 'n':
		arguments->atlas_file = arg;
		break;

	case 'x':
		arguments->xml_file = arg;
		break;

	case 'c':
		arguments->unicode = 1;
		break;

	case 'i':
		arguments->char_file = arg;
		break;

	case 'l':
		arguments->log_file = arg;
		break;

	case ARGP_KEY_ARG:

		// handle too many arguments
		if(state->arg_num > 1)
			argp_usage(state);
		arguments->pcf_file = arg;
		break;

	case ARGP_KEY_END:

		//handle not enough arguments
		if(state->arg_num < 1)
			argp_usage(state);
		break;

	default:
		return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

// argp parser struct
static struct argp parser = {options, parse_opt, args_doc, docstring};

/* Utility section */
// Struct that holds all file references
struct file_table {
	FILE* atlas;
	FILE* xml;
	FILE* chars;
	FILE* pcf;
};

static void log_input_args(struct arguments* args) {
	log(LOG_DEBUG, "Supplied arguments:");

	log(LOG_DEBUG, "Width: %d", 			args->width);
	log(LOG_DEBUG, "Height: %d", 			args->height);
	log(LOG_DEBUG, "Atlas: %s", 			args->atlas_file);
	log(LOG_DEBUG, "Xml: %s", 				args->xml_file);
	log(LOG_DEBUG, "Translate unicode: %d", args->unicode);
	log(LOG_DEBUG, "Chars: %s", 			args->char_file);
	log(LOG_DEBUG, "Log: %s", 				args->log_file);
	log(LOG_DEBUG, "Pcf: %s", 				args->pcf_file);
}

int main(int argc, char** argv) {

	// initialize default values
	struct arguments args;

	args.width = 1024;
	args.height = 1024;
	args.atlas_file = "atlas.png";
	args.xml_file = "font.xml";
	args.unicode = 0;
	args.char_file = "chars.txt";
	args.log_file = "stdout";
	args.pcf_file = "";

	argp_parse(&parser, argc, argv, 0, 0, &args);

	// open log_file
	FILE* logfile;
	if(args.log_file == "stdout")
		logfile = stdout;
	else if((logfile = fopen(args.log_file, "w")) == NULL) {
		fprintf(stderr, "Unable to open %s, defaulting to stdout", args.log_file);
		logfile = stdout;
	}

	setup_logger(stdout);
	log_input_args(&args);
	return 0;
}
