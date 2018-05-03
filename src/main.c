#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <lexer.h>
#include <parser.h>
#include "symbol_table.h"

extern int yydebug;

static void usage (const char const *s_program) {
    printf("\nusage : %s [OPTIONS]\n\n"
	   "OPTIONS :\n"
	   "-f : a source file to compile.\n"
	   "By default, the program takes his input on stdin.\n"
	   "-o : an output file.\n"
	   "By default, the program writes his output on stdout.\n\n"
	   , s_program);
    exit(EXIT_FAILURE);
}

int main (int argc, char *argv[]) {
    FILE *fdout = stdout;
    FILE *fdin  = stdin;
    int c;
    while ((c = getopt(argc, argv, "f:o:")) > -1) {
	switch (c) {
	case 'f' :
	    fdin = fopen(optarg, "r");
	    if (fdin == NULL) {
		fprintf(stderr, "An error occured while opening input file %s !\n", optarg);
	        return EXIT_FAILURE;
	    }
	    break ;

	case 'o' :
	    fdout = fopen(optarg, "w");
	    if (fdout == NULL) {
		fprintf(stderr, "An error occured while creating output file %s !\n", optarg);
		return EXIT_FAILURE;
	    }
	    break ;

	case '?' :
	    if (optopt == 'f' || optopt == 'o') {
		fprintf (stderr, "Option -%c requires an argument.\n", optopt);
	    } else if (isprint (optopt)) {
		fprintf (stderr, "Unknown option `-%c'.\n", optopt);
	    } else {
		fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
	    }
	    usage(argv[0]);
	    break ;

	default : break ;
	}
    }

    ST_create_symbol_table();

    yyin = fdin;
    yyout = fdout;

    int status = yyparse();

    fclose(fdin);
    fclose(fdout);

    ST_free_symbol_table();
    return (status == 0) ? EXIT_SUCCESS : EXIT_FAILURE ;
}
