/* A Bison parser, made by GNU Bison 3.7.5.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30705

/* Bison version string.  */
#define YYBISON_VERSION "3.7.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "test.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definicoes.h"

extern FILE *yyin;
extern int linhaCount;
extern int colunaCount;
extern int yylex_destroy();

void yyerror(char const *s);
int yylex(void);


//#define ID 1
#define CHAR_TABLE 2
#define INT_TABLE 3
#define FLOAT_TABLE 4
#define STRING_TABLE 5
#define FUNC_TABLE 6
#define ELEM_TABLE 7
#define SET_TABLE 8

/*
TODO
	
	2- Implementar conversão de tipos
	3- Implementar acuso de erro devido a uso de tipo errado
*/


simbolo* tabelaSimbolos;
simbolo* topo;
no* raiz;
pilha* pilhaEscopo;

int escopoCounter;


char* ConverteRetornoTipoString(int entrada){
	char* resultado;
	if(entrada == Int){
		resultado = strdup("Inteiro");
	}
	else if(entrada == Float){
		resultado = strdup("Float");
	}
	else if(entrada == Elem){
		resultado = strdup("Elem");
	}
	else if(entrada == Set){
		resultado = strdup("Set");
	}
	return resultado;
}


int ConverteRetornoTipo(no* entrada){
	char* nome = (*entrada).nome;

	if(!strcmp(nome,"set")){
		return Set;
	}
	else if(!strcmp(nome,"int")){
		return Int;
	}
	else if(!strcmp(nome,"elem")){
		return Elem;
	}
	else if(!strcmp(nome,"void")){
		return Void;
	}
	else{
		return Float;
	}
}


int ConverteTableTipo(int tipo){
	switch(tipo){
		case INT_TABLE:
			return Int;
		break;

		case FLOAT_TABLE:
			return Float;
		break;

		case ELEM_TABLE:
			return Elem;
		break;

		case SET_TABLE:
			return Set;
		break;
	}
	return 0;
}

int DecideConversao(int tipo1, int tipo2, int tipoAlvo){//Precisamos de uma função para decidir se vamos fazer uma conversão de tipos.
	if((tipo1 == tipo2) && (tipo2 == tipoAlvo)){
		return None;
	}

	if(tipoAlvo == Int){
		if(tipo1 == Int && tipo2 == Float){
			return FloatToIntRight;
		}
		else if(tipo1 == Float && tipo2 == Int){
			return FloatToIntLeft;
		}
		else if(tipo1 == Elem && tipo2 != Elem){
			return ElemToIntLeft;
		}
		else if(tipo1 != Elem && tipo2 == Elem){
			return ElemToIntRight;
		}
		else if (tipo1 == Elem && tipo2 == Elem){
			return ElemToIntBoth;
		}
	}
	else if(tipoAlvo == Float){
		if(tipo1 == Int && tipo2 == Float){
			return IntToFloatLeft;
		}
		else if(tipo1 == Float && tipo2 == Int){
			return IntToFloatRight;
		}
		else if(tipo1 == Elem && tipo2 != Elem){
			return ElemToFloatLeft;
		}
		else if(tipo1 != Elem && tipo2 == Elem){
			return ElemToFloatRight;
		}
		else if((tipo1 == tipo2) && (tipo2 == Elem)){
			return ElemToFloatBoth;
		}
	}
	else if(tipoAlvo == Elem){
		return None;
	}
	else {
		printf("ERRO SEMANTICO! NAO CONSEGUI DETERMINAR COMO CONVERTER! Linha: %d, Coluna: %d\n",linhaCount,colunaCount);
		return 99;
	}


}


int DecideTipo(int tipo1, int tipo2){ //Função pra decidir qual deve ser o tipo de uma operação que use os dois tipos de entrada
	if(tipo1 == tipo2){ //Se já são do mesmo tipo, use esse tipo mesmo
		return tipo1;
	}
	else if((tipo1 == Float && tipo2 == Int) || (tipo1 == Int && tipo2 == Float)){ //Se um for Int e o outro Float, a operação deve retornar um Float
		return Float;
	}
	else if(tipo1 == Elem && tipo2 != Elem){//Se um deles for Elem, o resultado deve ser o tipo do outro
		return tipo2;
	}
	else {			
		return tipo1;
	}
}
	
simbolo* CriarSimbolo(char* nome, int tipo, char* valor, int escopo){
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
	(*ancora).escopo = escopo;
	(*ancora).returnType = Untyped;
	topo = ancora;
	return ancora;
}

simbolo* CriarSimboloFuncao(char* nome, int tipo, char* valor, int escopo, int retorno){
	simbolo* ancora = CriarSimbolo(nome,tipo,valor,escopo);
	(*ancora).returnType = retorno;
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

simbolo* ProcurarTabelaEscopo(char* alvo, int escopo){//Procura na tabela por um ID no escopo dado com o nome alvo. Usado pra verificar redeclaracao
	simbolo *ancora = tabelaSimbolos; 					//

	while(ancora != NULL){
		if(!strcmp((*ancora).nome,alvo) && (*ancora).escopo == escopo){
			return ancora;
		}
		ancora = (*ancora).seguinte;
	}
	return NULL;
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

void EscreverTabela(){
	printf("-----------TABELA DE SIMBOLOS----------\n");
	printf("|-------------------------------------|\n");
	simbolo *ancora = tabelaSimbolos;
	char* ancoraString;
	while(ancora != NULL){
		printf("%s|",(*ancora).nome);
		if((*ancora).tipo == INT_TABLE){
			printf("variavel de numero inteiro|Escopo: %d\n",(*ancora).escopo);
		}
		else if((*ancora).tipo == FLOAT_TABLE){
			printf("variavel de numero float|Escopo: %d\n",(*ancora).escopo);
		}
		else if((*ancora).tipo == FUNC_TABLE){
			ancoraString = ConverteRetornoTipoString((*ancora).returnType)
			printf("funcao|Tipo de retorno: %s|Escopo: %d\n",ancoraString,(*ancora).escopo);
			free(ancoraString);
		}
		else if((*ancora).tipo == SET_TABLE){
			printf("variavel de set|Escopo: %d\n",(*ancora).escopo);
		}
		else if((*ancora).tipo == ELEM_TABLE){
			printf("variavel polimorfica|Escopo: %d\n",(*ancora).escopo);
		}
		else{
			printf("Constante\n");
		}

		ancora = (*ancora).seguinte;
	}
	printf("\n");
}

void ApagarNo(no* argumento){
	free((*argumento).nome);
	free((*argumento).valor);
	free(argumento);
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

no* ProcurarArvore(int tipo,char* nome, no* base){
	int i;
	no* ancora;
	if(!strcmp((*base).nome,nome) && (*base).tipo == tipo){
		return base;
	}
	else if((*base).numFilhos > 0){
		for(i = 0;i < (*base).numFilhos;i++){
			ancora = ProcurarArvore(tipo,nome,(*base).filhos[i]);
			if(ancora != NULL){
				return ancora;
			}
		}
	}
	return NULL;
}


pilha* Top(pilha* stack){
	pilha* ancora = stack;
	while((*ancora).seguinte != NULL){
		ancora = (*ancora).seguinte;
	}
	return ancora;
}

void Pop(pilha* stack){
	pilha* ancora = stack;
	pilha* ancora2;
	while((*ancora).seguinte != NULL){
		ancora = (*ancora).seguinte;
	}
	ancora2 = (*ancora).anterior;
	(*ancora2).seguinte = NULL;
	free(ancora);
}

void Push(pilha* stack, pilha* argumento){
	pilha* ancora = stack;
	while((*ancora).seguinte != NULL){
		ancora = (*ancora).seguinte;
	}
	(*argumento).anterior = ancora;
	(*ancora).seguinte = argumento;
}

pilha* CriarStack(int valor){
	pilha* ancora = (pilha*)malloc(sizeof(pilha));

	(*ancora).valor = valor;
	(*ancora).seguinte = NULL;
	(*ancora).anterior = NULL;
	return ancora;
}

void LimparStack(pilha* stack){
	pilha* ancora = stack;
	pilha* ancora2;

	while(ancora != NULL){
		ancora2 = ancora;
		ancora = (*ancora).seguinte;
		free(ancora2);
	}
}

simbolo* VerificarEscopo(char* alvo){		//Verifica se o simbolo 'alvo' é acessível dentro da pilha de escopo atual, olhando do topo até sua base, e retornando o ponteiro a tabela se sim.
											//Fazemos isto procurando na tabela de simbolos pela combinação do simbolo alvo e o escopo mais externo ainda não checado(podemos ter o mesmo simbolo em escopos diferentes!)
	pilha* ancora = Top(pilhaEscopo);
	simbolo* ancoraSimbolo;

	while(ancora != NULL){
		ancoraSimbolo = ProcurarTabelaEscopo(alvo,(*ancora).valor);
		if(ancoraSimbolo != NULL){
			return ancoraSimbolo;
		}
		ancora = (*ancora).anterior;
	}
	return NULL;
}


#line 447 "test.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "test.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INTEGER = 3,                    /* INTEGER  */
  YYSYMBOL_FLOATING = 4,                   /* FLOATING  */
  YYSYMBOL_STRING = 5,                     /* STRING  */
  YYSYMBOL_EMPTY = 6,                      /* EMPTY  */
  YYSYMBOL_CHAR = 7,                       /* CHAR  */
  YYSYMBOL_ID = 8,                         /* ID  */
  YYSYMBOL_INT = 9,                        /* INT  */
  YYSYMBOL_FLOAT = 10,                     /* FLOAT  */
  YYSYMBOL_ELEM = 11,                      /* ELEM  */
  YYSYMBOL_SET = 12,                       /* SET  */
  YYSYMBOL_IF = 13,                        /* IF  */
  YYSYMBOL_ELSE = 14,                      /* ELSE  */
  YYSYMBOL_FOR = 15,                       /* FOR  */
  YYSYMBOL_READ = 16,                      /* READ  */
  YYSYMBOL_WRITE = 17,                     /* WRITE  */
  YYSYMBOL_WRITELN = 18,                   /* WRITELN  */
  YYSYMBOL_IN = 19,                        /* IN  */
  YYSYMBOL_IS_SET = 20,                    /* IS_SET  */
  YYSYMBOL_ADD = 21,                       /* ADD  */
  YYSYMBOL_REMOVE = 22,                    /* REMOVE  */
  YYSYMBOL_EXISTS = 23,                    /* EXISTS  */
  YYSYMBOL_FORALL = 24,                    /* FORALL  */
  YYSYMBOL_RETURN = 25,                    /* RETURN  */
  YYSYMBOL_OPENPAR = 26,                   /* OPENPAR  */
  YYSYMBOL_CLOSEPAR = 27,                  /* CLOSEPAR  */
  YYSYMBOL_OPENCURLY = 28,                 /* OPENCURLY  */
  YYSYMBOL_CLOSECURLY = 29,                /* CLOSECURLY  */
  YYSYMBOL_OPENBRAC = 30,                  /* OPENBRAC  */
  YYSYMBOL_CLOSEBRAC = 31,                 /* CLOSEBRAC  */
  YYSYMBOL_LESS = 32,                      /* LESS  */
  YYSYMBOL_LE = 33,                        /* LE  */
  YYSYMBOL_EQ = 34,                        /* EQ  */
  YYSYMBOL_NEQ = 35,                       /* NEQ  */
  YYSYMBOL_GREATER = 36,                   /* GREATER  */
  YYSYMBOL_GE = 37,                        /* GE  */
  YYSYMBOL_PLUS = 38,                      /* PLUS  */
  YYSYMBOL_MINUS = 39,                     /* MINUS  */
  YYSYMBOL_DBS = 40,                       /* DBS  */
  YYSYMBOL_AST = 41,                       /* AST  */
  YYSYMBOL_BS = 42,                        /* BS  */
  YYSYMBOL_ASSIGN = 43,                    /* ASSIGN  */
  YYSYMBOL_COMMA = 44,                     /* COMMA  */
  YYSYMBOL_SEMICOLON = 45,                 /* SEMICOLON  */
  YYSYMBOL_OR = 46,                        /* OR  */
  YYSYMBOL_AND = 47,                       /* AND  */
  YYSYMBOL_NOT = 48,                       /* NOT  */
  YYSYMBOL_AMP = 49,                       /* AMP  */
  YYSYMBOL_PCENT = 50,                     /* PCENT  */
  YYSYMBOL_VOID = 51,                      /* VOID  */
  YYSYMBOL_YYACCEPT = 52,                  /* $accept  */
  YYSYMBOL_inicio = 53,                    /* inicio  */
  YYSYMBOL_statement = 54,                 /* statement  */
  YYSYMBOL_55_1 = 55,                      /* $@1  */
  YYSYMBOL_single_line_statement = 56,     /* single_line_statement  */
  YYSYMBOL_comparg = 57,                   /* comparg  */
  YYSYMBOL_comparison = 58,                /* comparison  */
  YYSYMBOL_read = 59,                      /* read  */
  YYSYMBOL_write = 60,                     /* write  */
  YYSYMBOL_writeln = 61,                   /* writeln  */
  YYSYMBOL_return = 62,                    /* return  */
  YYSYMBOL_for = 63,                       /* for  */
  YYSYMBOL_64_2 = 64,                      /* $@2  */
  YYSYMBOL_if = 65,                        /* if  */
  YYSYMBOL_66_3 = 66,                      /* $@3  */
  YYSYMBOL_67_4 = 67,                      /* $@4  */
  YYSYMBOL_else = 68,                      /* else  */
  YYSYMBOL_69_5 = 69,                      /* $@5  */
  YYSYMBOL_conjuntoop = 70,                /* conjuntoop  */
  YYSYMBOL_conjuntoop1 = 71,               /* conjuntoop1  */
  YYSYMBOL_pertinencia = 72,               /* pertinencia  */
  YYSYMBOL_tipagem = 73,                   /* tipagem  */
  YYSYMBOL_inclusao = 74,                  /* inclusao  */
  YYSYMBOL_remocao = 75,                   /* remocao  */
  YYSYMBOL_selecao = 76,                   /* selecao  */
  YYSYMBOL_iteracao = 77,                  /* iteracao  */
  YYSYMBOL_78_6 = 78,                      /* $@6  */
  YYSYMBOL_79_7 = 79,                      /* $@7  */
  YYSYMBOL_function_call = 80,             /* function_call  */
  YYSYMBOL_args = 81,                      /* args  */
  YYSYMBOL_args1 = 82,                     /* args1  */
  YYSYMBOL_funcargs = 83,                  /* funcargs  */
  YYSYMBOL_function_declaration = 84,      /* function_declaration  */
  YYSYMBOL_85_8 = 85,                      /* $@8  */
  YYSYMBOL_assignment = 86,                /* assignment  */
  YYSYMBOL_variable_declaration = 87,      /* variable_declaration  */
  YYSYMBOL_mathop = 88,                    /* mathop  */
  YYSYMBOL_mathop1 = 89,                   /* mathop1  */
  YYSYMBOL_mathop2 = 90,                   /* mathop2  */
  YYSYMBOL_matharg = 91,                   /* matharg  */
  YYSYMBOL_function_type = 92,             /* function_type  */
  YYSYMBOL_type = 93,                      /* type  */
  YYSYMBOL_num = 94                        /* num  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
# define YYCOPY_NEEDED 1
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  72
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   429

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  43
/* YYNRULES -- Number of rules.  */
#define YYNRULES  108
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  223

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   306


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   497,   497,   511,   528,   527,   552,   584,   600,   616,
     632,   741,   758,   774,   790,   806,   822,   850,   866,   911,
     926,   957,   982,   997,  1012,  1027,  1042,  1057,  1072,  1087,
    1105,  1132,  1149,  1165,  1186,  1203,  1228,  1243,  1258,  1275,
    1275,  1311,  1310,  1337,  1356,  1355,  1382,  1403,  1417,  1431,
    1431,  1452,  1467,  1480,  1493,  1506,  1519,  1535,  1548,  1561,
    1574,  1587,  1600,  1623,  1638,  1656,  1675,  1694,  1714,  1734,
    1733,  1759,  1777,  1776,  1802,  1822,  1857,  1898,  1914,  1928,
    1943,  1959,  1976,  1988,  2006,  2005,  2044,  2069,  2089,  2110,
    2125,  2140,  2156,  2171,  2186,  2203,  2216,  2234,  2255,  2268,
    2282,  2297,  2311,  2329,  2344,  2359,  2374,  2392,  2406
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  static const char *const yy_sname[] =
  {
  "end of file", "error", "invalid token", "INTEGER", "FLOATING",
  "STRING", "EMPTY", "CHAR", "ID", "INT", "FLOAT", "ELEM", "SET", "IF",
  "ELSE", "FOR", "READ", "WRITE", "WRITELN", "IN", "IS_SET", "ADD",
  "REMOVE", "EXISTS", "FORALL", "RETURN", "OPENPAR", "CLOSEPAR",
  "OPENCURLY", "CLOSECURLY", "OPENBRAC", "CLOSEBRAC", "LESS", "LE", "EQ",
  "NEQ", "GREATER", "GE", "PLUS", "MINUS", "DBS", "AST", "BS", "ASSIGN",
  "COMMA", "SEMICOLON", "OR", "AND", "NOT", "AMP", "PCENT", "VOID",
  "$accept", "inicio", "statement", "$@1", "single_line_statement",
  "comparg", "comparison", "read", "write", "writeln", "return", "for",
  "$@2", "if", "$@3", "$@4", "else", "$@5", "conjuntoop", "conjuntoop1",
  "pertinencia", "tipagem", "inclusao", "remocao", "selecao", "iteracao",
  "$@6", "$@7", "function_call", "args", "args1", "funcargs",
  "function_declaration", "$@8", "assignment", "variable_declaration",
  "mathop", "mathop1", "mathop2", "matharg", "function_type", "type",
  "num", YY_NULLPTR
  };
  return yy_sname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306
};
#endif

#define YYPACT_NINF (-165)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-83)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     187,   -34,  -165,  -165,  -165,    -6,  -165,  -165,  -165,  -165,
      -9,    -7,    -1,     1,    20,    31,    50,    72,    75,    95,
       6,   310,  -165,  -165,   115,  -165,    71,   117,   120,   156,
     169,    71,    71,   171,  -165,  -165,  -165,  -165,   104,    71,
    -165,    71,   176,   181,    23,    78,  -165,  -165,   211,   174,
    -165,  -165,    52,   310,    25,   216,   225,   284,   166,   381,
     387,   387,   387,   138,   220,     6,   336,   382,  -165,   -15,
     111,   219,  -165,  -165,  -165,  -165,  -165,  -165,  -165,  -165,
    -165,   381,  -165,  -165,  -165,    71,   381,   310,   310,   310,
     310,  -165,  -165,  -165,   205,   223,   145,   113,   224,   226,
     113,   213,   215,   231,   234,   235,   141,   236,   179,    79,
     238,  -165,  -165,  -165,  -165,   104,   239,   104,   248,   255,
     257,   259,   269,   111,  -165,   336,   336,   336,   336,   336,
     336,   336,   336,  -165,   272,  -165,  -165,  -165,    78,    78,
    -165,  -165,   282,    52,  -165,  -165,  -165,   277,   303,     6,
    -165,  -165,  -165,  -165,  -165,  -165,  -165,  -165,  -165,  -165,
     329,   355,  -165,  -165,  -165,  -165,  -165,  -165,  -165,  -165,
    -165,  -165,   146,  -165,  -165,   298,  -165,   298,   270,  -165,
    -165,  -165,  -165,  -165,   290,   314,   219,   251,  -165,   219,
    -165,   216,   219,   219,   299,   294,   305,  -165,  -165,  -165,
     312,   316,   319,   324,   219,   146,   298,   219,   298,  -165,
    -165,  -165,   331,  -165,  -165,   335,  -165,   337,  -165,  -165,
     219,   338,  -165
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,   107,   108,   100,    97,   104,   106,   105,   103,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      38,     0,     4,   102,     0,     2,     0,     0,     0,     0,
       0,     0,     0,     0,    52,    53,    54,    55,    56,     0,
      99,     0,     0,     0,     0,    91,    94,    95,     0,     0,
      98,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,     0,     0,     0,    36,    20,
       0,     0,     1,     3,    16,    14,    15,    12,     7,     8,
      17,     0,     9,     6,    13,     0,     0,     0,     0,     0,
       0,    84,    88,    87,    79,     0,    79,    86,     0,     0,
      20,     0,     0,     0,     0,     0,     0,     0,     0,    97,
       0,    57,    58,    59,    60,    61,     0,     0,     0,     0,
       0,     0,     0,    20,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,    64,    10,    63,    89,    90,
      92,    93,     0,     0,    77,    75,    76,     0,     0,     0,
      30,    32,    33,    31,    35,    34,    65,    66,    67,    68,
       0,     0,    19,    26,    27,    28,    29,    24,    25,    23,
      22,     5,     0,    78,    44,    51,    41,    51,     0,    72,
      74,    69,    71,    83,     0,     0,     0,     0,    46,     0,
      43,     0,     0,     0,     0,    80,     0,    49,    48,    47,
       0,     0,     0,     0,     0,     0,    51,     0,    51,    39,
      73,    70,     0,    81,    45,     0,    42,     0,    85,    50,
       0,     0,    40
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -165,  -165,   -26,  -165,   -44,   -62,   -47,  -165,  -165,  -165,
    -165,  -165,  -165,   182,  -165,  -165,  -156,  -165,  -165,     4,
      67,   -43,   -37,    59,    73,  -165,  -165,  -165,  -165,   227,
     278,   161,  -165,  -165,   -54,  -165,   -18,    89,    96,  -165,
    -165,  -164,  -165
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    24,    25,    71,    26,    67,    68,    27,    28,    29,
      30,    31,   217,    32,   189,   186,   188,   207,    33,   110,
      34,    35,    36,    37,    38,    39,   193,   192,    40,    95,
     144,   184,    41,   142,    42,    43,    44,    45,    46,    47,
      48,    49,    50
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      73,   102,    69,    70,   124,    78,    79,    99,   185,     2,
       3,    51,     4,    82,    64,    83,   112,    54,   122,    55,
      52,   190,   113,    87,    88,    56,    98,    57,     2,     3,
     -37,     4,    65,    64,    96,    97,   100,    53,   112,   106,
     108,   185,    86,   112,   113,   134,    58,   123,   100,   113,
     214,    65,   216,    94,    66,     2,     3,    59,     4,   136,
      64,    87,    88,   163,   164,   165,   166,   167,   168,   169,
     170,   -11,     1,    66,     2,     3,    60,     4,    21,     5,
       6,     7,     8,     9,    10,   135,    11,    12,    13,    14,
     137,    15,    16,    17,    18,    19,    20,    21,    61,    22,
     -11,    62,   178,   175,   177,    52,   -62,   100,   100,   100,
     100,   100,   100,   100,   100,    72,   180,   182,   114,    89,
      90,    63,    23,    81,   -62,    96,   111,   116,   118,   119,
     121,   100,   115,   117,   117,   117,   117,   201,   133,   120,
     114,     2,     3,   198,     4,   114,    64,   183,   111,    87,
      88,    87,    88,   111,   115,     6,     7,     8,     9,   115,
     196,    18,    74,   200,    21,    75,   202,   203,   153,     2,
       3,   107,     4,   -82,    64,    92,   138,   139,   212,    87,
      88,   215,    93,    87,    88,   140,   141,   -11,     1,   143,
       2,     3,    21,     4,   221,     5,     6,     7,     8,     9,
      10,    76,    11,    12,    13,    14,   155,    15,    16,    17,
      18,    19,    20,    21,    77,    22,    80,    87,    88,    91,
       1,    84,     2,     3,   101,     4,    85,     5,     6,     7,
       8,     9,    10,   103,    11,    12,    13,    14,    23,    15,
      16,    17,    18,    19,    20,    21,    52,    22,   -11,   143,
     145,   147,     1,   148,     2,     3,    53,     4,   150,     5,
     149,   151,   152,   154,    10,   156,   157,    12,    13,    14,
      23,    15,    16,    17,    18,   158,    20,    21,     1,   197,
       2,     3,   159,     4,   160,     5,   161,     2,     3,   104,
       4,   105,    64,    12,    13,    14,   162,    15,    16,    17,
      18,   171,    20,    21,     1,   174,     2,     3,   172,     4,
      21,     5,   187,     2,     3,   191,     4,   194,    64,    12,
      13,    14,   195,    15,    16,    17,    18,   204,    20,    21,
       1,   176,     2,     3,   206,     4,    21,     5,   205,     2,
       3,   208,     4,   209,    64,    12,    13,    14,   210,    15,
      16,    17,    18,   211,    20,    21,     1,   179,     2,     3,
     218,     4,    65,     5,   219,   220,   213,   222,     0,   199,
     173,    12,    13,    14,   146,    15,    16,    17,    18,     0,
      20,    21,     0,   181,     2,     3,     0,     4,     0,   109,
       2,     3,     0,     4,     0,    64,     0,     0,     0,     0,
       0,    15,    16,    17,    18,     0,     0,    21,     0,     0,
      18,     0,     0,    21,   125,   126,   127,   128,   129,   130,
       0,     0,     0,     0,     0,     0,     0,     0,   131,   132
};

static const yytype_int16 yycheck[] =
{
      26,    55,    20,    21,    66,    31,    32,    54,   172,     3,
       4,    45,     6,    39,     8,    41,    59,    26,    65,    26,
      26,   177,    59,    38,    39,    26,     1,    26,     3,     4,
      45,     6,    26,     8,    52,    53,    54,    43,    81,    57,
      58,   205,    19,    86,    81,    71,    26,    65,    66,    86,
     206,    26,   208,     1,    48,     3,     4,    26,     6,    85,
       8,    38,    39,   125,   126,   127,   128,   129,   130,   131,
     132,     0,     1,    48,     3,     4,    26,     6,    26,     8,
       9,    10,    11,    12,    13,    81,    15,    16,    17,    18,
      86,    20,    21,    22,    23,    24,    25,    26,    26,    28,
      29,    26,   149,   147,   148,    26,    27,   125,   126,   127,
     128,   129,   130,   131,   132,     0,   160,   161,    59,    41,
      42,    26,    51,    19,    45,   143,    59,    60,    61,    62,
      63,   149,    59,    60,    61,    62,    63,   191,    27,     1,
      81,     3,     4,   187,     6,    86,     8,     1,    81,    38,
      39,    38,    39,    86,    81,     9,    10,    11,    12,    86,
     186,    23,    45,   189,    26,    45,   192,   193,    27,     3,
       4,     5,     6,    27,     8,     1,    87,    88,   204,    38,
      39,   207,     8,    38,    39,    89,    90,     0,     1,    44,
       3,     4,    26,     6,   220,     8,     9,    10,    11,    12,
      13,    45,    15,    16,    17,    18,    27,    20,    21,    22,
      23,    24,    25,    26,    45,    28,    45,    38,    39,     8,
       1,    45,     3,     4,     8,     6,    45,     8,     9,    10,
      11,    12,    13,     8,    15,    16,    17,    18,    51,    20,
      21,    22,    23,    24,    25,    26,    26,    28,    29,    44,
      27,    27,     1,    27,     3,     4,    43,     6,    27,     8,
      45,    27,    27,    27,    13,    27,    27,    16,    17,    18,
      51,    20,    21,    22,    23,    27,    25,    26,     1,    28,
       3,     4,    27,     6,    27,     8,    27,     3,     4,     5,
       6,     7,     8,    16,    17,    18,    27,    20,    21,    22,
      23,    29,    25,    26,     1,    28,     3,     4,    26,     6,
      26,     8,    14,     3,     4,    45,     6,    27,     8,    16,
      17,    18,     8,    20,    21,    22,    23,    28,    25,    26,
       1,    28,     3,     4,    29,     6,    26,     8,    44,     3,
       4,    29,     6,    27,     8,    16,    17,    18,    29,    20,
      21,    22,    23,    29,    25,    26,     1,    28,     3,     4,
      29,     6,    26,     8,    29,    28,   205,    29,    -1,   187,
     143,    16,    17,    18,    96,    20,    21,    22,    23,    -1,
      25,    26,    -1,    28,     3,     4,    -1,     6,    -1,     8,
       3,     4,    -1,     6,    -1,     8,    -1,    -1,    -1,    -1,
      -1,    20,    21,    22,    23,    -1,    -1,    26,    -1,    -1,
      23,    -1,    -1,    26,    32,    33,    34,    35,    36,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    47
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     1,     3,     4,     6,     8,     9,    10,    11,    12,
      13,    15,    16,    17,    18,    20,    21,    22,    23,    24,
      25,    26,    28,    51,    53,    54,    56,    59,    60,    61,
      62,    63,    65,    70,    72,    73,    74,    75,    76,    77,
      80,    84,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    45,    26,    43,    26,    26,    26,    26,    26,    26,
      26,    26,    26,    26,     8,    26,    48,    57,    58,    88,
      88,    55,     0,    54,    45,    45,    45,    45,    54,    54,
      45,    19,    54,    54,    45,    45,    19,    38,    39,    41,
      42,     8,     1,     8,     1,    81,    88,    88,     1,    58,
      88,     8,    86,     8,     5,     7,    88,     5,    88,     8,
      71,    72,    73,    74,    75,    76,    72,    76,    72,    72,
       1,    72,    58,    88,    57,    32,    33,    34,    35,    36,
      37,    46,    47,    27,    54,    71,    54,    71,    89,    89,
      90,    90,    85,    44,    82,    27,    82,    27,    27,    45,
      27,    27,    27,    27,    27,    27,    27,    27,    27,    27,
      27,    27,    27,    57,    57,    57,    57,    57,    57,    57,
      57,    29,    26,    81,    28,    56,    28,    56,    58,    28,
      56,    28,    56,     1,    83,    93,    67,    14,    68,    66,
      68,    45,    79,    78,    27,     8,    54,    28,    56,    65,
      54,    86,    54,    54,    28,    44,    29,    69,    29,    27,
      29,    29,    54,    83,    68,    54,    68,    64,    29,    29,
      28,    54,    29
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    52,    53,    54,    55,    54,    54,    54,    54,    54,
      54,    54,    56,    56,    56,    56,    56,    56,    56,    57,
      57,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      59,    60,    60,    60,    61,    61,    62,    62,    62,    64,
      63,    66,    65,    65,    67,    65,    65,    68,    68,    69,
      68,    68,    70,    70,    70,    70,    70,    71,    71,    71,
      71,    71,    71,    72,    72,    73,    74,    75,    76,    78,
      77,    77,    79,    77,    77,    80,    81,    81,    82,    82,
      83,    83,    83,    83,    85,    84,    86,    87,    87,    88,
      88,    88,    89,    89,    89,    90,    90,    91,    91,    91,
      91,    92,    92,    93,    93,    93,    93,    94,    94
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     4,     2,     2,     2,     2,
       3,     0,     2,     2,     2,     2,     2,     2,     2,     3,
       1,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       4,     4,     4,     4,     4,     4,     2,     2,     1,     0,
      12,     0,     9,     6,     0,     9,     6,     2,     2,     0,
       5,     0,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     4,     4,     4,     4,     0,
       8,     5,     0,     8,     5,     4,     2,     2,     2,     0,
       2,     4,     0,     1,     0,     9,     3,     2,     2,     3,
       3,     1,     3,     3,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        YY_LAC_DISCARD ("YYBACKUP");                              \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Given a state stack such that *YYBOTTOM is its bottom, such that
   *YYTOP is either its top or is YYTOP_EMPTY to indicate an empty
   stack, and such that *YYCAPACITY is the maximum number of elements it
   can hold without a reallocation, make sure there is enough room to
   store YYADD more elements.  If not, allocate a new stack using
   YYSTACK_ALLOC, copy the existing elements, and adjust *YYBOTTOM,
   *YYTOP, and *YYCAPACITY to reflect the new capacity and memory
   location.  If *YYBOTTOM != YYBOTTOM_NO_FREE, then free the old stack
   using YYSTACK_FREE.  Return 0 if successful or if no reallocation is
   required.  Return YYENOMEM if memory is exhausted.  */
static int
yy_lac_stack_realloc (YYPTRDIFF_T *yycapacity, YYPTRDIFF_T yyadd,
#if YYDEBUG
                      char const *yydebug_prefix,
                      char const *yydebug_suffix,
#endif
                      yy_state_t **yybottom,
                      yy_state_t *yybottom_no_free,
                      yy_state_t **yytop, yy_state_t *yytop_empty)
{
  YYPTRDIFF_T yysize_old =
    *yytop == yytop_empty ? 0 : *yytop - *yybottom + 1;
  YYPTRDIFF_T yysize_new = yysize_old + yyadd;
  if (*yycapacity < yysize_new)
    {
      YYPTRDIFF_T yyalloc = 2 * yysize_new;
      yy_state_t *yybottom_new;
      /* Use YYMAXDEPTH for maximum stack size given that the stack
         should never need to grow larger than the main state stack
         needs to grow without LAC.  */
      if (YYMAXDEPTH < yysize_new)
        {
          YYDPRINTF ((stderr, "%smax size exceeded%s", yydebug_prefix,
                      yydebug_suffix));
          return YYENOMEM;
        }
      if (YYMAXDEPTH < yyalloc)
        yyalloc = YYMAXDEPTH;
      yybottom_new =
        YY_CAST (yy_state_t *,
                 YYSTACK_ALLOC (YY_CAST (YYSIZE_T,
                                         yyalloc * YYSIZEOF (*yybottom_new))));
      if (!yybottom_new)
        {
          YYDPRINTF ((stderr, "%srealloc failed%s", yydebug_prefix,
                      yydebug_suffix));
          return YYENOMEM;
        }
      if (*yytop != yytop_empty)
        {
          YYCOPY (yybottom_new, *yybottom, yysize_old);
          *yytop = yybottom_new + (yysize_old - 1);
        }
      if (*yybottom != yybottom_no_free)
        YYSTACK_FREE (*yybottom);
      *yybottom = yybottom_new;
      *yycapacity = yyalloc;
    }
  return 0;
}

/* Establish the initial context for the current lookahead if no initial
   context is currently established.

   We define a context as a snapshot of the parser stacks.  We define
   the initial context for a lookahead as the context in which the
   parser initially examines that lookahead in order to select a
   syntactic action.  Thus, if the lookahead eventually proves
   syntactically unacceptable (possibly in a later context reached via a
   series of reductions), the initial context can be used to determine
   the exact set of tokens that would be syntactically acceptable in the
   lookahead's place.  Moreover, it is the context after which any
   further semantic actions would be erroneous because they would be
   determined by a syntactically unacceptable token.

   YY_LAC_ESTABLISH should be invoked when a reduction is about to be
   performed in an inconsistent state (which, for the purposes of LAC,
   includes consistent states that don't know they're consistent because
   their default reductions have been disabled).  Iff there is a
   lookahead token, it should also be invoked before reporting a syntax
   error.  This latter case is for the sake of the debugging output.

   For parse.lac=full, the implementation of YY_LAC_ESTABLISH is as
   follows.  If no initial context is currently established for the
   current lookahead, then check if that lookahead can eventually be
   shifted if syntactic actions continue from the current context.
   Report a syntax error if it cannot.  */
#define YY_LAC_ESTABLISH                                                \
do {                                                                    \
  if (!yy_lac_established)                                              \
    {                                                                   \
      YYDPRINTF ((stderr,                                               \
                  "LAC: initial context established for %s\n",          \
                  yysymbol_name (yytoken)));                            \
      yy_lac_established = 1;                                           \
      switch (yy_lac (yyesa, &yyes, &yyes_capacity, yyssp, yytoken))    \
        {                                                               \
        case YYENOMEM:                                                  \
          goto yyexhaustedlab;                                          \
        case 1:                                                         \
          goto yyerrlab;                                                \
        }                                                               \
    }                                                                   \
} while (0)

/* Discard any previous initial lookahead context because of Event,
   which may be a lookahead change or an invalidation of the currently
   established initial context for the current lookahead.

   The most common example of a lookahead change is a shift.  An example
   of both cases is syntax error recovery.  That is, a syntax error
   occurs when the lookahead is syntactically erroneous for the
   currently established initial context, so error recovery manipulates
   the parser stacks to try to find a new initial context in which the
   current lookahead is syntactically acceptable.  If it fails to find
   such a context, it discards the lookahead.  */
#if YYDEBUG
# define YY_LAC_DISCARD(Event)                                           \
do {                                                                     \
  if (yy_lac_established)                                                \
    {                                                                    \
      YYDPRINTF ((stderr, "LAC: initial context discarded due to "       \
                  Event "\n"));                                          \
      yy_lac_established = 0;                                            \
    }                                                                    \
} while (0)
#else
# define YY_LAC_DISCARD(Event) yy_lac_established = 0
#endif

/* Given the stack whose top is *YYSSP, return 0 iff YYTOKEN can
   eventually (after perhaps some reductions) be shifted, return 1 if
   not, or return YYENOMEM if memory is exhausted.  As preconditions and
   postconditions: *YYES_CAPACITY is the allocated size of the array to
   which *YYES points, and either *YYES = YYESA or *YYES points to an
   array allocated with YYSTACK_ALLOC.  yy_lac may overwrite the
   contents of either array, alter *YYES and *YYES_CAPACITY, and free
   any old *YYES other than YYESA.  */
static int
yy_lac (yy_state_t *yyesa, yy_state_t **yyes,
        YYPTRDIFF_T *yyes_capacity, yy_state_t *yyssp, yysymbol_kind_t yytoken)
{
  yy_state_t *yyes_prev = yyssp;
  yy_state_t *yyesp = yyes_prev;
  /* Reduce until we encounter a shift and thereby accept the token.  */
  YYDPRINTF ((stderr, "LAC: checking lookahead %s:", yysymbol_name (yytoken)));
  if (yytoken == YYSYMBOL_YYUNDEF)
    {
      YYDPRINTF ((stderr, " Always Err\n"));
      return 1;
    }
  while (1)
    {
      int yyrule = yypact[+*yyesp];
      if (yypact_value_is_default (yyrule)
          || (yyrule += yytoken) < 0 || YYLAST < yyrule
          || yycheck[yyrule] != yytoken)
        {
          /* Use the default action.  */
          yyrule = yydefact[+*yyesp];
          if (yyrule == 0)
            {
              YYDPRINTF ((stderr, " Err\n"));
              return 1;
            }
        }
      else
        {
          /* Use the action from yytable.  */
          yyrule = yytable[yyrule];
          if (yytable_value_is_error (yyrule))
            {
              YYDPRINTF ((stderr, " Err\n"));
              return 1;
            }
          if (0 < yyrule)
            {
              YYDPRINTF ((stderr, " S%d\n", yyrule));
              return 0;
            }
          yyrule = -yyrule;
        }
      /* By now we know we have to simulate a reduce.  */
      YYDPRINTF ((stderr, " R%d", yyrule - 1));
      {
        /* Pop the corresponding number of values from the stack.  */
        YYPTRDIFF_T yylen = yyr2[yyrule];
        /* First pop from the LAC stack as many tokens as possible.  */
        if (yyesp != yyes_prev)
          {
            YYPTRDIFF_T yysize = yyesp - *yyes + 1;
            if (yylen < yysize)
              {
                yyesp -= yylen;
                yylen = 0;
              }
            else
              {
                yyesp = yyes_prev;
                yylen -= yysize;
              }
          }
        /* Only afterwards look at the main stack.  */
        if (yylen)
          yyesp = yyes_prev -= yylen;
      }
      /* Push the resulting state of the reduction.  */
      {
        yy_state_fast_t yystate;
        {
          const int yylhs = yyr1[yyrule] - YYNTOKENS;
          const int yyi = yypgoto[yylhs] + *yyesp;
          yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyesp
                     ? yytable[yyi]
                     : yydefgoto[yylhs]);
        }
        if (yyesp == yyes_prev)
          {
            yyesp = *yyes;
            YY_IGNORE_USELESS_CAST_BEGIN
            *yyesp = YY_CAST (yy_state_t, yystate);
            YY_IGNORE_USELESS_CAST_END
          }
        else
          {
            if (yy_lac_stack_realloc (yyes_capacity, 1,
#if YYDEBUG
                                      " (", ")",
#endif
                                      yyes, yyesa, &yyesp, yyes_prev))
              {
                YYDPRINTF ((stderr, "\n"));
                return YYENOMEM;
              }
            YY_IGNORE_USELESS_CAST_BEGIN
            *++yyesp = YY_CAST (yy_state_t, yystate);
            YY_IGNORE_USELESS_CAST_END
          }
        YYDPRINTF ((stderr, " G%d", yystate));
      }
    }
}

/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yy_state_t *yyesa;
  yy_state_t **yyes;
  YYPTRDIFF_T *yyes_capacity;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;

  int yyx;
  for (yyx = 0; yyx < YYNTOKENS; ++yyx)
    {
      yysymbol_kind_t yysym = YY_CAST (yysymbol_kind_t, yyx);
      if (yysym != YYSYMBOL_YYerror && yysym != YYSYMBOL_YYUNDEF)
        switch (yy_lac (yyctx->yyesa, yyctx->yyes, yyctx->yyes_capacity, yyctx->yyssp, yysym))
          {
          case YYENOMEM:
            return YYENOMEM;
          case 1:
            continue;
          default:
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = yysym;
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif



static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
       In the first two cases, it might appear that the current syntax
       error should have been detected in the previous state when yy_lac
       was invoked.  However, at that time, there might have been a
       different syntax error that discarded a different initial context
       during error recovery, leaving behind the current lookahead.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      YYDPRINTF ((stderr, "Constructing syntax error message\n"));
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else if (yyn == 0)
        YYDPRINTF ((stderr, "No expected tokens.\n"));
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.  In order to see if a particular token T is a
   valid looakhead, invoke yy_lac (YYESA, YYES, YYES_CAPACITY, YYSSP, T).

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store or if
   yy_lac returned YYENOMEM.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yystrlen (yysymbol_name (yyarg[yyi]));
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp = yystpcpy (yyp, yysymbol_name (yyarg[yyi++]));
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yykind)
    {
    case YYSYMBOL_INTEGER: /* INTEGER  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1913 "test.tab.c"
        break;

    case YYSYMBOL_FLOATING: /* FLOATING  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1919 "test.tab.c"
        break;

    case YYSYMBOL_STRING: /* STRING  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1925 "test.tab.c"
        break;

    case YYSYMBOL_EMPTY: /* EMPTY  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1931 "test.tab.c"
        break;

    case YYSYMBOL_CHAR: /* CHAR  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1937 "test.tab.c"
        break;

    case YYSYMBOL_ID: /* ID  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1943 "test.tab.c"
        break;

    case YYSYMBOL_INT: /* INT  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1949 "test.tab.c"
        break;

    case YYSYMBOL_FLOAT: /* FLOAT  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1955 "test.tab.c"
        break;

    case YYSYMBOL_ELEM: /* ELEM  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1961 "test.tab.c"
        break;

    case YYSYMBOL_SET: /* SET  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1967 "test.tab.c"
        break;

    case YYSYMBOL_IF: /* IF  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1973 "test.tab.c"
        break;

    case YYSYMBOL_ELSE: /* ELSE  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1979 "test.tab.c"
        break;

    case YYSYMBOL_FOR: /* FOR  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1985 "test.tab.c"
        break;

    case YYSYMBOL_READ: /* READ  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1991 "test.tab.c"
        break;

    case YYSYMBOL_WRITE: /* WRITE  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1997 "test.tab.c"
        break;

    case YYSYMBOL_WRITELN: /* WRITELN  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2003 "test.tab.c"
        break;

    case YYSYMBOL_IN: /* IN  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2009 "test.tab.c"
        break;

    case YYSYMBOL_IS_SET: /* IS_SET  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2015 "test.tab.c"
        break;

    case YYSYMBOL_ADD: /* ADD  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2021 "test.tab.c"
        break;

    case YYSYMBOL_REMOVE: /* REMOVE  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2027 "test.tab.c"
        break;

    case YYSYMBOL_EXISTS: /* EXISTS  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2033 "test.tab.c"
        break;

    case YYSYMBOL_FORALL: /* FORALL  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2039 "test.tab.c"
        break;

    case YYSYMBOL_RETURN: /* RETURN  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2045 "test.tab.c"
        break;

    case YYSYMBOL_OPENPAR: /* OPENPAR  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2051 "test.tab.c"
        break;

    case YYSYMBOL_CLOSEPAR: /* CLOSEPAR  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2057 "test.tab.c"
        break;

    case YYSYMBOL_OPENCURLY: /* OPENCURLY  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2063 "test.tab.c"
        break;

    case YYSYMBOL_CLOSECURLY: /* CLOSECURLY  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2069 "test.tab.c"
        break;

    case YYSYMBOL_OPENBRAC: /* OPENBRAC  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2075 "test.tab.c"
        break;

    case YYSYMBOL_CLOSEBRAC: /* CLOSEBRAC  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2081 "test.tab.c"
        break;

    case YYSYMBOL_LESS: /* LESS  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2087 "test.tab.c"
        break;

    case YYSYMBOL_LE: /* LE  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2093 "test.tab.c"
        break;

    case YYSYMBOL_EQ: /* EQ  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2099 "test.tab.c"
        break;

    case YYSYMBOL_NEQ: /* NEQ  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2105 "test.tab.c"
        break;

    case YYSYMBOL_GREATER: /* GREATER  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2111 "test.tab.c"
        break;

    case YYSYMBOL_GE: /* GE  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2117 "test.tab.c"
        break;

    case YYSYMBOL_PLUS: /* PLUS  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2123 "test.tab.c"
        break;

    case YYSYMBOL_MINUS: /* MINUS  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2129 "test.tab.c"
        break;

    case YYSYMBOL_DBS: /* DBS  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2135 "test.tab.c"
        break;

    case YYSYMBOL_AST: /* AST  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2141 "test.tab.c"
        break;

    case YYSYMBOL_BS: /* BS  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2147 "test.tab.c"
        break;

    case YYSYMBOL_ASSIGN: /* ASSIGN  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2153 "test.tab.c"
        break;

    case YYSYMBOL_COMMA: /* COMMA  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2159 "test.tab.c"
        break;

    case YYSYMBOL_SEMICOLON: /* SEMICOLON  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2165 "test.tab.c"
        break;

    case YYSYMBOL_OR: /* OR  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2171 "test.tab.c"
        break;

    case YYSYMBOL_AND: /* AND  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2177 "test.tab.c"
        break;

    case YYSYMBOL_NOT: /* NOT  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2183 "test.tab.c"
        break;

    case YYSYMBOL_AMP: /* AMP  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2189 "test.tab.c"
        break;

    case YYSYMBOL_PCENT: /* PCENT  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2195 "test.tab.c"
        break;

    case YYSYMBOL_VOID: /* VOID  */
#line 474 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2201 "test.tab.c"
        break;

    case YYSYMBOL_statement: /* statement  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2207 "test.tab.c"
        break;

    case YYSYMBOL_single_line_statement: /* single_line_statement  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2213 "test.tab.c"
        break;

    case YYSYMBOL_comparg: /* comparg  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2219 "test.tab.c"
        break;

    case YYSYMBOL_comparison: /* comparison  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2225 "test.tab.c"
        break;

    case YYSYMBOL_read: /* read  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2231 "test.tab.c"
        break;

    case YYSYMBOL_write: /* write  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2237 "test.tab.c"
        break;

    case YYSYMBOL_writeln: /* writeln  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2243 "test.tab.c"
        break;

    case YYSYMBOL_return: /* return  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2249 "test.tab.c"
        break;

    case YYSYMBOL_for: /* for  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2255 "test.tab.c"
        break;

    case YYSYMBOL_if: /* if  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2261 "test.tab.c"
        break;

    case YYSYMBOL_else: /* else  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2267 "test.tab.c"
        break;

    case YYSYMBOL_conjuntoop: /* conjuntoop  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2273 "test.tab.c"
        break;

    case YYSYMBOL_conjuntoop1: /* conjuntoop1  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2279 "test.tab.c"
        break;

    case YYSYMBOL_pertinencia: /* pertinencia  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2285 "test.tab.c"
        break;

    case YYSYMBOL_tipagem: /* tipagem  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2291 "test.tab.c"
        break;

    case YYSYMBOL_inclusao: /* inclusao  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2297 "test.tab.c"
        break;

    case YYSYMBOL_remocao: /* remocao  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2303 "test.tab.c"
        break;

    case YYSYMBOL_selecao: /* selecao  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2309 "test.tab.c"
        break;

    case YYSYMBOL_iteracao: /* iteracao  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2315 "test.tab.c"
        break;

    case YYSYMBOL_function_call: /* function_call  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2321 "test.tab.c"
        break;

    case YYSYMBOL_args: /* args  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2327 "test.tab.c"
        break;

    case YYSYMBOL_args1: /* args1  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2333 "test.tab.c"
        break;

    case YYSYMBOL_funcargs: /* funcargs  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2339 "test.tab.c"
        break;

    case YYSYMBOL_function_declaration: /* function_declaration  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2345 "test.tab.c"
        break;

    case YYSYMBOL_assignment: /* assignment  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2351 "test.tab.c"
        break;

    case YYSYMBOL_variable_declaration: /* variable_declaration  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2357 "test.tab.c"
        break;

    case YYSYMBOL_mathop: /* mathop  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2363 "test.tab.c"
        break;

    case YYSYMBOL_mathop1: /* mathop1  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2369 "test.tab.c"
        break;

    case YYSYMBOL_mathop2: /* mathop2  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2375 "test.tab.c"
        break;

    case YYSYMBOL_matharg: /* matharg  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2381 "test.tab.c"
        break;

    case YYSYMBOL_function_type: /* function_type  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2387 "test.tab.c"
        break;

    case YYSYMBOL_type: /* type  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2393 "test.tab.c"
        break;

    case YYSYMBOL_num: /* num  */
#line 474 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2399 "test.tab.c"
        break;

      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    yy_state_t yyesa[20];
    yy_state_t *yyes = yyesa;
    YYPTRDIFF_T yyes_capacity = 20 < YYMAXDEPTH ? 20 : YYMAXDEPTH;

  /* Whether LAC context is established.  A Boolean.  */
  int yy_lac_established = 0;
  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    {
      YY_LAC_ESTABLISH;
      goto yydefault;
    }
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      YY_LAC_ESTABLISH;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  YY_LAC_DISCARD ("shift");
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  {
    int yychar_backup = yychar;
    switch (yyn)
      {
  case 2: /* inicio: statement  */
#line 497 "test.y"
                                                                                        {
													raiz = (yyvsp[0].node);
													//(*raiz).escopo = 1;
													if(ProcurarTabela("main") == NULL){
														printf("ERRO SEMANTICO! NAO FOI DECLARADA UMA FUNCAO MAIN!\n");
													}
												}
#line 2689 "test.tab.c"
    break;

  case 3: /* statement: single_line_statement statement  */
#line 511 "test.y"
                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[-1].node);
													(*ancora).filhos[1] = (yyvsp[0].node);
													//(*(no*)$1).escopo = (*ancora).escopo;
													//(*(no*)$2).escopo = (*ancora).escopo;
													(*ancora).numFilhos = 2;
													char ancora2[] = "single_line_statement";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
												}
#line 2710 "test.tab.c"
    break;

  case 4: /* $@1: %empty  */
#line 528 "test.y"
                                                                                                {
													escopoCounter++;
													Push(pilhaEscopo,CriarStack(escopoCounter));
													(yyvsp[0].text) = NULL;
												}
#line 2720 "test.tab.c"
    break;

  case 5: /* statement: OPENCURLY $@1 statement CLOSECURLY  */
#line 533 "test.y"
                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[-1].node);
													//(*(no*)$1).escopo = (*ancora).escopo;
													//(*(no*)$2).escopo = (*ancora).escopo;
													(*ancora).numFilhos = 1;
													char ancora2[] = "curly";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
													// $ 3 = NULL;
													(yyvsp[0].text) = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													Pop(pilhaEscopo);
												}
#line 2743 "test.tab.c"
    break;

  case 6: /* statement: function_declaration statement  */
#line 552 "test.y"
                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[-1].node);
													(*ancora).filhos[1] = (yyvsp[0].node);
													//(*(no*)$1).escopo = (*ancora).escopo;
													//(*(no*)$2).escopo = (*ancora).escopo;
													(*ancora).numFilhos = 2;
													char ancora2[] = "function_declaration";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
												}
#line 2764 "test.tab.c"
    break;

  case 7: /* statement: for statement  */
#line 584 "test.y"
                                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[-1].node);
													(*ancora).filhos[1] = (yyvsp[0].node);
													//(*(no*)$1).escopo = (*ancora).escopo;
													//(*(no*)$2).escopo = (*ancora).escopo;
													(*ancora).numFilhos = 2;
													char ancora2[] = "for";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
												}
#line 2785 "test.tab.c"
    break;

  case 8: /* statement: if statement  */
#line 600 "test.y"
                                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[-1].node);
													(*ancora).filhos[1] = (yyvsp[0].node);
													//(*(no*)$1).escopo = (*ancora).escopo;
													//(*(no*)$2).escopo = (*ancora).escopo;
													(*ancora).numFilhos = 2;
													char ancora2[] = "if";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
												}
#line 2806 "test.tab.c"
    break;

  case 9: /* statement: iteracao statement  */
#line 616 "test.y"
                                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 2;
													(*ancora).filhos[0] = (yyvsp[-1].node);
													(*ancora).filhos[1] = (yyvsp[0].node);
													//(*(no*)$1).escopo = (*ancora).escopo;
													//(*(no*)$2).escopo = (*ancora).escopo;
													(*ancora).tipo = YYSYMBOL_statement;
													char ancora2[] = "iteracao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
												}
#line 2827 "test.tab.c"
    break;

  case 10: /* statement: variable_declaration SEMICOLON statement  */
#line 632 "test.y"
                                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[-2].node);
													(*ancora).filhos[1] = (yyvsp[0].node);
													(*ancora).numFilhos = 2;
													//(*(no*)$1).escopo = (*ancora).escopo;
													//(*(no*)$3).escopo = (*ancora).escopo;
													char ancora2[] = "variable_declaration";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
													(yyvsp[-1].text) = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
												}
#line 2849 "test.tab.c"
    break;

  case 11: /* statement: %empty  */
#line 741 "test.y"
                                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 0;
													char ancora2[] = "epsilon";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
												}
#line 2866 "test.tab.c"
    break;

  case 12: /* single_line_statement: return SEMICOLON  */
#line 758 "test.y"
                                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[-1].node);
													//(*(no*)$1).escopo = (*ancora).escopo;
													(*ancora).numFilhos = 1;
													char ancora2[] = "return";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_single_line_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
													(yyvsp[0].text) = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
												}
#line 2886 "test.tab.c"
    break;

  case 13: /* single_line_statement: assignment SEMICOLON  */
#line 774 "test.y"
                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[-1].node);
													//(*(no*)$1).escopo = (*ancora).escopo;
													(*ancora).numFilhos = 1;
													char ancora2[] = "assignment";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_single_line_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
													(yyvsp[0].text) = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
												}
#line 2906 "test.tab.c"
    break;

  case 14: /* single_line_statement: write SEMICOLON  */
#line 790 "test.y"
                                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[-1].node);
													//(*(no*)$1).escopo = (*ancora).escopo;
													(*ancora).numFilhos = 1;
													char ancora2[] = "write";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_single_line_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
													(yyvsp[0].text) = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
												}
#line 2926 "test.tab.c"
    break;

  case 15: /* single_line_statement: writeln SEMICOLON  */
#line 806 "test.y"
                                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[-1].node);
													//(*(no*)$1).escopo = (*ancora).escopo;
													(*ancora).numFilhos = 1;
													char ancora2[] = "writeln";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_single_line_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
													(yyvsp[0].text) = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
												}
#line 2946 "test.tab.c"
    break;

  case 16: /* single_line_statement: read SEMICOLON  */
#line 822 "test.y"
                                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[-1].node);
													//(*(no*)$1).escopo = (*ancora).escopo;
													(*ancora).numFilhos = 1;
													char ancora2[] = "read";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_single_line_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
													(yyvsp[0].text) = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
												}
#line 2966 "test.tab.c"
    break;

  case 17: /* single_line_statement: conjuntoop SEMICOLON  */
#line 850 "test.y"
                                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[-1].node);
													//(*(no*)$1).escopo = (*ancora).escopo;
													(*ancora).numFilhos = 1;
													char ancora2[] = "conjuntoop";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_single_line_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
													(yyvsp[0].text) = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
												}
#line 2986 "test.tab.c"
    break;

  case 18: /* single_line_statement: error SEMICOLON  */
#line 866 "test.y"
                                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 0;
													char ancora2[] = "ERROR";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_single_line_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
													(yyvsp[0].text) = NULL;
													yyerrok;
												}
#line 3005 "test.tab.c"
    break;

  case 19: /* comparg: OPENPAR comparison CLOSEPAR  */
#line 911 "test.y"
                                                                        {	
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[-1].node);
													(*ancora).numFilhos = 1;
													(*ancora).tipo = YYSYMBOL_comparg;
													char ancora2[] = "comparison";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = ((yyvsp[-1].node))->tipoVirtual;
													(yyval.node) = ancora;
													(yyvsp[-2].text) = NULL;
													(yyvsp[0].text) = NULL;
												}
#line 3025 "test.tab.c"
    break;

  case 20: /* comparg: mathop  */
#line 926 "test.y"
                                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[0].node);
													(*ancora).tipo = YYSYMBOL_comparg;
													char ancora2[] = "mathop";
													(*ancora).nome = strdup(ancora2);
													(*ancora).valor = NULL;
													(*ancora).refereTabela = NULL;
													(*ancora).tipoVirtual = ((yyvsp[0].node))->tipoVirtual;
													(*ancora).conversion = None;
													(yyval.node) = ancora;
												}
#line 3043 "test.tab.c"
    break;

  case 21: /* comparison: NOT comparg  */
#line 957 "test.y"
                                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[0].node);
													(*ancora).numFilhos = 1;
													(*ancora).tipo = YYSYMBOL_comparison;
													char ancora2[] = "not";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													switch(((yyvsp[0].node))->tipoVirtual){
														case Elem:
															(*ancora).conversion = ElemToIntLeft;
															(*ancora).tipoVirtual = Int;
															break;
														case Set:
															printf("ERRO SEMANTICO! EXPRESSAO RESULTANTE EM SET USADA EM OPERACAO INVALIDA! Linha: %d, Coluna: %d\n",linhaCount,colunaCount);
															break;
														default:
															(*ancora).conversion = None;
															(*ancora).tipoVirtual = ((yyvsp[0].node))->tipoVirtual;
															break;
													}
													(yyval.node) = ancora;
													(yyvsp[-1].text) = NULL;
												}
#line 3073 "test.tab.c"
    break;

  case 22: /* comparison: comparg AND comparg  */
#line 982 "test.y"
                                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[-2].node);
													(*ancora).filhos[1] = (yyvsp[0].node);
													(*ancora).numFilhos = 2;
													(*ancora).tipo = YYSYMBOL_comparison;
													char ancora2[] = "and";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).tipoVirtual = DecideTipo(((yyvsp[-2].node))->tipoVirtual,((yyvsp[0].node))->tipoVirtual);
													(*ancora).conversion = DecideConversao(((yyvsp[-2].node))->tipoVirtual,((yyvsp[0].node))->tipoVirtual,(*ancora).tipoVirtual);
													(yyval.node) = ancora;
													(yyvsp[-1].text) = NULL;
												}
#line 3093 "test.tab.c"
    break;

  case 23: /* comparison: comparg OR comparg  */
#line 997 "test.y"
                                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[-2].node);
													(*ancora).filhos[1] = (yyvsp[0].node);
													(*ancora).numFilhos = 2;
													(*ancora).tipo = YYSYMBOL_comparison;
													char ancora2[] = "or";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).tipoVirtual = DecideTipo(((yyvsp[-2].node))->tipoVirtual,((yyvsp[0].node))->tipoVirtual);
													(*ancora).conversion = DecideConversao(((yyvsp[-2].node))->tipoVirtual,((yyvsp[0].node))->tipoVirtual,(*ancora).tipoVirtual);
													(yyval.node) = ancora;
													(yyvsp[-1].text) = NULL;
												}
#line 3113 "test.tab.c"
    break;

  case 24: /* comparison: comparg GREATER comparg  */
#line 1012 "test.y"
                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[-2].node);
													(*ancora).filhos[1] = (yyvsp[0].node);
													(*ancora).numFilhos = 2;
													(*ancora).tipo = YYSYMBOL_comparison;
													char ancora2[] = "greater";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).tipoVirtual = DecideTipo(((yyvsp[-2].node))->tipoVirtual,((yyvsp[0].node))->tipoVirtual);
													(*ancora).conversion = DecideConversao(((yyvsp[-2].node))->tipoVirtual,((yyvsp[0].node))->tipoVirtual,(*ancora).tipoVirtual);
													(yyval.node) = ancora;
													(yyvsp[-1].text) = NULL;
												}
#line 3133 "test.tab.c"
    break;

  case 25: /* comparison: comparg GE comparg  */
#line 1027 "test.y"
                                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[-2].node);
													(*ancora).filhos[1] = (yyvsp[0].node);
													(*ancora).numFilhos = 2;
													(*ancora).tipo = YYSYMBOL_comparison;
													char ancora2[] = "greater_or_equal";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).tipoVirtual = DecideTipo(((yyvsp[-2].node))->tipoVirtual,((yyvsp[0].node))->tipoVirtual);
													(*ancora).conversion = DecideConversao(((yyvsp[-2].node))->tipoVirtual,((yyvsp[0].node))->tipoVirtual,(*ancora).tipoVirtual);
													(yyval.node) = ancora;
													(yyvsp[-1].text) = NULL;
												}
#line 3153 "test.tab.c"
    break;

  case 26: /* comparison: comparg LESS comparg  */
#line 1042 "test.y"
                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[-2].node);
													(*ancora).filhos[1] = (yyvsp[0].node);
													(*ancora).numFilhos = 2;
													(*ancora).tipo = YYSYMBOL_comparison;
													char ancora2[] = "less";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).tipoVirtual = DecideTipo(((yyvsp[-2].node))->tipoVirtual,((yyvsp[0].node))->tipoVirtual);
													(*ancora).conversion = DecideConversao(((yyvsp[-2].node))->tipoVirtual,((yyvsp[0].node))->tipoVirtual,(*ancora).tipoVirtual);
													(yyval.node) = ancora;
													(yyvsp[-1].text) = NULL;
												}
#line 3173 "test.tab.c"
    break;

  case 27: /* comparison: comparg LE comparg  */
#line 1057 "test.y"
                                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[-2].node);
													(*ancora).filhos[1] = (yyvsp[0].node);
													(*ancora).numFilhos = 2;
													(*ancora).tipo = YYSYMBOL_comparison;
													char ancora2[] = "less_or_equal";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).tipoVirtual = DecideTipo(((yyvsp[-2].node))->tipoVirtual,((yyvsp[0].node))->tipoVirtual);
													(*ancora).conversion = DecideConversao(((yyvsp[-2].node))->tipoVirtual,((yyvsp[0].node))->tipoVirtual,(*ancora).tipoVirtual);
													(yyval.node) = ancora;
													(yyvsp[-1].text) = NULL;
												}
#line 3193 "test.tab.c"
    break;

  case 28: /* comparison: comparg EQ comparg  */
#line 1072 "test.y"
                                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[-2].node);
													(*ancora).filhos[1] = (yyvsp[0].node);
													(*ancora).numFilhos = 2;
													(*ancora).tipo = YYSYMBOL_comparison;
													char ancora2[] = "equal";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).tipoVirtual = DecideTipo(((yyvsp[-2].node))->tipoVirtual,((yyvsp[0].node))->tipoVirtual);
													(*ancora).conversion = DecideConversao(((yyvsp[-2].node))->tipoVirtual,((yyvsp[0].node))->tipoVirtual,(*ancora).tipoVirtual);
													(yyval.node) = ancora;
													(yyvsp[-1].text) = NULL;
												}
#line 3213 "test.tab.c"
    break;

  case 29: /* comparison: comparg NEQ comparg  */
#line 1087 "test.y"
                                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[-2].node);
													(*ancora).filhos[1] = (yyvsp[0].node);
													(*ancora).numFilhos = 2;
													(*ancora).tipo = YYSYMBOL_comparison;
													char ancora2[] = "not_equal";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).tipoVirtual = DecideTipo(((yyvsp[-2].node))->tipoVirtual,((yyvsp[0].node))->tipoVirtual);
													(*ancora).conversion = DecideConversao(((yyvsp[-2].node))->tipoVirtual,((yyvsp[0].node))->tipoVirtual,(*ancora).tipoVirtual);
													(yyval.node) = ancora;
													(yyvsp[-1].text) = NULL;
												}
#line 3233 "test.tab.c"
    break;

  case 30: /* read: READ OPENPAR ID CLOSEPAR  */
#line 1105 "test.y"
                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 0;
													(*ancora).tipo = YYSYMBOL_read;
													char ancora2[] = "read";
													(*ancora).nome = strdup(ancora2);
													simbolo *ancoraSimb = VerificarEscopo((yyvsp[-1].text));
													if(ancoraSimb != NULL){ 
														(*ancora).refereTabela = ancoraSimb;
													}
													else{
														printf("ERRO SEMANTICO! ID %s USADO FORA DE ESCOPO! Linha: %d, Coluna: %d\n",(yyvsp[-1].text),linhaCount,colunaCount);
														(*ancora).refereTabela = NULL;
													}
													(yyval.node) = ancora;
													(*ancora).valor = strdup((yyvsp[-1].text));
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													free((yyvsp[-1].text));
													(yyvsp[-3].text) = NULL;
													(yyvsp[-2].text) = NULL;
													(yyvsp[0].text) = NULL;
												}
#line 3261 "test.tab.c"
    break;

  case 31: /* write: WRITE OPENPAR mathop CLOSEPAR  */
#line 1132 "test.y"
                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[-1].node);
													(*ancora).numFilhos = 1;
													(*ancora).tipo = YYSYMBOL_write;
													char ancora2[] = "mathop";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
													(yyvsp[-3].text) = NULL;
													(yyvsp[-2].text) = NULL;
													(yyvsp[0].text) = NULL;
												}
#line 3282 "test.tab.c"
    break;

  case 32: /* write: WRITE OPENPAR STRING CLOSEPAR  */
#line 1149 "test.y"
                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 0;
													(*ancora).tipo = YYSYMBOL_write;
													char ancora2[] = "string";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = strdup((yyvsp[-1].text));
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
													free((yyvsp[-1].text));
													(yyvsp[-3].text) = NULL;
													(yyvsp[-2].text) = NULL;
													(yyvsp[0].text) = NULL;
												}
#line 3303 "test.tab.c"
    break;

  case 33: /* write: WRITE OPENPAR CHAR CLOSEPAR  */
#line 1165 "test.y"
                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 0;
													(*ancora).tipo = YYSYMBOL_write;
													char ancora2[] = "char";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = strdup((yyvsp[-1].text));
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
													free((yyvsp[-1].text));
													(yyvsp[-3].text) = NULL;
													(yyvsp[-2].text) = NULL;
													(yyvsp[0].text) = NULL;
												}
#line 3324 "test.tab.c"
    break;

  case 34: /* writeln: WRITELN OPENPAR mathop CLOSEPAR  */
#line 1186 "test.y"
                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[-1].node);
													(*ancora).numFilhos = 1;
													(*ancora).tipo = YYSYMBOL_writeln;
													char ancora2[] = "mathop";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
													(yyvsp[-3].text) = NULL;
													(yyvsp[-2].text) = NULL;
													(yyvsp[0].text) = NULL;
												}
#line 3345 "test.tab.c"
    break;

  case 35: /* writeln: WRITELN OPENPAR STRING CLOSEPAR  */
#line 1203 "test.y"
                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 0;
													(*ancora).tipo = YYSYMBOL_writeln;
													char ancora2[] = "string";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = strdup((yyvsp[-1].text));
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
													free((yyvsp[-1].text));
													(yyvsp[-3].text) = NULL;
													(yyvsp[-2].text) = NULL;
													(yyvsp[0].text) = NULL;
												}
#line 3366 "test.tab.c"
    break;

  case 36: /* return: RETURN comparison  */
#line 1228 "test.y"
                                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[0].node);
													(*ancora).numFilhos = 1;
													(*ancora).tipo = YYSYMBOL_return;
													char ancora2[] = "comparison";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
													(yyvsp[-1].text) = NULL;
													//free($3);
												}
#line 3386 "test.tab.c"
    break;

  case 37: /* return: RETURN mathop  */
#line 1243 "test.y"
                                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[0].node);
													(*ancora).numFilhos = 1;
													(*ancora).tipo = YYSYMBOL_return;
													char ancora2[] = "mathop";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
													(yyvsp[-1].text) = NULL;
													//free($3);
												}
#line 3406 "test.tab.c"
    break;

  case 38: /* return: RETURN  */
#line 1258 "test.y"
                                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 0;
													(*ancora).tipo = YYSYMBOL_return;
													char ancora2[] = "null";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
													(yyvsp[0].text) = NULL;
													//free($2);
												}
#line 3425 "test.tab.c"
    break;

  case 39: /* $@2: %empty  */
#line 1275 "test.y"
                                                                                                                                                        {
																													escopoCounter++;
																													Push(pilhaEscopo,CriarStack(escopoCounter));
																													(yyvsp[-7].text) = NULL;
																													(yyvsp[-6].text) = NULL;
																													(yyvsp[-4].text) = NULL;
																													(yyvsp[-2].text) = NULL;
																													(yyvsp[0].text) = NULL;
																													
																													
																												}
#line 3441 "test.tab.c"
    break;

  case 40: /* for: FOR OPENPAR assignment SEMICOLON comparison SEMICOLON assignment CLOSEPAR $@2 OPENCURLY statement CLOSECURLY  */
#line 1286 "test.y"
                                                                                                                                                                                                {
																													no* ancora = (no*)malloc(sizeof(no));
																													(*ancora).filhos[0] = (yyvsp[-9].node);
																													(*ancora).filhos[1] = (yyvsp[-7].node);
																													(*ancora).filhos[2] = (yyvsp[-5].node);
																													//$ $ = ancora;
																													(*ancora).numFilhos = 4;
																													(*ancora).tipo = YYSYMBOL_for;
																													char ancora2[] = "for";
																													(*ancora).nome = strdup(ancora2);
																													(*ancora).refereTabela = NULL;
																													(*ancora).valor = NULL;
																													(*ancora).filhos[3] = (yyvsp[-1].node);
																													(*ancora).conversion = None;
																													(*ancora).tipoVirtual = 0;
																													(yyvsp[0].text) = NULL;
																													(yyvsp[-2].text) = NULL;
																													(yyval.node) = ancora;
																													Pop(pilhaEscopo);
																												}
#line 3466 "test.tab.c"
    break;

  case 41: /* $@3: %empty  */
#line 1311 "test.y"
                                                                                                                                                                                                                        {
																												escopoCounter++;
																												Push(pilhaEscopo,CriarStack(escopoCounter));
																												
																											}
#line 3476 "test.tab.c"
    break;

  case 42: /* if: IF OPENPAR comparison CLOSEPAR OPENCURLY $@3 statement CLOSECURLY else  */
#line 1316 "test.y"
                                                                                                                                                                                                {
																												no* ancora = (no*)malloc(sizeof(no));
																												(*ancora).filhos[0] = (yyvsp[-6].node);
																												(yyvsp[-8].text) = NULL;
																												(yyvsp[-7].text) = NULL;
																												(yyvsp[-5].text) = NULL;
																												(yyvsp[-4].text) = NULL;
																												char ancora2[] = "if";
																												(*ancora).nome = strdup(ancora2);
																												(*ancora).refereTabela = NULL;
																												(*ancora).valor = NULL;
																												(*ancora).numFilhos = 3;
																												(*ancora).tipo = YYSYMBOL_if;
																												(*ancora).filhos[1] = (yyvsp[-2].node);
																												(*ancora).filhos[2] = (yyvsp[0].node);
																												(*ancora).conversion = None;
																												(*ancora).tipoVirtual = 0;
																												(yyvsp[-1].text) = NULL;
																												Pop(pilhaEscopo);
																												(yyval.node) = ancora;
																											}
#line 3502 "test.tab.c"
    break;

  case 43: /* if: IF OPENPAR comparison CLOSEPAR single_line_statement else  */
#line 1337 "test.y"
                                                                                                                                                        {
																												no* ancora = (no*)malloc(sizeof(no));
																												(*ancora).filhos[0] = (yyvsp[-3].node);
																												(*ancora).filhos[1] = (yyvsp[-1].node);
																												(*ancora).filhos[2] = (yyvsp[0].node);
																												(*ancora).numFilhos = 3;
																												(*ancora).tipo = YYSYMBOL_if;
																												char ancora2[] = "single_line_statement";
																												(*ancora).nome = strdup(ancora2);
																												(*ancora).refereTabela = NULL;
																												(*ancora).valor = NULL;
																												(*ancora).conversion = None;
																												(*ancora).tipoVirtual = 0;
																												(yyval.node) = ancora;
																												(yyvsp[-5].text) = NULL;
																												(yyvsp[-4].text) = NULL;
																												(yyvsp[-2].text) = NULL;
																											}
#line 3525 "test.tab.c"
    break;

  case 44: /* $@4: %empty  */
#line 1356 "test.y"
                                                                                                                                                                                                                        {
																												escopoCounter++;
																												Push(pilhaEscopo,CriarStack(escopoCounter));
																												
																											}
#line 3535 "test.tab.c"
    break;

  case 45: /* if: IF OPENPAR error CLOSEPAR OPENCURLY $@4 statement CLOSECURLY else  */
#line 1361 "test.y"
                                                                                                                                                                                                {
																												no* ancora = (no*)malloc(sizeof(no));
																												(yyvsp[-8].text) = NULL;
																												(yyvsp[-7].text) = NULL;
																												(yyvsp[-5].text) = NULL;
																												(yyvsp[-4].text) = NULL;
																												char ancora2[] = "ERROR";
																												(*ancora).nome = strdup(ancora2);
																												(*ancora).refereTabela = NULL;
																												(*ancora).valor = NULL;
																												(*ancora).numFilhos = 2;
																												(*ancora).tipo = YYSYMBOL_if;
																												(*ancora).filhos[0] = (yyvsp[-2].node);
																												(*ancora).filhos[1] = (yyvsp[0].node);
																												(*ancora).conversion = None;
																												(*ancora).tipoVirtual = 0;
																												(yyvsp[-1].text) = NULL;
																												Pop(pilhaEscopo);
																												(yyval.node) = ancora;
																												yyerrok;
																											}
#line 3561 "test.tab.c"
    break;

  case 46: /* if: IF OPENPAR error CLOSEPAR single_line_statement else  */
#line 1382 "test.y"
                                                                                                                                                                {
																												no* ancora = (no*)malloc(sizeof(no));
																												(*ancora).filhos[0] = (yyvsp[-1].node);
																												(*ancora).filhos[1] = (yyvsp[0].node);
																												(*ancora).numFilhos = 2;
																												(*ancora).tipo = YYSYMBOL_if;
																												char ancora2[] = "ERROR";
																												(*ancora).nome = strdup(ancora2);
																												(*ancora).refereTabela = NULL;
																												(*ancora).valor = NULL;
																												(*ancora).conversion = None;
																												(*ancora).tipoVirtual = 0;
																												(yyval.node) = ancora;
																												(yyvsp[-5].text) = NULL;
																												(yyvsp[-4].text) = NULL;
																												(yyvsp[-2].text) = NULL;
																												yyerrok;
																											}
#line 3584 "test.tab.c"
    break;

  case 47: /* else: ELSE if  */
#line 1403 "test.y"
                                                                                                {
														no* ancora = (no*)malloc(sizeof(no));
														(*ancora).filhos[0] = (yyvsp[0].node);
														(*ancora).numFilhos = 1;
														char ancora2[] = "if";
														(*ancora).nome = strdup(ancora2);
														(*ancora).tipo = YYSYMBOL_else;
														(*ancora).refereTabela = NULL;
														(*ancora).valor = NULL;
														(*ancora).conversion = None;
														(*ancora).tipoVirtual = 0;
														(yyval.node) = ancora;
														free((yyvsp[-1].text));
													}
#line 3603 "test.tab.c"
    break;

  case 48: /* else: ELSE single_line_statement  */
#line 1417 "test.y"
                                                                                {
														no* ancora = (no*)malloc(sizeof(no));
														(*ancora).filhos[0] = (yyvsp[0].node);
														(*ancora).numFilhos = 1;
														char ancora2[] = "single_line_statement";
														(*ancora).nome = strdup(ancora2);
														(*ancora).tipo = YYSYMBOL_else;
														(*ancora).refereTabela = NULL;
														(*ancora).valor = NULL;
														(*ancora).conversion = None;
														(*ancora).tipoVirtual = 0;
														(yyval.node) = ancora;
														(yyvsp[-1].text) = NULL;
													}
#line 3622 "test.tab.c"
    break;

  case 49: /* $@5: %empty  */
#line 1431 "test.y"
                                                                                        {
														escopoCounter++;
														Push(pilhaEscopo,CriarStack(escopoCounter));
														(yyvsp[-1].text) = NULL;
														(yyvsp[0].text) = NULL;
													}
#line 3633 "test.tab.c"
    break;

  case 50: /* else: ELSE OPENCURLY $@5 statement CLOSECURLY  */
#line 1437 "test.y"
                                                                                {
														no* ancora = (no*)malloc(sizeof(no));
														(*ancora).filhos[0] = (yyvsp[-1].node);
														(*ancora).numFilhos = 1;
														char ancora2[] = "curly";
														(*ancora).nome = strdup(ancora2);
														(*ancora).tipo = YYSYMBOL_else;
														(*ancora).refereTabela = NULL;
														(*ancora).valor = NULL;
														(*ancora).conversion = None;
														(*ancora).tipoVirtual = 0;
														(yyval.node) = ancora;
														(yyvsp[0].text) = NULL;
														Pop(pilhaEscopo);
													}
#line 3653 "test.tab.c"
    break;

  case 51: /* else: %empty  */
#line 1452 "test.y"
                                                                                                {
														no* ancora = (no*)malloc(sizeof(no));
														(*ancora).numFilhos = 0;
														char ancora2[] = "epsilon";
														(*ancora).nome = strdup(ancora2);
														(*ancora).tipo = YYSYMBOL_else;
														(*ancora).refereTabela = NULL;
														(*ancora).valor = NULL;
														(*ancora).conversion = None;
														(*ancora).tipoVirtual = 0;
														(yyval.node) = ancora;
													}
#line 3670 "test.tab.c"
    break;

  case 52: /* conjuntoop: pertinencia  */
#line 1467 "test.y"
                                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[0].node);
													(*ancora).tipo = YYSYMBOL_conjuntoop;
													char ancora2[] = "pertinencia";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
												}
#line 3688 "test.tab.c"
    break;

  case 53: /* conjuntoop: tipagem  */
#line 1480 "test.y"
                                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[0].node);
													(*ancora).tipo = YYSYMBOL_conjuntoop;
													char ancora2[] = "tipagem";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
												}
#line 3706 "test.tab.c"
    break;

  case 54: /* conjuntoop: inclusao  */
#line 1493 "test.y"
                                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[0].node);
													(*ancora).tipo = YYSYMBOL_conjuntoop;
													char ancora2[] = "inclusao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
												}
#line 3724 "test.tab.c"
    break;

  case 55: /* conjuntoop: remocao  */
#line 1506 "test.y"
                                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[0].node);
													(*ancora).tipo = YYSYMBOL_conjuntoop;
													char ancora2[] = "remocao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
												}
#line 3742 "test.tab.c"
    break;

  case 56: /* conjuntoop: selecao  */
#line 1519 "test.y"
                                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[0].node);
													(*ancora).tipo = YYSYMBOL_conjuntoop;
													char ancora2[] = "selecao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
												}
#line 3760 "test.tab.c"
    break;

  case 57: /* conjuntoop1: pertinencia  */
#line 1535 "test.y"
                                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[0].node);
													(*ancora).tipo = YYSYMBOL_conjuntoop1;
													char ancora2[] = "pertinencia";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
												}
#line 3778 "test.tab.c"
    break;

  case 58: /* conjuntoop1: tipagem  */
#line 1548 "test.y"
                                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[0].node);
													(*ancora).tipo = YYSYMBOL_conjuntoop1;
													char ancora2[] = "tipagem";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
												}
#line 3796 "test.tab.c"
    break;

  case 59: /* conjuntoop1: inclusao  */
#line 1561 "test.y"
                                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[0].node);
													(*ancora).tipo = YYSYMBOL_conjuntoop1;
													char ancora2[] = "inclusao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
												}
#line 3814 "test.tab.c"
    break;

  case 60: /* conjuntoop1: remocao  */
#line 1574 "test.y"
                                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[0].node);
													(*ancora).tipo = YYSYMBOL_conjuntoop1;
													char ancora2[] = "remocao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
												}
#line 3832 "test.tab.c"
    break;

  case 61: /* conjuntoop1: selecao  */
#line 1587 "test.y"
                                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[0].node);
													(*ancora).tipo = YYSYMBOL_conjuntoop1;
													char ancora2[] = "selecao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
												}
#line 3850 "test.tab.c"
    break;

  case 62: /* conjuntoop1: ID  */
#line 1600 "test.y"
                                                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 0;
													(*ancora).tipo = YYSYMBOL_conjuntoop1;
													char ancora2[] = "ID";
													(*ancora).nome = strdup(ancora2);
													simbolo *ancoraSimb = VerificarEscopo((yyvsp[0].text));
													if(ancoraSimb != NULL){ 
														(*ancora).refereTabela = ancoraSimb;
													}
													else{
														printf("ERRO SEMANTICO! ID %s USADO FORA DE ESCOPO! Linha: %d, Coluna: %d\n",(yyvsp[0].text),linhaCount,colunaCount);
														(*ancora).refereTabela = NULL;
													}
													(*ancora).valor = strdup((yyvsp[0].text));
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													free((yyvsp[0].text));
													(yyval.node) = ancora;
												}
#line 3875 "test.tab.c"
    break;

  case 63: /* pertinencia: mathop IN conjuntoop1  */
#line 1623 "test.y"
                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 2;
													(*ancora).filhos[0] = (yyvsp[-2].node);
													(*ancora).filhos[1] = (yyvsp[0].node);
													(*ancora).tipo = YYSYMBOL_pertinencia;
													char ancora2[] = "mathop";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
													(yyvsp[-1].text) = NULL;
												}
#line 3895 "test.tab.c"
    break;

  case 64: /* pertinencia: selecao IN conjuntoop1  */
#line 1638 "test.y"
                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 2;
													(*ancora).filhos[0] = (yyvsp[-2].node);
													(*ancora).filhos[1] = (yyvsp[0].node);
													(*ancora).tipo = YYSYMBOL_pertinencia;
													char ancora2[] = "selecao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
													(yyvsp[-1].text) = NULL;
												}
#line 3915 "test.tab.c"
    break;

  case 65: /* tipagem: IS_SET OPENPAR conjuntoop1 CLOSEPAR  */
#line 1656 "test.y"
                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[-1].node);
													(*ancora).tipo = YYSYMBOL_tipagem;
													char ancora2[] = "tipagem";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
													(yyvsp[-3].text) = NULL;
													(yyvsp[-2].text) = NULL;
													(yyvsp[0].text) = NULL;
												}
#line 3936 "test.tab.c"
    break;

  case 66: /* inclusao: ADD OPENPAR pertinencia CLOSEPAR  */
#line 1675 "test.y"
                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[-1].node);
													(*ancora).tipo = YYSYMBOL_inclusao;
													char ancora2[] = "inclusao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
													(yyvsp[-3].text) = NULL;
													(yyvsp[-2].text) = NULL;
													(yyvsp[0].text) = NULL;
												}
#line 3957 "test.tab.c"
    break;

  case 67: /* remocao: REMOVE OPENPAR pertinencia CLOSEPAR  */
#line 1694 "test.y"
                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[-1].node);
													(*ancora).tipo = YYSYMBOL_remocao;
													char ancora2[] = "remocao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
													(yyvsp[-3].text) = NULL;
													(yyvsp[-2].text) = NULL;
													(yyvsp[0].text) = NULL;
												}
#line 3978 "test.tab.c"
    break;

  case 68: /* selecao: EXISTS OPENPAR pertinencia CLOSEPAR  */
#line 1714 "test.y"
                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[-1].node);
													(*ancora).tipo = YYSYMBOL_conjuntoop1;
													char ancora2[] = "iteracao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(*ancora).conversion = None;
													(*ancora).tipoVirtual = 0;
													(yyval.node) = ancora;
													(yyvsp[-3].text) = NULL;
													(yyvsp[-2].text) = NULL;
													(yyvsp[0].text) = NULL;
												}
#line 3999 "test.tab.c"
    break;

  case 69: /* $@6: %empty  */
#line 1734 "test.y"
                                                                                                                                                        {
																				escopoCounter++;
																				Push(pilhaEscopo,CriarStack(escopoCounter));
																				
																			}
#line 4009 "test.tab.c"
    break;

  case 70: /* iteracao: FORALL OPENPAR pertinencia CLOSEPAR OPENCURLY $@6 statement CLOSECURLY  */
#line 1739 "test.y"
                                                                                                                                {
		 																		(yyvsp[-7].text) = NULL;
																				(yyvsp[-6].text) = NULL;
																				(yyvsp[-4].text) = NULL;
																				(yyvsp[-3].text) = NULL;
																				no* ancora = (no*)malloc(sizeof(no));
																				(*ancora).numFilhos = 2;
																				(*ancora).filhos[0] = (yyvsp[-5].node);
																				(*ancora).tipo = YYSYMBOL_iteracao;
																				char ancora2[] = "bracket";
																				(*ancora).nome = strdup(ancora2);
																				(*ancora).refereTabela = NULL;
																				(*ancora).valor = NULL;
																				(*ancora).filhos[1] = (yyvsp[-1].node);
																				(*ancora).conversion = None;
																				(*ancora).tipoVirtual = 0;
																				(yyvsp[0].text) = NULL;
																				Pop(pilhaEscopo);
																				(yyval.node) = ancora;
																			}
#line 4034 "test.tab.c"
    break;

  case 71: /* iteracao: FORALL OPENPAR pertinencia CLOSEPAR single_line_statement  */
#line 1759 "test.y"
                                                                                                {
																				no* ancora = (no*)malloc(sizeof(no));
																				(*ancora).numFilhos = 2;
																				(*ancora).filhos[0] = (yyvsp[-2].node);
																				(*ancora).filhos[1] = (yyvsp[0].node);
																				(*ancora).tipo = YYSYMBOL_iteracao;
																				char ancora2[] = "semicolon";
																				(*ancora).nome = strdup(ancora2);
																				(*ancora).refereTabela = NULL;
																				(*ancora).valor = NULL;
																				(*ancora).conversion = None;
																				(*ancora).tipoVirtual = 0;
																				(yyval.node) = ancora;
																				(yyvsp[-4].text) = NULL;
																				(yyvsp[-3].text) = NULL;
																				(yyvsp[-1].text) = NULL;
																			}
#line 4056 "test.tab.c"
    break;

  case 72: /* $@7: %empty  */
#line 1777 "test.y"
                                                                                                                                                        {
																				escopoCounter++;
																				Push(pilhaEscopo,CriarStack(escopoCounter));
																				
																			}
#line 4066 "test.tab.c"
    break;

  case 73: /* iteracao: FORALL OPENPAR error CLOSEPAR OPENCURLY $@7 statement CLOSECURLY  */
#line 1782 "test.y"
                                                                                                                                {
		 																		(yyvsp[-7].text) = NULL;
																				(yyvsp[-6].text) = NULL;
																				(yyvsp[-4].text) = NULL;
																				(yyvsp[-3].text) = NULL;
																				no* ancora = (no*)malloc(sizeof(no));
																				(*ancora).numFilhos = 1;
																				(*ancora).filhos[0] = (yyvsp[-1].node);
																				(*ancora).tipo = YYSYMBOL_iteracao;
																				char ancora2[] = "ERROR";
																				(*ancora).nome = strdup(ancora2);
																				(*ancora).refereTabela = NULL;
																				(*ancora).valor = NULL;
																				(*ancora).conversion = None;
																				(*ancora).tipoVirtual = 0;
																				(yyvsp[0].text) = NULL;
																				Pop(pilhaEscopo);
																				(yyval.node) = ancora;
																				yyerrok;
																			}
#line 4091 "test.tab.c"
    break;

  case 74: /* iteracao: FORALL OPENPAR error CLOSEPAR single_line_statement  */
#line 1802 "test.y"
                                                                                                {
																				no* ancora = (no*)malloc(sizeof(no));
																				(*ancora).numFilhos = 1;
																				(*ancora).filhos[0] = (yyvsp[0].node);
																				(*ancora).tipo = YYSYMBOL_iteracao;
																				char ancora2[] = "ERROR SINGLE";
																				(*ancora).nome = strdup(ancora2);
																				(*ancora).refereTabela = NULL;
																				(*ancora).valor = NULL;
																				(*ancora).conversion = None;
																				(*ancora).tipoVirtual = 0;
																				(yyval.node) = ancora;
																				(yyvsp[-4].text) = NULL;
																				(yyvsp[-3].text) = NULL;
																				(yyvsp[-1].text) = NULL;
																				yyerrok;
																			}
#line 4113 "test.tab.c"
    break;

  case 75: /* function_call: ID OPENPAR args CLOSEPAR  */
#line 1822 "test.y"
                                                                                                                                {
																				no* ancora = (no*)malloc(sizeof(no));
																				(*ancora).numFilhos = 1;
																				(*ancora).filhos[0] = (yyvsp[-1].node);
																				(*ancora).tipo = YYSYMBOL_function_call;
																				char ancora2[] = "function_call";
																				(*ancora).nome = strdup(ancora2);
																				(*ancora).valor = strdup((yyvsp[-3].text));
																				simbolo *ancoraSimb = VerificarEscopo((yyvsp[-3].text));
																				if(ancoraSimb != NULL){ 
																					(*ancora).refereTabela = ancoraSimb;
																					(*ancora).tipoVirtual = (*ancoraSimb).returnType;
																				}
																				else{
																					printf("ERRO SEMANTICO! ID %s USADO FORA DE ESCOPO!\n",(yyvsp[-3].text));
																					(*ancora).refereTabela = NULL;
																					(*ancora).tipoVirtual = 0;
																				}
																				(*ancora).conversion = None;
																				
																				(yyval.node) = ancora;
																				(yyvsp[-3].text) = NULL;
																				(yyvsp[-2].text) = NULL;
																				(yyvsp[0].text) = NULL;
																			}
#line 4143 "test.tab.c"
    break;

  case 76: /* args: mathop args1  */
#line 1857 "test.y"
                                        {
								no* ancora = (no*)malloc(sizeof(no));
								(*ancora).numFilhos = 2;
								(*ancora).filhos[0] = (yyvsp[-1].node);
								(*ancora).filhos[1] = (yyvsp[0].node);
								(*ancora).tipo = YYSYMBOL_args;
								char ancora2[] = "mathop";
								(*ancora).nome = strdup(ancora2);
								(*ancora).refereTabela = NULL;
								(*ancora).valor = strdup(((yyvsp[-1].node))->valor);
								(*ancora).conversion = None;
								(*ancora).tipoVirtual = 0;
								(yyval.node) = ancora;
							}
#line 4162 "test.tab.c"
    break;

  case 77: /* args: error args1  */
#line 1898 "test.y"
                                        {	
								no* ancora = (no*)malloc(sizeof(no));
								(*ancora).numFilhos = 1;
								(*ancora).filhos[0] = (yyvsp[0].node);
								(*ancora).tipo = YYSYMBOL_args;
								char ancora2[] = "ERROR";
								(*ancora).nome = strdup(ancora2);
								(*ancora).refereTabela = NULL;
								(*ancora).conversion = None;
								(*ancora).tipoVirtual = 0;
								(yyval.node) = ancora;
								yyerrok;
							}
#line 4180 "test.tab.c"
    break;

  case 78: /* args1: COMMA args  */
#line 1914 "test.y"
                                        {
							no* ancora = (no*)malloc(sizeof(no));
							(*ancora).numFilhos = 1;
							(*ancora).filhos[0] = (yyvsp[0].node);
							(*ancora).tipo = YYSYMBOL_args1;
							char ancora2[] = "comma";
							(*ancora).nome = strdup(ancora2);
							(*ancora).refereTabela = NULL;
							(*ancora).valor = NULL;
							(*ancora).conversion = None;
							(*ancora).tipoVirtual = 0;
							(yyval.node) = ancora;
							(yyvsp[-1].text) = NULL;
						}
#line 4199 "test.tab.c"
    break;

  case 79: /* args1: %empty  */
#line 1928 "test.y"
                                        {
							no* ancora = (no*)malloc(sizeof(no));
							(*ancora).numFilhos = 0;
							char ancora2[] = "epsilon";
							(*ancora).nome = strdup(ancora2);
							(*ancora).tipo = YYSYMBOL_args1;
							(*ancora).refereTabela = NULL;
							(*ancora).valor = NULL;
							(*ancora).conversion = None;
							(*ancora).tipoVirtual = 0;
							(yyval.node) = ancora;
						}
#line 4216 "test.tab.c"
    break;

  case 80: /* funcargs: type ID  */
#line 1943 "test.y"
                                                        {
									no* ancora = (no*)malloc(sizeof(no));
									(*ancora).numFilhos = 1;
									(*ancora).filhos[0] = (yyvsp[-1].node);
									(*ancora).tipo = YYSYMBOL_funcargs;
									char ancora2[] = "single";
									(*ancora).nome = strdup(ancora2);
									simbolo *ancoraSimb = VerificarEscopo((yyvsp[0].text));
									(*ancora).valor = strdup((yyvsp[0].text));
									(*ancora).refereTabela = CriarSimbolo((yyvsp[0].text),atoi(((no*)(yyvsp[-1].node))->valor),NULL,escopoCounter);
									
									(*ancora).conversion = None;
									(*ancora).tipoVirtual = 0;
									free((yyvsp[0].text));
									(yyval.node) = ancora;
								}
#line 4237 "test.tab.c"
    break;

  case 81: /* funcargs: type ID COMMA funcargs  */
#line 1959 "test.y"
                                        {
									no* ancora = (no*)malloc(sizeof(no));
									(*ancora).numFilhos = 2;
									(*ancora).filhos[0] = (yyvsp[-3].node);
									(*ancora).filhos[1] = (yyvsp[0].node);
									(*ancora).tipo = YYSYMBOL_funcargs;
									char ancora2[] = "comma";
									(*ancora).nome = strdup(ancora2);
									//printf("\n\nOI %s OI\n\n",$2);
									(*ancora).valor = strdup((yyvsp[-2].text));
									(*ancora).refereTabela = CriarSimbolo((yyvsp[-2].text),atoi(((no*)(yyvsp[-3].node))->valor),NULL,escopoCounter);
									(*ancora).conversion = None;
									(*ancora).tipoVirtual = 0;
									free((yyvsp[-2].text));
									(yyvsp[-1].text) = NULL;
									(yyval.node) = ancora;
								}
#line 4259 "test.tab.c"
    break;

  case 82: /* funcargs: %empty  */
#line 1976 "test.y"
                                                        {
									no* ancora = (no*)malloc(sizeof(no));
									(*ancora).numFilhos = 0;
									char ancora2[] = "epsilon";
									(*ancora).nome = strdup(ancora2);
									(*ancora).tipo = YYSYMBOL_funcargs;
									(*ancora).refereTabela = NULL;
									(*ancora).valor = NULL;
									(*ancora).conversion = None;
									(*ancora).tipoVirtual = 0;
									(yyval.node) = ancora;
								}
#line 4276 "test.tab.c"
    break;

  case 83: /* funcargs: error  */
#line 1988 "test.y"
                                                        {
									no* ancora = (no*)malloc(sizeof(no));
									(*ancora).numFilhos = 0;
									char ancora2[] = "ERROR";
									(*ancora).nome = strdup(ancora2);
									(*ancora).tipo = YYSYMBOL_funcargs;
									(*ancora).refereTabela = NULL;
									(*ancora).valor = NULL;
									(*ancora).conversion = None;
									(*ancora).tipoVirtual = 0;
									(yyval.node) = ancora;
									yyerrok;
								}
#line 4294 "test.tab.c"
    break;

  case 84: /* $@8: %empty  */
#line 2006 "test.y"
                                                                                                                                                        { //Declaração de função é um pouco estranha, pq o escopo da função é diferente dos argumentos
																				escopoCounter++;
																				Push(pilhaEscopo,CriarStack(escopoCounter));
																			}
#line 4303 "test.tab.c"
    break;

  case 85: /* function_declaration: function_type ID $@8 OPENPAR funcargs CLOSEPAR OPENCURLY statement CLOSECURLY  */
#line 2010 "test.y"
                                                                                                {
																				(yyvsp[-5].text) = NULL;
																				(yyvsp[-3].text) = NULL;
																				(yyvsp[-2].text) = NULL;
																				(yyvsp[0].text) = NULL;
																				no* ancora = (no*)malloc(sizeof(no));
																				int tipoRetorno = ConverteRetornoTipo((yyvsp[-8].node));
																				int realEscopo;
																				(*ancora).numFilhos = 3;
																				(*ancora).filhos[0] = (yyvsp[-8].node);
																				(*ancora).filhos[1] = (yyvsp[-4].node);
																				(*ancora).filhos[2] = (yyvsp[-1].node);
																				(*ancora).tipo = YYSYMBOL_function_declaration;
																				char ancora2[] = "function_declaration";
																				(*ancora).nome = strdup(ancora2);
																				simbolo *ancoraSimb = ProcurarTabela((yyvsp[-7].text));
																				Pop(pilhaEscopo);
																				realEscopo = Top(pilhaEscopo)->valor;
																				if(ancoraSimb != NULL){
																					printf("ERRO SEMANTICO! ID %s REDECLARADO COMO FUNCAO! LINHA: %d, COLUNA: %d \n",(yyvsp[-7].text),linhaCount,colunaCount);
																				}
																				else{
																					(*ancora).refereTabela = CriarSimboloFuncao((yyvsp[-7].text),FUNC_TABLE,NULL,realEscopo,tipoRetorno);
																				}
																				(*ancora).valor = strdup((yyvsp[-7].text));
																				free((yyvsp[-7].text));
																				(*ancora).conversion = None;
																				(*ancora).tipoVirtual = 0;
																				(yyval.node) = ancora;
																			}
#line 4338 "test.tab.c"
    break;

  case 86: /* assignment: ID ASSIGN mathop  */
#line 2044 "test.y"
                                                                                                                                        {
																				no* ancora = (no*)malloc(sizeof(no));
																				(*ancora).numFilhos = 1;
																				(*ancora).tipo = YYSYMBOL_assignment;
																				(*ancora).filhos[0] = (yyvsp[0].node);
																				char ancora2[] = "ID";
																				(*ancora).nome = strdup(ancora2);
																				simbolo *ancoraSimb = VerificarEscopo((yyvsp[-2].text));
																				if(ancoraSimb != NULL){ 
																					(*ancora).refereTabela = ancoraSimb;
																				}
																				else{
																					printf("ERRO SEMANTICO! ID %s USADO FORA DE ESCOPO!\n",(yyvsp[-2].text));
																					(*ancora).refereTabela = NULL;
																				}
																				(*ancora).valor = strdup((yyvsp[-2].text));
																				(*ancora).conversion = None;
																				(*ancora).tipoVirtual = 0;
																				free((yyvsp[-2].text));
																				(yyvsp[-1].text) = NULL;
																				(yyval.node) = ancora;
																			}
#line 4365 "test.tab.c"
    break;

  case 87: /* variable_declaration: type ID  */
#line 2069 "test.y"
                                                                                                                                        {
																			no* ancora = (no*)malloc(sizeof(no));
																			(*ancora).numFilhos = 1;
																			(*ancora).filhos[0] = (yyvsp[-1].node);
																			(*ancora).tipo = YYSYMBOL_variable_declaration;
																			char ancora2[] = "variable_declaration";
																			(*ancora).nome = strdup(ancora2);
																			simbolo *ancoraSimb = ProcurarTabelaEscopo((yyvsp[0].text));
																			if(ancoraSimb != NULL){
																				printf("ERRO SEMANTICO! VARIAVEL %s REDECLARADA! LINHA: %d, COLUNA: %d\n",(yyvsp[0].text),linhaCount,colunaCount);
																			}
																			else{
																				(*ancora).refereTabela = CriarSimbolo((yyvsp[0].text),atoi(((no*)(yyvsp[-1].node))->valor),NULL,escopoCounter);
																			}
																			(*ancora).valor = strdup((yyvsp[0].text));
																			(*ancora).conversion = None;
																			(*ancora).tipoVirtual = 0;
																			free((yyvsp[0].text));
																			(yyval.node) = ancora;
																		}
#line 4390 "test.tab.c"
    break;

  case 88: /* variable_declaration: type error  */
#line 2089 "test.y"
                                                                                                                                                {
																			no* ancora = (no*)malloc(sizeof(no));
																			(*ancora).numFilhos = 1;
																			(*ancora).filhos[0] = (yyvsp[-1].node);
																			char ancora2[] = "ERROR";
																			(*ancora).nome = strdup(ancora2);
																			(*ancora).tipo = YYSYMBOL_variable_declaration;
																			(*ancora).refereTabela = NULL;
																			(*ancora).valor = NULL;
																			(*ancora).conversion = None;
																			(*ancora).tipoVirtual = 0;
																			(yyval.node) = ancora;
																			yyerrok;
																		}
#line 4409 "test.tab.c"
    break;

  case 89: /* mathop: mathop PLUS mathop1  */
#line 2110 "test.y"
                                                        {
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 2;
										(*ancora).filhos[0] = (yyvsp[-2].node);
										(*ancora).filhos[1] = (yyvsp[0].node);
										char ancora2[] = "plus";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_mathop;
										(*ancora).refereTabela = NULL;
										(*ancora).valor = NULL;
										(*ancora).tipoVirtual = DecideTipo(((yyvsp[-2].node))->tipoVirtual,((yyvsp[0].node))->tipoVirtual);
										(*ancora).conversion = DecideConversao(((yyvsp[-2].node))->tipoVirtual,((yyvsp[0].node))->tipoVirtual,(*ancora).tipoVirtual);
										(yyval.node) = ancora;
										(yyvsp[-1].text) = NULL;
									}
#line 4429 "test.tab.c"
    break;

  case 90: /* mathop: mathop MINUS mathop1  */
#line 2125 "test.y"
                                                {
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 2;
										(*ancora).filhos[0] = (yyvsp[-2].node);
										(*ancora).filhos[1] = (yyvsp[0].node);
										char ancora2[] = "minus";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_mathop;
										(*ancora).refereTabela = NULL;
										(*ancora).valor = NULL;
										(*ancora).tipoVirtual = DecideTipo(((yyvsp[-2].node))->tipoVirtual,((yyvsp[0].node))->tipoVirtual);
										(*ancora).conversion = DecideConversao(((yyvsp[-2].node))->tipoVirtual,((yyvsp[0].node))->tipoVirtual,(*ancora).tipoVirtual);
										(yyval.node) = ancora;
										(yyvsp[-1].text) = NULL;
									}
#line 4449 "test.tab.c"
    break;

  case 91: /* mathop: mathop1  */
#line 2140 "test.y"
                                                                {
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 1;
										(*ancora).filhos[0] = (yyvsp[0].node);
										char ancora2[] = "mathop1";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_mathop;
										(*ancora).refereTabela = NULL;
										(*ancora).valor = NULL;
										(*ancora).conversion = None;
										(*ancora).tipoVirtual = ((yyvsp[0].node))->tipoVirtual;
										(yyval.node) = ancora;
									}
#line 4467 "test.tab.c"
    break;

  case 92: /* mathop1: mathop1 AST mathop2  */
#line 2156 "test.y"
                                                        {
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 2;
										(*ancora).filhos[0] = (yyvsp[-2].node);
										(*ancora).filhos[1] = (yyvsp[0].node);
										char ancora2[] = "ast";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_mathop1;
										(*ancora).refereTabela = NULL;
										(*ancora).valor = NULL;
										(*ancora).tipoVirtual = DecideTipo(((yyvsp[-2].node))->tipoVirtual,((yyvsp[0].node))->tipoVirtual);
										(*ancora).conversion = DecideConversao(((yyvsp[-2].node))->tipoVirtual,((yyvsp[0].node))->tipoVirtual,(*ancora).tipoVirtual);
										(yyval.node) = ancora;
										(yyvsp[-1].text) = NULL;
									}
#line 4487 "test.tab.c"
    break;

  case 93: /* mathop1: mathop1 BS mathop2  */
#line 2171 "test.y"
                                                        {
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 2;
										(*ancora).filhos[0] = (yyvsp[-2].node);
										(*ancora).filhos[1] = (yyvsp[0].node);
										char ancora2[] = "bs";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_mathop1;
										(*ancora).refereTabela = NULL;
										(*ancora).valor = NULL;
										(*ancora).tipoVirtual = DecideTipo(((yyvsp[-2].node))->tipoVirtual,((yyvsp[0].node))->tipoVirtual);
										(*ancora).conversion = DecideConversao(((yyvsp[-2].node))->tipoVirtual,((yyvsp[0].node))->tipoVirtual,(*ancora).tipoVirtual);
										(yyval.node) = ancora;
										(yyvsp[-1].text) = NULL;
									}
#line 4507 "test.tab.c"
    break;

  case 94: /* mathop1: mathop2  */
#line 2186 "test.y"
                                                                {
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 1;
										(*ancora).filhos[0] = (yyvsp[0].node);
										char ancora2[] = "mathop2";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_mathop1;
										(*ancora).refereTabela = NULL;
										(*ancora).valor = NULL;
										(*ancora).conversion = None;
										(*ancora).tipoVirtual = ((yyvsp[0].node))->tipoVirtual;
										(yyval.node) = ancora;
									}
#line 4525 "test.tab.c"
    break;

  case 95: /* mathop2: matharg  */
#line 2203 "test.y"
                                                                {
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 1;
										(*ancora).filhos[0] = (yyvsp[0].node);
										char ancora2[] = "matharg";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_mathop2;
										(*ancora).refereTabela = NULL;
										(*ancora).valor = NULL;
										(*ancora).conversion = None;
										(*ancora).tipoVirtual = ((yyvsp[0].node))->tipoVirtual;
										(yyval.node) = ancora;
									}
#line 4543 "test.tab.c"
    break;

  case 96: /* mathop2: OPENPAR mathop CLOSEPAR  */
#line 2216 "test.y"
                                                {
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 1;
										(*ancora).filhos[0] = (yyvsp[-1].node);
										char ancora2[] = "pars";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_mathop2;
										(*ancora).refereTabela = NULL;
										(*ancora).valor = NULL;
										(*ancora).conversion = None;
										(*ancora).tipoVirtual = ((yyvsp[-1].node))->tipoVirtual;
										(yyval.node) = ancora;
										(yyvsp[-2].text) = NULL;
										(yyvsp[0].text) = NULL;
									}
#line 4563 "test.tab.c"
    break;

  case 97: /* matharg: ID  */
#line 2234 "test.y"
                                                                        {
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 0;
										(*ancora).tipo = YYSYMBOL_matharg;
										char ancora2[] = "ID";
										(*ancora).nome = strdup(ancora2);
										simbolo *ancoraSimb = VerificarEscopo((yyvsp[0].text));
										if(ancoraSimb != NULL){ 
											(*ancora).refereTabela = ancoraSimb;
										}
										else{
											printf("ERRO SEMANTICO! ID %s USADO FORA DE ESCOPO!\n",(yyvsp[0].text));
											(*ancora).refereTabela = NULL;
										}
										(*ancora).valor = strdup((yyvsp[0].text));
										(*ancora).conversion = None;
										(*ancora).tipoVirtual = ConverteTableTipo((*ancoraSimb).tipo);
										free((yyvsp[0].text));
										(yyval.node) = ancora;																
									}
#line 4588 "test.tab.c"
    break;

  case 98: /* matharg: num  */
#line 2255 "test.y"
                                                                {
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 1;
										(*ancora).filhos[0] = (yyvsp[0].node);
										(*ancora).tipo = YYSYMBOL_matharg;
										char ancora2[] = "num";
										(*ancora).nome = strdup(ancora2);
										(*ancora).valor = strdup((yyvsp[0].node));
										(*ancora).refereTabela = NULL;
										(*ancora).conversion = None;
										(*ancora).tipoVirtual = ((yyvsp[0].node))->tipoVirtual;
										(yyval.node) = ancora;																
									}
#line 4606 "test.tab.c"
    break;

  case 99: /* matharg: function_call  */
#line 2268 "test.y"
                                                        {
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 1;
										(*ancora).filhos[0] = (yyvsp[0].node);
										(*ancora).tipo = YYSYMBOL_matharg;
										char ancora2[] = "function_call";
										(*ancora).nome = strdup(ancora2);
										(*ancora).valor = NULL;
										(*ancora).refereTabela = NULL;
										(*ancora).conversion = None;
										(*ancora).tipoVirtual = ((yyvsp[0].node))->tipoVirtual;
										(yyval.node) = ancora;																
									}
#line 4624 "test.tab.c"
    break;

  case 100: /* matharg: EMPTY  */
#line 2282 "test.y"
                                                                {
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 0;
										(*ancora).tipo = YYSYMBOL_matharg;
										char ancora2[] = "empty";
										(*ancora).nome = strdup(ancora2);
										(*ancora).valor = strdup((yyvsp[0].text));
										(*ancora).refereTabela = NULL;
										(*ancora).conversion = None;
										(*ancora).tipoVirtual = Set;
										(yyval.node) = ancora;																
									}
#line 4641 "test.tab.c"
    break;

  case 101: /* function_type: type  */
#line 2297 "test.y"
                                                                {
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 1;
										(*ancora).filhos[0] = (yyvsp[0].node);
										char ancora2[] = "type";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_function_type;
										(*ancora).refereTabela = NULL;
										(*ancora).valor = NULL;
										(*ancora).conversion = None;
										(*ancora).tipoVirtual = ((yyvsp[0].node))->tipoVirtual;
										(yyval.node) = ancora;
									}
#line 4659 "test.tab.c"
    break;

  case 102: /* function_type: VOID  */
#line 2311 "test.y"
                                                                {
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 0;
										(*ancora).tipo = YYSYMBOL_function_type;
										char ancora2[] = "void";
										(*ancora).nome = strdup(ancora2);
										(*ancora).valor = strdup((yyvsp[0].text));
										(*ancora).refereTabela = NULL;
										(*ancora).conversion = None;
										(*ancora).tipoVirtual = Void;
										(yyval.node) = ancora;																
									}
#line 4676 "test.tab.c"
    break;

  case 103: /* type: SET  */
#line 2329 "test.y"
                                                                {
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 0;
										char ancora2[] = "set";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_type;
										(*ancora).refereTabela = NULL;
										char ancora3[2];
										sprintf(ancora3,"%d",SET_TABLE);
										(*ancora).valor = strdup(ancora3);
										(*ancora).conversion = None;
										(*ancora).tipoVirtual = Set;
										(yyval.node) = ancora;
										(yyvsp[0].text) = NULL;
									}
#line 4696 "test.tab.c"
    break;

  case 104: /* type: INT  */
#line 2344 "test.y"
                                                                {
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 0;
										char ancora2[] = "int";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_type;
										(*ancora).refereTabela = NULL;
										char ancora3[2];
										sprintf(ancora3,"%d",NUM_TABLE);
										(*ancora).valor = strdup(ancora3);
										(*ancora).conversion = None;
										(*ancora).tipoVirtual = Int;
										(yyval.node) = ancora;
										(yyvsp[0].text) = NULL;
									}
#line 4716 "test.tab.c"
    break;

  case 105: /* type: ELEM  */
#line 2359 "test.y"
                                                                {
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 0;
										char ancora2[] = "elem";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_type;
										(*ancora).refereTabela = NULL;
										char ancora3[2];
										sprintf(ancora3,"%d",ELEM_TABLE);
										(*ancora).valor = strdup(ancora3);
										(*ancora).conversion = None;
										(*ancora).tipoVirtual = Elem;
										(yyval.node) = ancora;
										(yyvsp[0].text) = NULL;
									}
#line 4736 "test.tab.c"
    break;

  case 106: /* type: FLOAT  */
#line 2374 "test.y"
                                                                {
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 0;
										char ancora2[] = "float";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_type;
										(*ancora).refereTabela = NULL;
										char ancora3[2];
										sprintf(ancora3,"%d",NUM_TABLE);
										(*ancora).valor = strdup(ancora3);
										(*ancora).conversion = None;
										(*ancora).tipoVirtual = Float;
										(yyval.node) = ancora;
										(yyvsp[0].text) = NULL;
									}
#line 4756 "test.tab.c"
    break;

  case 107: /* num: INTEGER  */
#line 2392 "test.y"
                                                                {
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 0;
										char ancora2[] = "integer"
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_num;
										(*ancora).refereTabela = NULL;
										(*ancora).valor = strdup((yyvsp[0].text));
										(*ancora).conversion = None;
										(*ancora).tipoVirtual = Int;
										(yyval.node) = ancora;
										free((yyvsp[0].text));
									}
#line 4774 "test.tab.c"
    break;

  case 108: /* num: FLOATING  */
#line 2406 "test.y"
                                                                {
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 0;
										char ancora2[] = "floating"
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_num;
										(*ancora).refereTabela = NULL;
										(*ancora).valor = strdup((yyvsp[0].text));
										(*ancora).conversion = None;
										(*ancora).tipoVirtual = Float;
										(yyval.node) = ancora;
										free((yyvsp[0].text));
									}
#line 4792 "test.tab.c"
    break;


#line 4796 "test.tab.c"

        default: break;
      }
    if (yychar_backup != yychar)
      YY_LAC_DISCARD ("yychar change");
  }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yyesa, &yyes, &yyes_capacity, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        if (yychar != YYEMPTY)
          YY_LAC_ESTABLISH;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          goto yyexhaustedlab;
      }
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  /* If the stack popping above didn't lose the initial context for the
     current lookahead token, the shift below will for sure.  */
  YY_LAC_DISCARD ("error recovery");

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if 1
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yyes != yyesa)
    YYSTACK_FREE (yyes);
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 2421 "test.y"

/*
void yyerror(char *s){
	extern int yylineno;
	extern char *yytext;

	printf("ERRO %s NO SIMBOLO <%s> na linha <%d>\n",s,yytext,yylineno);
	exit(1);
}
*/

void yyerror(char const *s){
	printf("Linha: %d, Coluna: %d -> %s\n",linhaCount,colunaCount,s);

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
	escopoCounter = 0;
	pilhaEscopo = CriarStack(0);
	if (argc > 0){
		yyin = fopen(argv[0],"r");
	}
	else{
		yyin = stdin;
	}
	yyparse();
	fclose(yyin);
	EscreverTabela();
	printf("--------------------ARVORE SINTATICA-------------------\n");
	EscreverArvore(raiz,1);
	//yylex_destroy();
	ApagarTabela();
	LimparStack(pilhaEscopo);
	
	yylex_destroy();
	return 0;
}

