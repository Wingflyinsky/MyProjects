#include "globals.h"
#include "util.h"
#include "scan.h"

int lineno; 

typedef enum
{START,INNUM,INID,INCOMPARE,INSPECIAL,INCOMMENT,INCHECK,DONE}
StateType;

#define MAXTOKENLEN 255

char tokenString[MAXTOKENLEN+1];

#define BUFLEN 256 
#define EOF_FILE -1

static char lineBuf[BUFLEN];
static int linepos = 0;
static int bufsize = 0;
static int EOF_flag = FALSE;

static int getNextChar() {
	if (linepos >= bufsize) {
		lineno++;
		if (fgets(lineBuf, BUFLEN - 1, source)) {
			if (EchoSource)fprintf(listing, "%4d:%s", lineno, lineBuf);
			bufsize = strlen(lineBuf);
			linepos = 0;
			return lineBuf[linepos++];
		}
		else {
			EOF_flag = TRUE;
			return EOF;
		}
	}
	else return lineBuf[linepos++];
}

static void ungetNextChar() {

	if (!EOF_flag)linepos--;
}

static struct {
	const char* str;
	TokenType tok;
}reservedWords[MAXRESERVED]
= { {"if",IF},{"else",ELSE},{"int",INT},{"return",RETURN},{"void",VOID},{"while",WHILE} };

static TokenType reservedLookup(char *s)
{
	int i;
	for (i = 0; i < MAXRESERVED; i++)
		if (!strcmp(s, reservedWords[i].str)) {      //strcmp函数返回0时，代表相等，则返回对应token
			return reservedWords[i].tok;
		}
	return ID;
}

TokenType getToken(void)
{  /* index for storing into tokenString */
	int tokenStringIndex = 0;
	/* holds current token to be returned */
	TokenType currentToken;
	/* current state - always begins at START */
	StateType state = START;
	/* flag to indicate save to tokenString */
	int save;
	char comflag; //判断比较符号
	while (state != DONE)
	{
		int c = getNextChar();
		save = TRUE;
		switch (state)
		{
		case START:
			if (isdigit(c))
				state = INNUM;
			else if (isalpha(c))
				state = INID;
			else if (c == '<' || c == '>' || c == '=' || c == '!') {
				state = INCOMPARE;
				comflag = c;
			}
			else if ((c == ' ') || (c == '\t') || (c == '\n'))
				save = FALSE;
			else if (c == '/')
			{
				save = FALSE;
				state = INSPECIAL;
			}
			else
			{
				state = DONE;
				switch (c)
				{
				case EOF_FILE:
					save = FALSE;
					currentToken = ENDFILE;
					break;
				case '+':
					currentToken = PLUS;
					break;
				case '-':
					currentToken = MINUS;
					break;
				case '*':
					currentToken = TIMES;
					break;
				case '(':
					currentToken = LPAREN;
					break;
				case ')':
					currentToken = RPAREN;
					break;
				case '[':
					currentToken = LBRACKET;
					break;
				case ']':
					currentToken = RBRACKET;
					break;
				case '{':
					currentToken = LBRACE;
					break;
				case '}':
					currentToken = RBRACE;
					break;
				case ';':
					currentToken = SEMI;
					break;
				case ',':
					currentToken = COMMA;
					break;
				default:
					currentToken = ERROR;
					break;
				}
			}
			break;
		case INSPECIAL:
			save = FALSE;
			if (c == '*')
				state = INCOMMENT;
			else
			{ /* backup in the input */
				ungetNextChar();
				save = FALSE;
				state = DONE;
				currentToken = OVER;
			}
			break;
		case INCOMMENT:
			save = FALSE;
			if (c == '*')
			{
				state = INCHECK;
			}
			else if (c == EOF)
			{
				state = DONE;
				currentToken = ENDFILE;
			}
			break;
		case INCHECK:
			save = FALSE;
			if (c == '/')
			{
				state = START;
			}
			else if (c == EOF)
			{
				state = DONE;
				currentToken = ENDFILE;
			}
			else if(c!='*')
			{
				state = INCOMMENT;
			}
			break;
		case INCOMPARE:
			state = DONE;
			if (c == '=') {
				switch (comflag)
				{
				case '<':
					currentToken = LE;
						break;
				case '>':
					currentToken = GE;
						break;
				case '=':
					currentToken = EQ;
						break;
				case '!':
					currentToken = NE;
						break;
				}
			}	
			else
			{ 
				ungetNextChar();
				save = FALSE;
				switch (comflag)
				{
				case '<':
					currentToken = LT;
						break;
				case '>':
					currentToken = GT;
						break;
				case '=':
					currentToken = ASSIGN;
						break;
				case '!':
					currentToken = ERROR;
						break;
				}
			}
			break;
		case INNUM:
			if (!isdigit(c))
			{ /* backup in the input */
				ungetNextChar();
				save = FALSE;
				state = DONE;
				currentToken = NUM;
			}
			break;
		case INID:
			if (!isalpha(c))
			{ /* backup in the input */
				ungetNextChar();
				save = FALSE;
				state = DONE;
				currentToken = ID;
			}
			break;
		case DONE:
		default: /* should never happen */
			fprintf(listing, "Scanner Bug: state= %d\n", state);
			state = DONE;
			currentToken = ERROR;
			break;
		}
		if ((save) && (tokenStringIndex <= MAXTOKENLEN))
			tokenString[tokenStringIndex++] = (char)c;
		if (state == DONE)
		{
			tokenString[tokenStringIndex] = '\0';
			if (currentToken == ID)
				currentToken = reservedLookup(tokenString);
		}
	}
	if (TraceScan) {
		fprintf(listing, "\t%4d: ", lineno);
		printToken(currentToken, tokenString);
	}
	return currentToken;
} /* end getToken */



