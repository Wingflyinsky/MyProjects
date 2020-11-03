#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

static TokenType token; /* holds current token */

/* function prototypes for recursive calls */
static TreeNode * declaration_list(void);
static TreeNode * declaration(void);
static TreeNode * params(void);
static TreeNode * param_list(void);
static TreeNode * param(void);
static TreeNode * compound_stmt(void);
static TreeNode * local_declarations(void);
static TreeNode * statement_list(void);
static TreeNode * statement(void);
static TreeNode * expression_stmt(void);
static TreeNode * selection_stmt(void);
static TreeNode * iteration_stmt(void);
static TreeNode * return_stmt(void);
static TreeNode * expression(void);
static TreeNode * simple_expression(void);
static TreeNode * additive_expression(void);
static TreeNode * term(void);
static TreeNode * factor(void);
static TreeNode * args(void);
static TreeNode * arg_list(void);

static void syntaxError(char * message)
{ fprintf(listing,"\n>>> ");
  fprintf(listing,"Syntax error at line %d: %s",lineno,message);
  Error = TRUE;
}

static void match(TokenType expected)
{ if (token == expected) token = getToken();
  else {
    syntaxError("unexpected token -> ");
    printToken(token,tokenString);
    fprintf(listing,"      ");
  }
}

TreeNode * declaration_list(void) {
	TreeNode * t = declaration();
	TreeNode * p = t;
	while (token != ENDFILE)
	{
		TreeNode * q;
		q = declaration();
		if (q != NULL) {
			if (t == NULL) t = p = q;
			else /* now p cannot be NULL either */
			{
				p->sibling = q;
				p = q;
			}
		}
	}
	return t;
}

/*语句节点*/
TreeNode * declaration(void)
{
	TreeNode * t = newStmtNode(DecK);
	if (token == INT ) {
		TreeNode * p = newStmtNode(IntK);
		if (t != NULL) {
			t->child[0] = p;
			match(INT);
		}
		
	}
	else if(token == VOID) {
		TreeNode * p = newStmtNode(VoidK);
		t->child[0] = p;
		match(VOID);
	}
	else {
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
		return t;        
	}
	t->child[1] = newExpNode(IdK);
	t->child[1]->attr.name = copyString(tokenString);
	match(ID);
	switch (token) {
	case LBRACKET:
		match(LBRACKET);		 //变量声明语句
		if (token == NUM) {
			t->child[2] = newExpNode(IndexK);
			if (t != NULL) 
				t->child[2]->attr.val = atoi(tokenString);
			match(NUM);
			match(RBRACKET);
		}
		break;
	case LPAREN:                 //函数声明语句
		match(LPAREN);
		t->kind.stmt = FunK;
		t->child[2] = params();
		match(RPAREN);
		t->child[3] = compound_stmt();
		break;
	}
	return t;
}

/*参数块节点*/
TreeNode * params(void) {
	TreeNode *t = newStmtNode(ParamK);
	if (token == VOID) {
		t->child[0] = newStmtNode(VoidK);
		match(VOID);
	}
	else {
		t->child[0] = param_list();
	}
	return t;
}

/*参数列表节点*/
TreeNode * param_list(void) {
	TreeNode * t = param();
	TreeNode * p = t;
	while (token != RPAREN)
	{
		TreeNode * q;
		match(COMMA);
		q = param();
		if (q != NULL) {
			if (t == NULL) t = p = q;
			else /* now p cannot be NULL either */
			{
				p->sibling = q;
				p = q;
			}
		}
	}
	return t;
}

/*单个参数节点*/
TreeNode * param(void) {
	TreeNode *t = NULL;
	if (token == INT) {  //此处不应该出现void
		t = newStmtNode(IntK);
		match(INT);
		t->child[0] = newExpNode(IdK);
		t->child[0]->attr.name = copyString(tokenString);
		match(ID);
		if (token == LBRACKET) {
			match(LBRACKET);
			match(RBRACKET);
		}
	}
	else {
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
	}
	return t;
}

/*语句块节点*/
TreeNode * compound_stmt(void) {  
	match(LBRACE);
	TreeNode *t = newStmtNode(ComK);
	t->child[0] = local_declarations();
	t->child[1] = statement_list();
	match(RBRACE);
	return t;
}

/*局部变量声明语句块节点*/
TreeNode * local_declarations(void) {
	TreeNode * t = NULL;
	if (token == INT) {
		t = newStmtNode(IntK);
	}
	else {
		t = newStmtNode(LocK);
	}
	TreeNode * p = t;
	TreeNode * q = NULL;
	while (token == INT)
	{
		q = newStmtNode(IntK);
		if (q != NULL) {
			match(INT);
			q->child[0] = newExpNode(IdK);
			q->child[0]->attr.name = copyString(tokenString);
			match(ID);
			if (token == LBRACKET) {
				match(LBRACKET);
				q->child[1] = newExpNode(ConstK);
				q->child[1]->attr.val = atoi(tokenString);
				match(NUM);
				match(RBRACKET);
			}
			p->sibling = q;
			p = q;
		}
	}
	return t;
}

TreeNode * statement_list(void)
{
  TreeNode * t = newStmtNode(StaK);
  TreeNode * p = t;
  TreeNode * q;
  while ((token==IF) || (token==WHILE) || (token == RETURN) || (token == ID) || (token == SEMI) || (token==LBRACE) || (token == LPAREN) || (token == NUM) )
  { 
    q = statement();
    if (q!=NULL) {
       p->sibling = q;
       p = q;
    }
  }
  return t;
}

TreeNode * statement(void)
{ TreeNode * t = NULL;
  switch (token) {
    case IF : t = selection_stmt(); break;
	case SEMI :case ID: case LPAREN: case NUM:
		t = expression_stmt(); break;
    case LBRACE : t = compound_stmt(); break;
    case WHILE : t = iteration_stmt(); break;
    case RETURN : t = return_stmt(); break;
    default : syntaxError("unexpected token -> ");
              printToken(token,tokenString);
              token = getToken();
              break;
  } /* end case */
  return t;
}

TreeNode * expression_stmt(void) {
	TreeNode * t = newStmtNode(ExpK);
	if (token == ID || token == LBRACE || token == NUM) {
		t = expression();
	}
	match(SEMI);
	return t;
}


TreeNode * selection_stmt(void)
{ TreeNode * t = newStmtNode(IfK);
  match(IF);
  match(LPAREN);
  if (t!=NULL) t->child[0] = expression();
  match(RPAREN);
  if (t!=NULL) t->child[1] = statement();
  if (token==ELSE) {
	  t->child[2] = newStmtNode(ElseK);
    match(ELSE);
    if (t!=NULL) t->child[3] = statement();
  }
  return t;
}

TreeNode * iteration_stmt(void) {
	TreeNode * t = newStmtNode(IterK);
	match(WHILE);
	match(LPAREN);
	t->child[0] = expression();
	match(RPAREN);
	t->child[1] = statement();
	return t;
}

TreeNode * return_stmt(void)
{ TreeNode * t = newStmtNode(ReturnK);
  match(RETURN);
  if (token == ID || token == LBRACE || token == LPAREN) {
	  t->child[0] = expression();
  }
  return t;
}

TreeNode * expression(void){
	TreeNode * t = newStmtNode(ExpK);
	int childNum=0;
	if (token == ID) {
		t->child[childNum] = newExpNode(IdK);
		t->child[childNum]->attr.name = copyString(tokenString);
		childNum++;
		match(ID);
		if (token == LBRACKET) {
			match(LBRACKET);
			t->child[childNum++] = expression();
			match(RBRACKET);
		}
		else if (token == LPAREN) {
			match(LPAREN);
			t->child[childNum++] = args();
			match(RPAREN);
		}
		if (token == ASSIGN) {
			t->child[childNum] = newExpNode(OpK);
			t->child[childNum]->attr.op = token;
			childNum++;
			match(ASSIGN);
			t->child[childNum++] = expression();
		}
		else if(token == LE || token == LT || token == GT || token == GE || token == EQ || token == NE ||
			    token == PLUS ||token == MINUS ||token == TIMES ||token == OVER ){
			TreeNode * p = newExpNode(OpK);
			p->attr.op = token;
			p->child[0] = t;
			switch (token)
			{
			case LE:case LT:case GT:case GE:case EQ:case NE:
				match(token);
				p->child[1] = additive_expression();
				break;
			case PLUS:case MINUS:
				match(token);
				p->child[1] = term();
				break;
			case TIMES:case OVER:
				match(token);
				p->child[1] = factor();
				break;
			}

			t=p;
			
		}
	}
	else if(token==NUM ||token==LPAREN){
		t = simple_expression();
	}
	return t;
}

TreeNode * simple_expression(void){ 
	TreeNode * t = additive_expression();
	TreeNode * p = NULL;
	if (token == LE || token ==LT ||token==GT||token==GE||token==EQ||token==NE)
	{
		p = newExpNode(OpK);
		p->attr.op = token;
		match(token);
		p->child[0] = t;
		p->child[1] = additive_expression();
		t = p;
	}
	return t;
}

TreeNode * additive_expression(void) {
	TreeNode * t = term();
	TreeNode * p = NULL;
	while (token == PLUS || token == MINUS)
	{
		p = newExpNode(OpK);
		p->attr.op = token;
		match(token);
		p->child[0] = t;
		p->child[1] = term();
		t = p;
	}
	return t;
}

TreeNode * term(void) {
	TreeNode * t = factor();
	TreeNode * p = NULL;
	while (token == TIMES || token == OVER)
	{
		p = newExpNode(OpK);
		p->attr.op = token;
		match(token);
		p->child[0] = t;
		p->child[1] = factor();
		t = p;
	}
	return t;
}

TreeNode * factor(void) {
	TreeNode * t = NULL;
	TreeNode * p = NULL;
	switch (token)
	{
	case LPAREN:
		match(LPAREN);
		t = expression();
		match(RPAREN);
		break;
	case NUM:
		t = newExpNode(ConstK);
		if ((t != NULL) && (token == NUM))
			t->attr.val = atoi(tokenString);
		match(NUM);
		break;
	case ID:
		t = newExpNode(IdK);
		t->attr.name = copyString(tokenString);
		match(ID);
		int childNum = 0;
		if (token == LBRACKET) {
			match(LBRACKET);
			p = expression();
			t->sibling = p;
			match(RBRACKET);
		}
		else if (token == LPAREN) {
			match(LPAREN);
			p = args();
			t->sibling = p;
			match(RPAREN);
		}
		break;
	default:
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
		break;
	}
	return t;
}

TreeNode * args() {
	TreeNode *t = newStmtNode(ArgK);
	if (token == ID || token == LPAREN || token == NUM) {
		t = arg_list();
	}
	return t;
}

TreeNode * arg_list() {
	TreeNode *t = expression();
	TreeNode * p = t;
	TreeNode * q;
	while (token == COMMA)
	{
		match(COMMA);
		q = expression();
		if (q != NULL) {
			p->sibling = q;
			p = q;
		}
	}
	return t;
}


/****************************************/
/* the primary function of the parser   */
/****************************************/
/* Function parse returns the newly 
 * constructed syntax tree
 */
TreeNode * parse(void)
{ TreeNode * t;
  token = getToken();
  t = declaration_list();
  if (token!=ENDFILE)
    syntaxError("Code ends before file\n");
  return t;
}
