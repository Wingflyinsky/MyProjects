#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"
#define NO_PARSE FALSE

int main()
{
	EchoSource = TRUE;
	TraceScan = TRUE;
	Error = FALSE;
	TraceParse = TRUE;
	source = fopen("source.txt", "r");
	listing = fopen("listing.txt", "w+");
	lineno = 0;
	TreeNode * syntaxTree;
#if NO_PARSE
	while (getToken() != ENDFILE);
#else
	syntaxTree = parse();
	if (TraceParse) {
		fprintf(listing, "\nSyntax tree:\n");
		printTree(syntaxTree);
	}
#endif
	fclose(source);
	fclose(listing);
	return 0;
}