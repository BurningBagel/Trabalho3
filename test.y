%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definicoes.h"

extern FILE *yyin;

void yyerror(char const *s);
int yylex(void);


//#define ID 1
#define CHAR_TABLE 2
#define NUM_TABLE 3
#define STRING_TABLE 5
#define FUNC_TABLE 6
#define ELEM_TABLE 7
#define SET_TABLE 8


simbolo* tabelaSimbolos;
simbolo* topo;
	
simbolo* CriarSimbolo(char* nome, int tipo, char* valor){
	simbolo *ancora = (simbolo*)malloc(sizeof(simbolo));
	if(tabelaSimbolos == NULL){
		tabelaSimbolos = topo = ancora;
		(*ancora).anterior = NULL;
	}
	else{
		(*topo).seguinte = ancora;
		(*ancora).anterior = topo;
	}
	(*ancora).seguinte = NULL;
	(*ancora).tipo = tipo;
	if(valor != NULL){
		(*ancora).valor = strdup(valor);
		(*ancora).tamanhoValor = strlen(valor);
	}
	else{
		(*ancora).valor = NULL;
		(*ancora).tamanhoValor = 0;
	} 
	(*ancora).nome = strdup(nome);
	topo = ancora;
	return ancora;
}
	
simbolo* RemoverSimbolo(simbolo* alvo){//retorna ponteiro para o próximo simbolo caso esteja removendo de algum lugar que não seja o fim da tabela
	simbolo *ancora = (*alvo).seguinte;
	free((*alvo).nome);
	free((*alvo).valor);
	free(alvo);
	return ancora;
}

void ApagarTabela(){
	simbolo *ancora = tabelaSimbolos;
	if(ancora == NULL) return;
	do{
		ancora = RemoverSimbolo(ancora);
	}
	while(ancora != NULL);
}

	
simbolo* ProcurarTabela(char* alvo){
	simbolo *ancora = tabelaSimbolos;
//	printf("\n\nOI %s OI \n\n",alvo);
	while(ancora != NULL){
		if(!strcmp((*ancora).nome,alvo)){
			return ancora;
		}
		ancora = (*ancora).seguinte;
	}
	return NULL;
}
	

no* raiz;



void EscreverTabela(){
	printf("-----------TABELA DE SIMBOLOS----------\n");
	printf("|-------------------------------------|\n");
	simbolo *ancora = tabelaSimbolos;
	while(ancora != NULL){
		printf("%s|",(*ancora).nome);
		if((*ancora).tipo == NUM_TABLE){
			printf("variavel de numero\n");
		}
		else if((*ancora).tipo == FUNC_TABLE){
			printf("funcao\n");
		}
		else if((*ancora).tipo == SET_TABLE){
			printf("variavel de set\n");
		}
		else if((*ancora).tipo == ELEM_TABLE){
			printf("variavel polimorfica\n");
		}
		else{
			printf("Constante\n");
		}

		ancora = (*ancora).seguinte;
	}
}

void ApagarNo(no* argumento){
	free((*argumento).nome);
	free((*argumento).valor);
}

void EscreverArvore(no* argumento,int profund){
	int i,j;
	if(argumento == NULL) return;
	for(j = 0;j < profund;j++){
		printf("-");
	}
	printf(">%s\n",(*argumento).nome);
	for(i = 0;i < (*argumento).numFilhos;i++){
		EscreverArvore((*argumento).filhos[i],profund+1);
	}
	
	ApagarNo(argumento);
}

%}

%define parse.lac full
%define parse.error detailed

%union {
	double val;	
	char *text;
	//bool boolean;
	struct no *node;
}

%token <text> NUM
%token <text> STRING
%token <text> CHAR
%token <text> ID
%token <text> INT
%token <text> FLOAT
%token <text> ELEM
%token <text> SET
%token <text> IF
%token <text> ELSE
%token <text> FOR
%token <text> READ
%token <text> WRITE
%token <text> WRITELN
%token <text> IN
%token <text> IS_SET
%token <text> ADD
%token <text> REMOVE
%token <text> EXISTS
%token <text> FORALL
%token <text> RETURN
%token <text> OPENPAR
%token <text> CLOSEPAR
%token <text> OPENCURLY
%token <text> CLOSECURLY
%token <text> OPENBRAC
%token <text> CLOSEBRAC
%token <text> LESS
%token <text> LE
%token <text> EQ
%token <text> NEQ
%token <text> GREATER
%token <text> GE
%token <text> PLUS
%token <text> MINUS
%token <text> DBS
%token <text> AST
%token <text> BS
%token <text> ASSIGN
%token <text> COMMA
%token <text> SEMICOLON
%token <text> OR
%token <text> AND
%token <text> NOT
%token <text> AMP
%token <text> PCENT

%start	inicio 

%type <node> statement
%type <node> assignment
%type <node> function_declaration
%type <node> conjuntoop
%type <node> mathop
%type <node> return
%type <node> for
%type <node> if
%type <node> write
%type <node> writeln
%type <node> read
%type <node> comparg
%type <node> comparison
//%type <text> in
%type <node> conjuntoop1
%type <node> pertinencia
%type <node> tipagem
%type <node> inclusao
%type <node> remocao
%type <node> selecao
%type <node> iteracao
%type <node> function_call
%type <node> funcargs
%type <node> args
%type <node> args1
%type <node> variable_declaration
%type <node> mathop1
%type <node> mathop2
%type <node> matharg
%type <node> type
%type <node> single_line_statement
%type <node> else

%destructor {$$ = NULL;} <*>

%%

inicio:
		statement								{raiz = $1;}
	;





statement: 
		single_line_statement statement 		{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $1;
													(*ancora).filhos[1] = $2;
													(*ancora).numFilhos = 2;
													char ancora2[] = "single_line_statement";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
												}
		
	|	function_declaration statement 			{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $1;
													(*ancora).filhos[1] = $2;
													(*ancora).numFilhos = 2;
													char ancora2[] = "function_declaration";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
												}
	
	
	/*|	mathop SEMICOLON statement				{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $1;
													(*ancora).filhos[1] = $3;
													(*ancora).numFilhos = 2;
													char ancora2[] = "mathop";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
												}
	*/
	
	|	for statement							{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $1;
													(*ancora).filhos[1] = $2;
													(*ancora).numFilhos = 2;
													char ancora2[] = "for";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
												}
	|	if statement							{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $1;
													(*ancora).filhos[1] = $2;
													(*ancora).numFilhos = 2;
													char ancora2[] = "if";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
												}

	|	iteracao statement 						{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 2;
													(*ancora).filhos[0] = $1;
													(*ancora).filhos[1] = $2;
													(*ancora).tipo = YYSYMBOL_statement;
													char ancora2[] = "iteracao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
												}
	|	variable_declaration SEMICOLON statement 			{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $1;
													(*ancora).filhos[1] = $3;
													(*ancora).numFilhos = 2;
													char ancora2[] = "variable_declaration";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													$2 = NULL;
												}
	/*
	|	assignment SEMICOLON statement			{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $1;
													(*ancora).filhos[1] = $3;
													(*ancora).numFilhos = 2;
													char ancora2[] = "assignment";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													free($2);
												}
	
	|	write SEMICOLON statement				{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $1;
													(*ancora).filhos[1] = $3;
													(*ancora).numFilhos = 2;
													char ancora2[] = "write";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													free($2);
												}
	|	conjuntoop statement			        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $1;
													(*ancora).filhos[1] = $2;
													(*ancora).numFilhos = 2;
													char ancora2[] = "conjuntoop";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
												}
	|	return statement						{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $1;
													(*ancora).filhos[1] = $2;
													(*ancora).numFilhos = 2;
													char ancora2[] = "return";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
												}
	|	writeln SEMICOLON statement				{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $1;
													(*ancora).filhos[1] = $3;
													(*ancora).numFilhos = 2;
													char ancora2[] = "writeln";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													free($2);
												}
	|	read SEMICOLON statement				{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $1;
													(*ancora).filhos[1] = $3;
													(*ancora).numFilhos = 2;
													char ancora2[] = "read";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													free($2);
												}
	|	function_call SEMICOLON statement 		{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $1;
													(*ancora).filhos[1] = $3;
													(*ancora).numFilhos = 2;
													char ancora2[] = "function_call";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													free($2);
												}
 */
	|	%empty									{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 0;
													char ancora2[] = "epsilon";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
												}
	;


single_line_statement:
		return SEMICOLON			 			{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $1;
													(*ancora).numFilhos = 1;
													char ancora2[] = "return";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_single_line_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													$2 = NULL;
												}

	|	assignment SEMICOLON 					{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $1;
													(*ancora).numFilhos = 1;
													char ancora2[] = "assignment";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_single_line_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													$2 = NULL;
												}

	|	write SEMICOLON		 					{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $1;
													(*ancora).numFilhos = 1;
													char ancora2[] = "write";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_single_line_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													$2 = NULL;
												}

	|	writeln SEMICOLON	 					{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $1;
													(*ancora).numFilhos = 1;
													char ancora2[] = "writeln";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_single_line_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													$2 = NULL;
												}

	|	read SEMICOLON		 					{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $1;
													(*ancora).numFilhos = 1;
													char ancora2[] = "read";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_single_line_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													$2 = NULL;
												}
/*
	|	mathop 									{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $1;
													(*ancora).numFilhos = 1;
													char ancora2[] = "mathop";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_single_line_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
												}
*/
	|	conjuntoop 	SEMICOLON		 			{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $1;
													(*ancora).numFilhos = 1;
													char ancora2[] = "conjuntoop";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_single_line_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													$2 = NULL;
												}

	;


comparg:
		ID										{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).tipo = YYSYMBOL_comparg;
													(*ancora).numFilhos = 0;
													(*ancora).valor = strdup($1);
													char ancora2[] = "ID";
													(*ancora).nome = strdup(ancora2);
													simbolo *ancoraSimb = ProcurarTabela($1);
													if(ancoraSimb != NULL){ //Vamos começar a usar a tabela de simbolos! Se não acharmos este ID na tabela, devemos colocar-lo lá, mas sem valor! Só em assignment a gente coloca valor
														(*ancora).refereTabela = ancoraSimb;
													}
													else{
														(*ancora).refereTabela = CriarSimbolo($1,0,NULL);//Não temos como saber aqui qual o tipo desse ID, então criamos ele sem isso tbm
													}
													$$ = ancora;
													free($1);

													

												}
	|	OPENPAR comparison CLOSEPAR				{	
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $2;
													(*ancora).numFilhos = 1;
													(*ancora).tipo = YYSYMBOL_comparg;
													char ancora2[] = "comparison";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													$1 = NULL;
													$3 = NULL;
												}
	|	NUM										{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 0;
													(*ancora).tipo = YYSYMBOL_comparg;
													char ancora2[] = "NUM";
													(*ancora).nome = strdup(ancora2);
													(*ancora).valor = strdup($1);
													(*ancora).refereTabela = NULL;
													$$ = ancora;
													free($1);
												}
	|	function_call 							{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = $1;
													(*ancora).tipo = YYSYMBOL_comparg;
													char ancora2[] = "function_call";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
												}
	;

comparison:
		NOT comparg								{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $2;
													(*ancora).numFilhos = 1;
													(*ancora).tipo = YYSYMBOL_comparison;
													char ancora2[] = "not";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													$1 = NULL;
												}
	|	comparg AND comparg						{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $1;
													(*ancora).filhos[1] = $3;
													(*ancora).numFilhos = 2;
													(*ancora).tipo = YYSYMBOL_comparison;
													char ancora2[] = "and";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													$2 = NULL;
												}
	|	comparg OR comparg						{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $1;
													(*ancora).filhos[1] = $3;
													(*ancora).numFilhos = 2;
													(*ancora).tipo = YYSYMBOL_comparison;
													char ancora2[] = "or";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													$2 = NULL;
												}
	|	comparg GREATER comparg					{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $1;
													(*ancora).filhos[1] = $3;
													(*ancora).numFilhos = 2;
													(*ancora).tipo = YYSYMBOL_comparison;
													char ancora2[] = "greater";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													$2 = NULL;
												}
	|	comparg GE comparg						{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $1;
													(*ancora).filhos[1] = $3;
													(*ancora).numFilhos = 2;
													(*ancora).tipo = YYSYMBOL_comparison;
													char ancora2[] = "greater_or_equal";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													$2 = NULL;
												}
	|	comparg LESS comparg					{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $1;
													(*ancora).filhos[1] = $3;
													(*ancora).numFilhos = 2;
													(*ancora).tipo = YYSYMBOL_comparison;
													char ancora2[] = "less";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													$2 = NULL;
												}
	|	comparg LE comparg						{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $1;
													(*ancora).filhos[1] = $3;
													(*ancora).numFilhos = 2;
													(*ancora).tipo = YYSYMBOL_comparison;
													char ancora2[] = "less_or_equal";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													$2 = NULL;
												}
	|	comparg EQ comparg						{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $1;
													(*ancora).filhos[1] = $3;
													(*ancora).numFilhos = 2;
													(*ancora).tipo = YYSYMBOL_comparison;
													char ancora2[] = "equal";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													$2 = NULL;
												}
	|	comparg NEQ comparg						{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $1;
													(*ancora).filhos[1] = $3;
													(*ancora).numFilhos = 2;
													(*ancora).tipo = YYSYMBOL_comparison;
													char ancora2[] = "not_equal";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													$2 = NULL;
												}
	;

read:
		READ OPENPAR ID CLOSEPAR				{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 0;
													(*ancora).tipo = YYSYMBOL_read;
													char ancora2[] = "read";
													(*ancora).nome = strdup(ancora2);
													simbolo *ancoraSimb = ProcurarTabela($1);
													if(ancoraSimb != NULL){ 
														(*ancora).refereTabela = ancoraSimb;
													}
													else{
														(*ancora).refereTabela = CriarSimbolo($1,0,NULL);
													}
													$$ = ancora;
													(*ancora).valor = strdup($3);
													free($3);
													$1 = NULL;
													$2 = NULL;
													$4 = NULL;
												}
	;

write:
//		WRITE OPENPAR ID CLOSEPAR
		WRITE OPENPAR mathop CLOSEPAR			{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $3;
													(*ancora).numFilhos = 1;
													(*ancora).tipo = YYSYMBOL_write;
													char ancora2[] = "mathop";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													$1 = NULL;
													$2 = NULL;
													$4 = NULL;
												}
//	|	WRITE OPENPAR NUM CLOSEPAR	
	|	WRITE OPENPAR  STRING  CLOSEPAR			{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 0;
													(*ancora).tipo = YYSYMBOL_write;
													char ancora2[] = "string";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = strdup($3);
													$$ = ancora;
													free($3);
													$1 = NULL;
													$2 = NULL;
													$4 = NULL;
												}
	|	WRITE OPENPAR CHAR CLOSEPAR				{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 0;
													(*ancora).tipo = YYSYMBOL_write;
													char ancora2[] = "char";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = strdup($3);
													$$ = ancora;
													free($3);
													$1 = NULL;
													$2 = NULL;
													$4 = NULL;
												}
	;


writeln:
//		WRITELN OPENPAR ID CLOSEPAR
		WRITELN OPENPAR mathop CLOSEPAR			{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $3;
													(*ancora).numFilhos = 1;
													(*ancora).tipo = YYSYMBOL_writeln;
													char ancora2[] = "mathop";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													$1 = NULL;
													$2 = NULL;
													$4 = NULL;
												}
//	|	WRITELN OPENPAR NUM CLOSEPAR
	|	WRITELN OPENPAR  STRING  CLOSEPAR		{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 0;
													(*ancora).tipo = YYSYMBOL_writeln;
													char ancora2[] = "string";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = strdup($3);
													$$ = ancora;
													free($3);
													$1 = NULL;
													$2 = NULL;
													$4 = NULL;
												}
	;
/*in:
		ID IN ID
	|	ID IN conjuntoop
	;
*/
return:
//		RETURN ID
//	|	RETURN function_call
		RETURN comparison						{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $2;
													(*ancora).numFilhos = 1;
													(*ancora).tipo = YYSYMBOL_return;
													char ancora2[] = "comparison";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													$1 = NULL;
													//free($3);
												}
	|	RETURN mathop							{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = $2;
													(*ancora).numFilhos = 1;
													(*ancora).tipo = YYSYMBOL_return;
													char ancora2[] = "mathop";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													$1 = NULL;
													//free($3);
												}
	|	RETURN 									{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 0;
													(*ancora).tipo = YYSYMBOL_return;
													char ancora2[] = "null";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													$1 = NULL;
													//free($2);
												}
	;

for:
		FOR OPENPAR assignment SEMICOLON comparison SEMICOLON assignment CLOSEPAR OPENCURLY statement CLOSECURLY{
																													no* ancora = (no*)malloc(sizeof(no));
																													(*ancora).filhos[0] = $3;
																													(*ancora).filhos[1] = $5;
																													(*ancora).filhos[2] = $7;
																													(*ancora).filhos[3] = $10;
																													(*ancora).numFilhos = 4;
																													(*ancora).tipo = YYSYMBOL_for;
																													char ancora2[] = "for";
																													(*ancora).nome = strdup(ancora2);
																													(*ancora).refereTabela = NULL;
																													(*ancora).valor = NULL;
																													$$ = ancora;
																													$1 = NULL;
																													$2 = NULL;
																													$4 = NULL;
																													$6 = NULL;
																													$8 = NULL;
																													$9 = NULL;
																													$11 = NULL;
																												}
	;

if:
		
	IF OPENPAR comparison CLOSEPAR OPENCURLY statement CLOSECURLY else										{
																												no* ancora = (no*)malloc(sizeof(no));
																												(*ancora).filhos[0] = $3;
																												(*ancora).filhos[1] = $6;
																												(*ancora).filhos[2] = $8;
																												(*ancora).numFilhos = 3;
																												(*ancora).tipo = YYSYMBOL_if;
																												char ancora2[] = "if";
																												(*ancora).nome = strdup(ancora2);
																												(*ancora).refereTabela = NULL;
																												(*ancora).valor = NULL;
																												$$ = ancora;
																												$1 = NULL;
																												$2 = NULL;
																												$4 = NULL;
																												$5 = NULL;
																												$7 = NULL;
																											}
	|  IF OPENPAR comparison CLOSEPAR single_line_statement else											{
																												no* ancora = (no*)malloc(sizeof(no));
																												(*ancora).filhos[0] = $3;
																												(*ancora).filhos[1] = $5;
																												(*ancora).filhos[2] = $6;
																												(*ancora).numFilhos = 3;
																												(*ancora).tipo = YYSYMBOL_if;
																												char ancora2[] = "single_line_statement";
																												(*ancora).nome = strdup(ancora2);
																												(*ancora).refereTabela = NULL;
																												(*ancora).valor = NULL;
																												$$ = ancora;
																												$1 = NULL;
																												$2 = NULL;
																												$4 = NULL;
																											}
	;

else:
	ELSE if 										{
														no* ancora = (no*)malloc(sizeof(no));
														(*ancora).filhos[0] = $2;
														(*ancora).numFilhos = 1;
														char ancora2[] = "if";
														(*ancora).nome = strdup(ancora2);
														(*ancora).tipo = YYSYMBOL_else;
														(*ancora).refereTabela = NULL;
														(*ancora).valor = NULL;
														$$ = ancora;
														free($1);
													}
|	ELSE single_line_statement						{
														no* ancora = (no*)malloc(sizeof(no));
														(*ancora).filhos[0] = $2;
														(*ancora).numFilhos = 1;
														char ancora2[] = "single_line_statement";
														(*ancora).nome = strdup(ancora2);
														(*ancora).tipo = YYSYMBOL_else;
														(*ancora).refereTabela = NULL;
														(*ancora).valor = NULL;
														$$ = ancora;
														$1 = NULL;
													}
|	ELSE OPENCURLY statement CLOSECURLY				{
														no* ancora = (no*)malloc(sizeof(no));
														(*ancora).filhos[0] = $3;
														(*ancora).numFilhos = 1;
														char ancora2[] = "curly";
														(*ancora).nome = strdup(ancora2);
														(*ancora).tipo = YYSYMBOL_else;
														(*ancora).refereTabela = NULL;
														(*ancora).valor = NULL;
														$$ = ancora;
														$1 = NULL;
														$2 = NULL;
														$4 = NULL;
													}
|	%empty											{
														no* ancora = (no*)malloc(sizeof(no));
														(*ancora).numFilhos = 0;
														char ancora2[] = "epsilon";
														(*ancora).nome = strdup(ancora2);
														(*ancora).tipo = YYSYMBOL_else;
														(*ancora).refereTabela = NULL;
														(*ancora).valor = NULL;
														$$ = ancora;
													}
;

conjuntoop:
		pertinencia								{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = $1;
													(*ancora).tipo = YYSYMBOL_conjuntoop;
													char ancora2[] = "pertinencia";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
												}
	|	tipagem									{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = $1;
													(*ancora).tipo = YYSYMBOL_conjuntoop;
													char ancora2[] = "tipagem";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
												}
	|	inclusao								{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = $1;
													(*ancora).tipo = YYSYMBOL_conjuntoop;
													char ancora2[] = "inclusao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
												}
	|	remocao									{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = $1;
													(*ancora).tipo = YYSYMBOL_conjuntoop;
													char ancora2[] = "remocao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
												}
	|	selecao									{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = $1;
													(*ancora).tipo = YYSYMBOL_conjuntoop;
													char ancora2[] = "selecao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
												}
	;
	
conjuntoop1:
		pertinencia								{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = $1;
													(*ancora).tipo = YYSYMBOL_conjuntoop1;
													char ancora2[] = "pertinencia";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
												}
	|	tipagem									{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = $1;
													(*ancora).tipo = YYSYMBOL_conjuntoop1;
													char ancora2[] = "tipagem";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
												}
	|	inclusao								{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = $1;
													(*ancora).tipo = YYSYMBOL_conjuntoop1;
													char ancora2[] = "inclusao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
												}
	|	remocao									{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = $1;
													(*ancora).tipo = YYSYMBOL_conjuntoop1;
													char ancora2[] = "remocao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
												}
	|	selecao									{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = $1;
													(*ancora).tipo = YYSYMBOL_conjuntoop1;
													char ancora2[] = "selecao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
												}
	|	ID 										{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 0;
													(*ancora).tipo = YYSYMBOL_conjuntoop1;
													char ancora2[] = "ID";
													(*ancora).nome = strdup(ancora2);
													simbolo *ancoraSimb = ProcurarTabela($1);
													if(ancoraSimb != NULL){
														(*ancora).refereTabela = ancoraSimb;
													}
													else{
														(*ancora).refereTabela = CriarSimbolo($1,0,NULL);
													}
													(*ancora).valor = strdup($1);
													free($1);
													$$ = ancora;
												}
	;

pertinencia:
		mathop IN conjuntoop1 					{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 2;
													(*ancora).filhos[0] = $1;
													(*ancora).filhos[1] = $3;
													(*ancora).tipo = YYSYMBOL_pertinencia;
													char ancora2[] = "mathop";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													$2 = NULL;
												}
	|	selecao IN conjuntoop1 					{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 2;
													(*ancora).filhos[0] = $1;
													(*ancora).filhos[1] = $3;
													(*ancora).tipo = YYSYMBOL_pertinencia;
													char ancora2[] = "selecao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													$2 = NULL;
												}
	;

tipagem:
		IS_SET OPENPAR conjuntoop1 CLOSEPAR		{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = $3;
													(*ancora).tipo = YYSYMBOL_tipagem;
													char ancora2[] = "tipagem";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													$1 = NULL;
													$2 = NULL;
													$4 = NULL;
												}
	;

inclusao:
		ADD OPENPAR pertinencia CLOSEPAR		{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = $3;
													(*ancora).tipo = YYSYMBOL_inclusao;
													char ancora2[] = "inclusao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													$1 = NULL;
													$2 = NULL;
													$4 = NULL;
												}
	;

remocao:
		REMOVE OPENPAR pertinencia CLOSEPAR		{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = $3;
													(*ancora).tipo = YYSYMBOL_remocao;
													char ancora2[] = "remocao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													$1 = NULL;
													$2 = NULL;
													$4 = NULL;
												}
	;
	
	
selecao:
		EXISTS OPENPAR pertinencia CLOSEPAR		{
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = $3;
													(*ancora).tipo = YYSYMBOL_conjuntoop1;
													char ancora2[] = "iteracao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													$$ = ancora;
													$1 = NULL;
													$2 = NULL;
													$4 = NULL;
												}
	;

iteracao:
		FORALL OPENPAR pertinencia CLOSEPAR OPENCURLY statement CLOSECURLY	{
																				no* ancora = (no*)malloc(sizeof(no));
																				(*ancora).numFilhos = 2;
																				(*ancora).filhos[0] = $3;
																				(*ancora).filhos[1] = $6;
																				(*ancora).tipo = YYSYMBOL_iteracao;
																				char ancora2[] = "bracket";
																				(*ancora).nome = strdup(ancora2);
																				(*ancora).refereTabela = NULL;
																				(*ancora).valor = NULL;
																				$$ = ancora;
																				$1 = NULL;
																				$2 = NULL;
																				$4 = NULL;
																				$5 = NULL;
																				$7 = NULL;
																			}
	|	FORALL OPENPAR pertinencia CLOSEPAR single_line_statement 				{
																				no* ancora = (no*)malloc(sizeof(no));
																				(*ancora).numFilhos = 2;
																				(*ancora).filhos[0] = $3;
																				(*ancora).filhos[1] = $5;
																				(*ancora).tipo = YYSYMBOL_iteracao;
																				char ancora2[] = "semicolon";
																				(*ancora).nome = strdup(ancora2);
																				(*ancora).refereTabela = NULL;
																				(*ancora).valor = NULL;
																				$$ = ancora;
																				$1 = NULL;
																				$2 = NULL;
																				$4 = NULL;
																			}
	;

function_call:
		ID OPENPAR args CLOSEPAR 											{
																				no* ancora = (no*)malloc(sizeof(no));
																				(*ancora).numFilhos = 1;
																				(*ancora).filhos[0] = $3;
																				(*ancora).tipo = YYSYMBOL_function_call;
																				char ancora2[] = "function_call";
																				(*ancora).nome = strdup(ancora2);
																				(*ancora).valor = strdup($1);
																				simbolo *ancoraSimb = ProcurarTabela($1);
																				if(ancoraSimb != NULL){
																					(*ancora).refereTabela = ancoraSimb;
																				}
																				else{
																					(*ancora).refereTabela = CriarSimbolo($1,FUNC_TABLE,NULL);
																				}
																				$$ = ancora;
																				$1 = NULL;
																				$2 = NULL;
																				$4 = NULL;
																			}
	;


/*args:
		args COMMA args
	|	ID
	|	NUM
	;
*/
args:
		ID args1		{
							no* ancora = (no*)malloc(sizeof(no));
							(*ancora).numFilhos = 1;
							(*ancora).filhos[0] = $2;
							(*ancora).tipo = YYSYMBOL_args;
							char ancora2[] = "ID";
							(*ancora).nome = strdup(ancora2);
							simbolo *ancoraSimb = ProcurarTabela($1);
							if(ancoraSimb != NULL){
								(*ancora).refereTabela = ancoraSimb;
							}
							else{
								(*ancora).refereTabela = CriarSimbolo($1,FUNC_TABLE,NULL);
							}
							(*ancora).valor = strdup($1);
							free($1);
							$$ = ancora;
						}
	|	NUM args1		{	
							no* ancora = (no*)malloc(sizeof(no));
							(*ancora).numFilhos = 1;
							(*ancora).filhos[0] = $2;
							(*ancora).tipo = YYSYMBOL_args;
							char ancora2[] = "NUM";
							(*ancora).nome = strdup(ancora2);
							(*ancora).refereTabela = NULL;
							(*ancora).valor = strdup($1);
							free($1);
							$$ = ancora;
						}
	;

args1:
		COMMA args		{
							no* ancora = (no*)malloc(sizeof(no));
							(*ancora).numFilhos = 1;
							(*ancora).filhos[0] = $2;
							(*ancora).tipo = YYSYMBOL_args1;
							char ancora2[] = "comma";
							(*ancora).nome = strdup(ancora2);
							(*ancora).refereTabela = NULL;
							(*ancora).valor = NULL;
							$$ = ancora;
							$1 = NULL;
						}
	|	%empty			{
							no* ancora = (no*)malloc(sizeof(no));
							(*ancora).numFilhos = 0;
							char ancora2[] = "epsilon";
							(*ancora).nome = strdup(ancora2);
							(*ancora).tipo = YYSYMBOL_args1;
							(*ancora).refereTabela = NULL;
							(*ancora).valor = NULL;
							$$ = ancora;
						}
	;

funcargs:
		type ID					{
									no* ancora = (no*)malloc(sizeof(no));
									(*ancora).numFilhos = 1;
									(*ancora).filhos[0] = $1;
									(*ancora).tipo = YYSYMBOL_funcargs;
									char ancora2[] = "single";
									(*ancora).nome = strdup(ancora2);
									simbolo *ancoraSimb = ProcurarTabela($2);
									if(ancoraSimb != NULL){
										(*ancora).refereTabela = ancoraSimb;
									}
									else{
										(*ancora).refereTabela = CriarSimbolo($2,0,NULL);
									}
									(*ancora).valor = strdup($2);
									free($2);
									$$ = ancora;
								}
	|	type ID COMMA funcargs	{
									no* ancora = (no*)malloc(sizeof(no));
									(*ancora).numFilhos = 2;
									(*ancora).filhos[0] = $1;
									(*ancora).filhos[1] = $4;
									(*ancora).tipo = YYSYMBOL_funcargs;
									char ancora2[] = "comma";
									(*ancora).nome = strdup(ancora2);
									//printf("\n\nOI %s OI\n\n",$2);
									simbolo *ancoraSimb = ProcurarTabela($2);
									if(ancoraSimb != NULL){
										(*ancora).refereTabela = ancoraSimb;
									}
									else{
										(*ancora).refereTabela = CriarSimbolo($2,0,NULL);
									}
									(*ancora).valor = strdup($2);
									free($2);
									$3 = NULL;
									$$ = ancora;
								}
	|	%empty					{
									no* ancora = (no*)malloc(sizeof(no));
									(*ancora).numFilhos = 0;
									char ancora2[] = "epsilon";
									(*ancora).nome = strdup(ancora2);
									(*ancora).tipo = YYSYMBOL_funcargs;
									(*ancora).refereTabela = NULL;
									(*ancora).valor = NULL;
									$$ = ancora;
								}
	;
	

function_declaration:
		type ID OPENPAR funcargs CLOSEPAR OPENCURLY statement CLOSECURLY 	{
																				no* ancora = (no*)malloc(sizeof(no));
																				(*ancora).numFilhos = 3;
																				(*ancora).filhos[0] = $1;
																				(*ancora).filhos[1] = $4;
																				(*ancora).filhos[2] = $7;
																				(*ancora).tipo = YYSYMBOL_function_declaration;
																				char ancora2[] = "function_declaration";
																				(*ancora).nome = strdup(ancora2);
																				simbolo *ancoraSimb = ProcurarTabela($2);
																				if(ancoraSimb != NULL){
																					(*ancora).refereTabela = ancoraSimb;
																					(*ancoraSimb).tipo = FUNC_TABLE;
																				}
																				else{
																					(*ancora).refereTabela = CriarSimbolo($2,FUNC_TABLE,NULL);
																				}
																				(*ancora).valor = strdup($2);
																				free($2);
																				$3 = NULL;
																				$5 = NULL;
																				$6 = NULL;
																				$8 = NULL;
																				$$ = ancora;
																			}
	;
	
	
assignment:
		ID ASSIGN mathop													{
																				no* ancora = (no*)malloc(sizeof(no));
																				(*ancora).numFilhos = 1;
																				(*ancora).tipo = YYSYMBOL_assignment;
																				(*ancora).filhos[0] = $3;
																				char ancora2[] = "ID";
																				(*ancora).nome = strdup(ancora2);
																				simbolo *ancoraSimb = ProcurarTabela($1);
																				if(ancoraSimb != NULL){
																					(*ancora).refereTabela = ancoraSimb;
																				}
																				else{
																					(*ancora).refereTabela = CriarSimbolo($1,0,NULL);
																				}
																				(*ancora).valor = strdup($1);
																				free($1);
																				$2 = NULL;
																				$$ = ancora;
																			}
	;

variable_declaration:
		type ID															{
																			no* ancora = (no*)malloc(sizeof(no));
																			(*ancora).numFilhos = 1;
																			(*ancora).filhos[0] = $1;
																			(*ancora).tipo = YYSYMBOL_variable_declaration;
																			char ancora2[] = "variable_declaration";
																			(*ancora).nome = strdup(ancora2);
																			simbolo *ancoraSimb = ProcurarTabela($2);
																			if(ancoraSimb != NULL){
																				(*ancora).refereTabela = ancoraSimb;
																				(*ancoraSimb).tipo = atoi(((no*)$1)->valor);
																			}
																			else{
																				(*ancora).refereTabela = CriarSimbolo($2,atoi(((no*)$1)->valor),NULL);
																			}
																			(*ancora).valor = strdup($2);
																			free($2);
																			$$ = ancora;
																		}
/*		INT ID SEMICOLON
	|	FLOAT ID SEMICOLON
	|	SET ID SEMICOLON
	|	ELEM ID SEMICOLON
*/	;

mathop:
		mathop PLUS mathop1			{
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 2;
										(*ancora).filhos[0] = $1;
										(*ancora).filhos[1] = $3;
										char ancora2[] = "plus";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_mathop;
										(*ancora).refereTabela = NULL;
										(*ancora).valor = NULL;
										$$ = ancora;
										$2 = NULL;
									}
	|	mathop MINUS mathop1 		{
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 2;
										(*ancora).filhos[0] = $1;
										(*ancora).filhos[1] = $3;
										char ancora2[] = "minus";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_mathop;
										(*ancora).refereTabela = NULL;
										(*ancora).valor = NULL;
										$$ = ancora;
										$2 = NULL;
									}
	|	mathop1						{
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 1;
										(*ancora).filhos[0] = $1;
										char ancora2[] = "mathop1";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_mathop;
										(*ancora).refereTabela = NULL;
										(*ancora).valor = NULL;
										$$ = ancora;
									}
	;

mathop1:
		mathop1 AST mathop2			{
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 2;
										(*ancora).filhos[0] = $1;
										(*ancora).filhos[1] = $3;
										char ancora2[] = "ast";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_mathop1;
										(*ancora).refereTabela = NULL;
										(*ancora).valor = NULL;
										$$ = ancora;
										$2 = NULL;
									}
	|	mathop1 BS mathop2			{
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 2;
										(*ancora).filhos[0] = $1;
										(*ancora).filhos[1] = $3;
										char ancora2[] = "bs";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_mathop1;
										(*ancora).refereTabela = NULL;
										(*ancora).valor = NULL;
										$$ = ancora;
										$2 = NULL;
									}
	|	mathop2						{
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 1;
										(*ancora).filhos[0] = $1;
										char ancora2[] = "mathop2";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_mathop1;
										(*ancora).refereTabela = NULL;
										(*ancora).valor = NULL;
										$$ = ancora;
									}
	;


mathop2:
		matharg						{
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 1;
										(*ancora).filhos[0] = $1;
										char ancora2[] = "matharg";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_mathop2;
										(*ancora).refereTabela = NULL;
										(*ancora).valor = NULL;
										$$ = ancora;
									}
	|	OPENPAR mathop CLOSEPAR		{
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 1;
										(*ancora).filhos[0] = $2;
										char ancora2[] = "pars";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_mathop2;
										(*ancora).refereTabela = NULL;
										(*ancora).valor = NULL;
										$$ = ancora;
										$1 = NULL;
										$3 = NULL;
									}
	;

matharg:
		ID							{
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 0;
										(*ancora).tipo = YYSYMBOL_matharg;
										char ancora2[] = "ID";
										(*ancora).nome = strdup(ancora2);
										simbolo *ancoraSimb = ProcurarTabela($1);
										if(ancoraSimb != NULL){
											(*ancora).refereTabela = ancoraSimb;
										}
										else{
											(*ancora).refereTabela = CriarSimbolo($1,0,NULL);
										}
										(*ancora).valor = strdup($1);
										free($1);
										$$ = ancora;																
									}

	|	NUM 						{
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 0;
										(*ancora).tipo = YYSYMBOL_matharg;
										char ancora2[] = "NUM";
										(*ancora).nome = strdup(ancora2);
										(*ancora).valor = strdup($1);
										(*ancora).refereTabela = NULL;
										free($1);
										$$ = ancora;																
									}
	|	function_call				{
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 1;
										(*ancora).filhos[0] = $1;
										(*ancora).tipo = YYSYMBOL_matharg;
										char ancora2[] = "function_call";
										(*ancora).nome = strdup(ancora2);
										(*ancora).valor = NULL;
										(*ancora).refereTabela = NULL;
										$$ = ancora;																
									}
	;

type:
		SET 						{
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 0;
										char ancora2[] = "set";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_type;
										(*ancora).refereTabela = NULL;
										char ancora3[2];
										sprintf(ancora3,"%d",SET_TABLE);
										(*ancora).valor = strdup(ancora3);
										$$ = ancora;
										$1 = NULL;
									}
	|	INT 						{
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 0;
										char ancora2[] = "int";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_type;
										(*ancora).refereTabela = NULL;
										char ancora3[2];
										sprintf(ancora3,"%d",NUM_TABLE);
										(*ancora).valor = strdup(ancora3);
										$$ = ancora;
										$1 = NULL;
									}
	|	ELEM 						{
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 0;
										char ancora2[] = "elem";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_type;
										(*ancora).refereTabela = NULL;
										char ancora3[2];
										sprintf(ancora3,"%d",ELEM_TABLE);
										(*ancora).valor = strdup(ancora3);
										$$ = ancora;
										$1 = NULL;
									}
	|	FLOAT 						{
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 0;
										char ancora2[] = "float";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_type;
										(*ancora).refereTabela = NULL;
										char ancora3[2];
										sprintf(ancora3,"%d",NUM_TABLE);
										(*ancora).valor = strdup(ancora3);
										$$ = ancora;
										$1 = NULL;
									}
	;

%%
/*
void yyerror(char *s){
	extern int yylineno;
	extern char *yytext;

	printf("ERRO %s NO SIMBOLO <%s> na linha <%d>\n",s,yytext,yylineno);
	exit(1);
}
*/

void yyerror(char const *s){
	printf("%s\n",s);

}


/*int yyerror(char *s){
	return yyerror(s);
}
*/
int main(int argc, char **argv){
	topo = tabelaSimbolos;
	//raiz = (no*)malloc(sizeof(no));
	//variavelRaiz = TRUE;
	++argv;
	--argc;//pula o nome do programa
	if (argc > 0){
		yyin = fopen(argv[0],"r");
	}
	else{
		yyin = stdin;
	}
	yyparse();
	fclose(yyin);
	EscreverTabela();
	EscreverArvore(raiz,1);
	//yylex_destroy();
	ApagarTabela();
	return 0;
}

