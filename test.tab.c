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

int escopoCounter;
pilha* pilhaEscopo;


void EscreverTabela(){
	printf("-----------TABELA DE SIMBOLOS----------\n");
	printf("|-------------------------------------|\n");
	simbolo *ancora = tabelaSimbolos;
	while(ancora != NULL){
		printf("%s|",(*ancora).nome);
		if((*ancora).tipo == NUM_TABLE){
			printf("variavel de numero|Escopo: %d\n",(*ancora).escopo);
		}
		else if((*ancora).tipo == FUNC_TABLE){
			printf("funcao|Escopo: %d\n",(*ancora).escopo);
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


int Top(pilha* stack){
	pilha* ancora = stack;
	while((*ancora).seguinte != NULL){
		ancora = (*ancora).seguinte;
	}
	return (*ancora).valor;
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

/*
void PreencherEscopo(no* base, int escopo){
	int i;
	no* ancora;
	simbolo* ancora = (*base).refereTabela;
	if((*base).tipo == YYSYMBOL_function_declaration || (*base).tipo == YYSYMBOL_variable_declaration) (*ancora).escopo = escopo; //Escopo é decidido na declaração
	if((*base).numFilhos > 0){
			switch((*base).tipo){
				case YYSYMBOL_statement:
					if(!strcmp((*base).nome,"curly")){
						PreencherEscopo((*base).filhos[0],escopo+1);
					}
					else {
						PreencherEscopo((*base).filhos[0],escopo);
						PreencherEscopo((*base).filhos[1],escopo);
					} 
				break;

				case YYSYMBOL_single_line_statement:
					PreencherEscopo((*base).filhos[0],escopo);
				break;

				case YYSYMBOL_comparg:
					PreencherEscopo((*base).filhos[0],escopo);
				break;

				case YYSYMBOL_comparison:
					PreencherEscopo((*base).filhos[0],escopo);
					if(strcmp((*base).nome,"not")){
						PreencherEscopo((*base).filhos[1],escopo);
					}
				break;

				case YYSYMBOL_write:
					PreencherEscopo((*base).filhos[0],escopo);
				break;

				case YYSYMBOL_writeln:
					PreencherEscopo((*base).filhos[0],escopo);
				break;

				case YYSYMBOL_return:
					PreencherEscopo((*base).filhos[0],escopo);
				break;

				case YYSYMBOL_for:
					PreencherEscopo((*base).filhos[0],escopo);
					PreencherEscopo((*base).filhos[1],escopo);
					PreencherEscopo((*base).filhos[2],escopo);
					PreencherEscopo((*base).filhos[3],escopo+1);
				break;

				case YYSYMBOL_if:
					PreencherEscopo((*base).filhos[0],escopo);
					PreencherEscopo((*base).filhos[2],escopo+1);
					if(!strcmp((*base).nome,""))
				break;
			}
	}
}

*/


#line 336 "test.tab.c"

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
  YYSYMBOL_NUM = 3,                        /* NUM  */
  YYSYMBOL_STRING = 4,                     /* STRING  */
  YYSYMBOL_CHAR = 5,                       /* CHAR  */
  YYSYMBOL_ID = 6,                         /* ID  */
  YYSYMBOL_INT = 7,                        /* INT  */
  YYSYMBOL_FLOAT = 8,                      /* FLOAT  */
  YYSYMBOL_ELEM = 9,                       /* ELEM  */
  YYSYMBOL_SET = 10,                       /* SET  */
  YYSYMBOL_IF = 11,                        /* IF  */
  YYSYMBOL_ELSE = 12,                      /* ELSE  */
  YYSYMBOL_FOR = 13,                       /* FOR  */
  YYSYMBOL_READ = 14,                      /* READ  */
  YYSYMBOL_WRITE = 15,                     /* WRITE  */
  YYSYMBOL_WRITELN = 16,                   /* WRITELN  */
  YYSYMBOL_IN = 17,                        /* IN  */
  YYSYMBOL_IS_SET = 18,                    /* IS_SET  */
  YYSYMBOL_ADD = 19,                       /* ADD  */
  YYSYMBOL_REMOVE = 20,                    /* REMOVE  */
  YYSYMBOL_EXISTS = 21,                    /* EXISTS  */
  YYSYMBOL_FORALL = 22,                    /* FORALL  */
  YYSYMBOL_RETURN = 23,                    /* RETURN  */
  YYSYMBOL_OPENPAR = 24,                   /* OPENPAR  */
  YYSYMBOL_CLOSEPAR = 25,                  /* CLOSEPAR  */
  YYSYMBOL_OPENCURLY = 26,                 /* OPENCURLY  */
  YYSYMBOL_CLOSECURLY = 27,                /* CLOSECURLY  */
  YYSYMBOL_OPENBRAC = 28,                  /* OPENBRAC  */
  YYSYMBOL_CLOSEBRAC = 29,                 /* CLOSEBRAC  */
  YYSYMBOL_LESS = 30,                      /* LESS  */
  YYSYMBOL_LE = 31,                        /* LE  */
  YYSYMBOL_EQ = 32,                        /* EQ  */
  YYSYMBOL_NEQ = 33,                       /* NEQ  */
  YYSYMBOL_GREATER = 34,                   /* GREATER  */
  YYSYMBOL_GE = 35,                        /* GE  */
  YYSYMBOL_PLUS = 36,                      /* PLUS  */
  YYSYMBOL_MINUS = 37,                     /* MINUS  */
  YYSYMBOL_DBS = 38,                       /* DBS  */
  YYSYMBOL_AST = 39,                       /* AST  */
  YYSYMBOL_BS = 40,                        /* BS  */
  YYSYMBOL_ASSIGN = 41,                    /* ASSIGN  */
  YYSYMBOL_COMMA = 42,                     /* COMMA  */
  YYSYMBOL_SEMICOLON = 43,                 /* SEMICOLON  */
  YYSYMBOL_OR = 44,                        /* OR  */
  YYSYMBOL_AND = 45,                       /* AND  */
  YYSYMBOL_NOT = 46,                       /* NOT  */
  YYSYMBOL_AMP = 47,                       /* AMP  */
  YYSYMBOL_PCENT = 48,                     /* PCENT  */
  YYSYMBOL_YYACCEPT = 49,                  /* $accept  */
  YYSYMBOL_inicio = 50,                    /* inicio  */
  YYSYMBOL_statement = 51,                 /* statement  */
  YYSYMBOL_52_1 = 52,                      /* $@1  */
  YYSYMBOL_single_line_statement = 53,     /* single_line_statement  */
  YYSYMBOL_comparg = 54,                   /* comparg  */
  YYSYMBOL_comparison = 55,                /* comparison  */
  YYSYMBOL_read = 56,                      /* read  */
  YYSYMBOL_write = 57,                     /* write  */
  YYSYMBOL_writeln = 58,                   /* writeln  */
  YYSYMBOL_return = 59,                    /* return  */
  YYSYMBOL_for = 60,                       /* for  */
  YYSYMBOL_61_2 = 61,                      /* $@2  */
  YYSYMBOL_if = 62,                        /* if  */
  YYSYMBOL_63_3 = 63,                      /* $@3  */
  YYSYMBOL_else = 64,                      /* else  */
  YYSYMBOL_65_4 = 65,                      /* $@4  */
  YYSYMBOL_conjuntoop = 66,                /* conjuntoop  */
  YYSYMBOL_conjuntoop1 = 67,               /* conjuntoop1  */
  YYSYMBOL_pertinencia = 68,               /* pertinencia  */
  YYSYMBOL_tipagem = 69,                   /* tipagem  */
  YYSYMBOL_inclusao = 70,                  /* inclusao  */
  YYSYMBOL_remocao = 71,                   /* remocao  */
  YYSYMBOL_selecao = 72,                   /* selecao  */
  YYSYMBOL_iteracao = 73,                  /* iteracao  */
  YYSYMBOL_74_5 = 74,                      /* $@5  */
  YYSYMBOL_function_call = 75,             /* function_call  */
  YYSYMBOL_args = 76,                      /* args  */
  YYSYMBOL_args1 = 77,                     /* args1  */
  YYSYMBOL_funcargs = 78,                  /* funcargs  */
  YYSYMBOL_function_declaration = 79,      /* function_declaration  */
  YYSYMBOL_80_6 = 80,                      /* $@6  */
  YYSYMBOL_assignment = 81,                /* assignment  */
  YYSYMBOL_variable_declaration = 82,      /* variable_declaration  */
  YYSYMBOL_mathop = 83,                    /* mathop  */
  YYSYMBOL_mathop1 = 84,                   /* mathop1  */
  YYSYMBOL_mathop2 = 85,                   /* mathop2  */
  YYSYMBOL_matharg = 86,                   /* matharg  */
  YYSYMBOL_type = 87                       /* type  */
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
#define YYFINAL  68
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   283

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  39
/* YYNRULES -- Number of rules.  */
#define YYNRULES  96
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  202

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   303


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
      45,    46,    47,    48
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   381,   381,   392,   407,   406,   429,   459,   473,   487,
     501,   608,   623,   637,   651,   665,   679,   705,   723,   745,
     758,   769,   783,   795,   808,   821,   834,   847,   860,   873,
     886,   902,   928,   943,   957,   976,   991,  1014,  1027,  1040,
    1055,  1055,  1089,  1088,  1113,  1132,  1144,  1156,  1156,  1175,
    1188,  1199,  1210,  1221,  1232,  1246,  1257,  1268,  1279,  1290,
    1301,  1322,  1335,  1351,  1368,  1385,  1403,  1421,  1420,  1444,
    1462,  1493,  1513,  1528,  1540,  1553,  1572,  1594,  1609,  1608,
    1644,  1667,  1693,  1706,  1719,  1733,  1746,  1759,  1774,  1785,
    1801,  1820,  1831,  1845,  1858,  1871,  1884
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
  "end of file", "error", "invalid token", "NUM", "STRING", "CHAR", "ID",
  "INT", "FLOAT", "ELEM", "SET", "IF", "ELSE", "FOR", "READ", "WRITE",
  "WRITELN", "IN", "IS_SET", "ADD", "REMOVE", "EXISTS", "FORALL", "RETURN",
  "OPENPAR", "CLOSEPAR", "OPENCURLY", "CLOSECURLY", "OPENBRAC",
  "CLOSEBRAC", "LESS", "LE", "EQ", "NEQ", "GREATER", "GE", "PLUS", "MINUS",
  "DBS", "AST", "BS", "ASSIGN", "COMMA", "SEMICOLON", "OR", "AND", "NOT",
  "AMP", "PCENT", "$accept", "inicio", "statement", "$@1",
  "single_line_statement", "comparg", "comparison", "read", "write",
  "writeln", "return", "for", "$@2", "if", "$@3", "else", "$@4",
  "conjuntoop", "conjuntoop1", "pertinencia", "tipagem", "inclusao",
  "remocao", "selecao", "iteracao", "$@5", "function_call", "args",
  "args1", "funcargs", "function_declaration", "$@6", "assignment",
  "variable_declaration", "mathop", "mathop1", "mathop2", "matharg",
  "type", YY_NULLPTR
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
     295,   296,   297,   298,   299,   300,   301,   302,   303
};
#endif

#define YYPACT_NINF (-132)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-93)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     125,  -132,    19,  -132,  -132,  -132,  -132,    12,    56,    64,
      71,    96,    99,   101,   102,   113,   126,     6,    70,  -132,
     153,  -132,   125,   111,   114,   116,   117,   125,   125,   119,
    -132,  -132,  -132,  -132,   138,   125,  -132,   125,   120,   121,
     150,   -11,  -132,  -132,   166,   112,    70,    11,   176,   178,
      15,    98,    79,    34,    34,    34,    34,   204,   199,     6,
     118,   223,  -132,   209,    13,   161,    80,   125,  -132,  -132,
    -132,  -132,  -132,  -132,  -132,  -132,  -132,    79,  -132,  -132,
    -132,   125,    79,    70,    70,    70,    70,   164,   147,   147,
     165,    13,  -132,   161,    11,   179,  -132,   167,   170,   182,
     184,   193,   169,   205,   225,    53,   206,  -132,  -132,  -132,
    -132,   138,   207,   138,   208,   212,   226,   234,  -132,   118,
     118,   118,   118,   118,   118,   118,   118,  -132,   194,  -132,
    -132,  -132,   -11,   -11,  -132,  -132,   218,   112,  -132,  -132,
    -132,   177,    11,  -132,  -132,  -132,  -132,  -132,  -132,  -132,
    -132,  -132,  -132,   196,  -132,  -132,  -132,  -132,  -132,  -132,
    -132,  -132,  -132,  -132,   235,   257,  -132,  -132,   252,   222,
    -132,  -132,   240,   227,   125,   155,  -132,   176,   125,  -132,
     218,   243,  -132,  -132,  -132,   246,   245,   125,  -132,   252,
     125,  -132,  -132,   247,  -132,   248,   250,  -132,  -132,   125,
     251,  -132
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
      11,    91,    90,    94,    96,    95,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    39,     0,     4,
       0,     2,    11,     0,     0,     0,     0,    11,    11,     0,
      50,    51,    52,    53,    54,    11,    92,    11,     0,     0,
       0,    84,    87,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    20,    18,     0,
       0,     0,    37,    21,    38,    90,     0,    11,     1,     3,
      16,    14,    15,    12,     7,     8,    17,     0,     9,     6,
      13,    11,     0,     0,     0,     0,     0,    81,    74,    74,
       0,    80,    20,    18,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    90,     0,    55,    56,    57,
      58,    59,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,     0,    89,     0,    62,
      10,    61,    82,    83,    85,    86,    77,     0,    72,    71,
      70,     0,     0,    31,    33,    34,    32,    36,    35,    63,
      64,    65,    66,     0,    19,    27,    28,    29,    30,    25,
      26,    24,    23,     5,     0,     0,    73,    42,    49,     0,
      67,    69,     0,    75,    11,     0,    44,     0,    11,    78,
      77,     0,    47,    46,    45,     0,     0,    11,    76,    49,
      11,    40,    68,     0,    43,     0,     0,    79,    48,    11,
       0,    41
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -132,  -132,   -22,  -132,  -131,   -57,   -15,  -132,  -132,  -132,
    -132,  -132,  -132,   104,  -132,    84,  -132,  -132,   -66,   -29,
     -44,   -21,     2,    37,  -132,  -132,   -13,   140,   191,   103,
    -132,  -132,   -47,  -132,    24,   -12,     1,  -132,  -129
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    20,    21,    67,    22,    61,   117,    23,    24,    25,
      26,    27,   196,    28,   174,   176,   190,    29,   106,    30,
      31,    32,    33,    34,    35,   178,    36,    90,   138,   164,
      37,   187,    38,    39,    40,    41,    42,    43,    44
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      69,    98,    62,   118,    63,    74,    75,   165,   108,    57,
     168,   129,    58,    78,    92,    79,   131,    93,     1,   100,
     101,    65,   171,   107,   112,   114,   115,   116,    85,    86,
      59,   109,    95,   108,    96,    94,    47,     1,   108,    18,
      65,    64,    66,    45,   183,   128,    63,    96,   107,    83,
      84,   165,    60,   107,   110,    15,   109,    60,    18,   130,
      46,   109,   155,   156,   157,   158,   159,   160,   161,   162,
      91,   132,   133,     1,   102,   104,    65,    45,   -60,   110,
      48,    96,     1,    66,   110,   105,   134,   135,    49,   111,
     113,   113,   113,   113,    18,    50,   -60,    12,    13,    14,
      15,     1,   103,    18,    65,   127,    96,    96,    96,    96,
      96,    96,    96,    96,   111,    88,    83,    84,    89,   111,
      51,    92,    18,    52,    93,    53,    54,   169,     1,    96,
     185,     2,     3,     4,     5,     6,     7,    55,     8,     9,
      10,    11,    94,    12,    13,    14,    15,    16,    17,    18,
      56,    19,   181,    68,    70,    77,   186,    71,     1,    72,
      73,     2,    76,    80,    81,   193,     7,    82,   195,     9,
      10,    11,    87,    12,    13,    14,    15,   200,    17,    18,
       1,   182,    97,     2,    99,    45,    83,    84,   136,   137,
     140,     9,    10,    11,   146,    12,    13,    14,    15,     1,
      17,    18,     2,   167,   141,    83,    84,   143,    46,   144,
       9,    10,    11,   142,    12,    13,    14,    15,   145,    17,
      18,   163,   170,    45,   -90,     3,     4,     5,     6,   -91,
     147,   149,   150,   151,   -92,   -90,   -90,   152,   -90,   -90,
     -91,   -91,   -90,   -91,   -91,   -92,   -92,   -91,   -92,   -92,
     148,   153,   -92,   119,   120,   121,   122,   123,   124,   154,
     172,    83,    84,   173,   175,   177,   179,   125,   126,   180,
     189,   191,   192,   194,   197,   198,   199,   166,   201,   184,
     139,     0,     0,   188
};

static const yytype_int16 yycheck[] =
{
      22,    48,    17,    60,    17,    27,    28,   136,    52,     3,
     141,    77,     6,    35,     3,    37,    82,     6,     3,     4,
       5,     6,   153,    52,    53,    54,    55,    56,    39,    40,
      24,    52,    47,    77,    47,    24,    24,     3,    82,    24,
       6,    17,    18,    24,   175,    67,    59,    60,    77,    36,
      37,   180,    46,    82,    52,    21,    77,    46,    24,    81,
      41,    82,   119,   120,   121,   122,   123,   124,   125,   126,
      46,    83,    84,     3,    50,    51,     6,    24,    25,    77,
      24,    94,     3,    59,    82,     6,    85,    86,    24,    52,
      53,    54,    55,    56,    24,    24,    43,    18,    19,    20,
      21,     3,     4,    24,     6,    25,   119,   120,   121,   122,
     123,   124,   125,   126,    77,     3,    36,    37,     6,    82,
      24,     3,    24,    24,     6,    24,    24,   142,     3,   142,
     177,     6,     7,     8,     9,    10,    11,    24,    13,    14,
      15,    16,    24,    18,    19,    20,    21,    22,    23,    24,
      24,    26,   174,     0,    43,    17,   178,    43,     3,    43,
      43,     6,    43,    43,    43,   187,    11,    17,   190,    14,
      15,    16,     6,    18,    19,    20,    21,   199,    23,    24,
       3,    26,     6,     6,     6,    24,    36,    37,    24,    42,
      25,    14,    15,    16,    25,    18,    19,    20,    21,     3,
      23,    24,     6,    26,    25,    36,    37,    25,    41,    25,
      14,    15,    16,    43,    18,    19,    20,    21,    25,    23,
      24,    27,    26,    24,    25,     7,     8,     9,    10,    25,
      25,    25,    25,    25,    25,    36,    37,    25,    39,    40,
      36,    37,    43,    39,    40,    36,    37,    43,    39,    40,
      25,    25,    43,    30,    31,    32,    33,    34,    35,    25,
      25,    36,    37,     6,    12,    43,    26,    44,    45,    42,
      27,    25,    27,   189,    27,    27,    26,   137,    27,   175,
      89,    -1,    -1,   180
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     6,     7,     8,     9,    10,    11,    13,    14,
      15,    16,    18,    19,    20,    21,    22,    23,    24,    26,
      50,    51,    53,    56,    57,    58,    59,    60,    62,    66,
      68,    69,    70,    71,    72,    73,    75,    79,    81,    82,
      83,    84,    85,    86,    87,    24,    41,    24,    24,    24,
      24,    24,    24,    24,    24,    24,    24,     3,     6,    24,
      46,    54,    55,    75,    83,     6,    83,    52,     0,    51,
      43,    43,    43,    43,    51,    51,    43,    17,    51,    51,
      43,    43,    17,    36,    37,    39,    40,     6,     3,     6,
      76,    83,     3,     6,    24,    55,    75,     6,    81,     6,
       4,     5,    83,     4,    83,     6,    67,    68,    69,    70,
      71,    72,    68,    72,    68,    68,    68,    55,    54,    30,
      31,    32,    33,    34,    35,    44,    45,    25,    51,    67,
      51,    67,    84,    84,    85,    85,    24,    42,    77,    77,
      25,    25,    43,    25,    25,    25,    25,    25,    25,    25,
      25,    25,    25,    25,    25,    54,    54,    54,    54,    54,
      54,    54,    54,    27,    78,    87,    76,    26,    53,    55,
      26,    53,    25,     6,    63,    12,    64,    43,    74,    26,
      42,    51,    26,    53,    62,    81,    51,    80,    78,    27,
      65,    25,    27,    51,    64,    51,    61,    27,    27,    26,
      51,    27
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    49,    50,    51,    52,    51,    51,    51,    51,    51,
      51,    51,    53,    53,    53,    53,    53,    53,    54,    54,
      54,    54,    55,    55,    55,    55,    55,    55,    55,    55,
      55,    56,    57,    57,    57,    58,    58,    59,    59,    59,
      61,    60,    63,    62,    62,    64,    64,    65,    64,    64,
      66,    66,    66,    66,    66,    67,    67,    67,    67,    67,
      67,    68,    68,    69,    70,    71,    72,    74,    73,    73,
      75,    76,    76,    77,    77,    78,    78,    78,    80,    79,
      81,    82,    83,    83,    83,    84,    84,    84,    85,    85,
      86,    86,    86,    87,    87,    87,    87
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     4,     2,     2,     2,     2,
       3,     0,     2,     2,     2,     2,     2,     2,     1,     3,
       1,     1,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     4,     4,     4,     4,     4,     4,     2,     2,     1,
       0,    12,     0,     9,     6,     2,     2,     0,     5,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     4,     4,     4,     4,     0,     8,     5,
       4,     2,     2,     2,     0,     2,     4,     0,     0,     9,
       3,     2,     3,     3,     1,     3,     3,     1,     1,     3,
       1,     1,     1,     1,     1,     1,     1
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
    case YYSYMBOL_NUM: /* NUM  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1754 "test.tab.c"
        break;

    case YYSYMBOL_STRING: /* STRING  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1760 "test.tab.c"
        break;

    case YYSYMBOL_CHAR: /* CHAR  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1766 "test.tab.c"
        break;

    case YYSYMBOL_ID: /* ID  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1772 "test.tab.c"
        break;

    case YYSYMBOL_INT: /* INT  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1778 "test.tab.c"
        break;

    case YYSYMBOL_FLOAT: /* FLOAT  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1784 "test.tab.c"
        break;

    case YYSYMBOL_ELEM: /* ELEM  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1790 "test.tab.c"
        break;

    case YYSYMBOL_SET: /* SET  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1796 "test.tab.c"
        break;

    case YYSYMBOL_IF: /* IF  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1802 "test.tab.c"
        break;

    case YYSYMBOL_ELSE: /* ELSE  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1808 "test.tab.c"
        break;

    case YYSYMBOL_FOR: /* FOR  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1814 "test.tab.c"
        break;

    case YYSYMBOL_READ: /* READ  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1820 "test.tab.c"
        break;

    case YYSYMBOL_WRITE: /* WRITE  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1826 "test.tab.c"
        break;

    case YYSYMBOL_WRITELN: /* WRITELN  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1832 "test.tab.c"
        break;

    case YYSYMBOL_IN: /* IN  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1838 "test.tab.c"
        break;

    case YYSYMBOL_IS_SET: /* IS_SET  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1844 "test.tab.c"
        break;

    case YYSYMBOL_ADD: /* ADD  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1850 "test.tab.c"
        break;

    case YYSYMBOL_REMOVE: /* REMOVE  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1856 "test.tab.c"
        break;

    case YYSYMBOL_EXISTS: /* EXISTS  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1862 "test.tab.c"
        break;

    case YYSYMBOL_FORALL: /* FORALL  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1868 "test.tab.c"
        break;

    case YYSYMBOL_RETURN: /* RETURN  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1874 "test.tab.c"
        break;

    case YYSYMBOL_OPENPAR: /* OPENPAR  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1880 "test.tab.c"
        break;

    case YYSYMBOL_CLOSEPAR: /* CLOSEPAR  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1886 "test.tab.c"
        break;

    case YYSYMBOL_OPENCURLY: /* OPENCURLY  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1892 "test.tab.c"
        break;

    case YYSYMBOL_CLOSECURLY: /* CLOSECURLY  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1898 "test.tab.c"
        break;

    case YYSYMBOL_OPENBRAC: /* OPENBRAC  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1904 "test.tab.c"
        break;

    case YYSYMBOL_CLOSEBRAC: /* CLOSEBRAC  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1910 "test.tab.c"
        break;

    case YYSYMBOL_LESS: /* LESS  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1916 "test.tab.c"
        break;

    case YYSYMBOL_LE: /* LE  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1922 "test.tab.c"
        break;

    case YYSYMBOL_EQ: /* EQ  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1928 "test.tab.c"
        break;

    case YYSYMBOL_NEQ: /* NEQ  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1934 "test.tab.c"
        break;

    case YYSYMBOL_GREATER: /* GREATER  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1940 "test.tab.c"
        break;

    case YYSYMBOL_GE: /* GE  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1946 "test.tab.c"
        break;

    case YYSYMBOL_PLUS: /* PLUS  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1952 "test.tab.c"
        break;

    case YYSYMBOL_MINUS: /* MINUS  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1958 "test.tab.c"
        break;

    case YYSYMBOL_DBS: /* DBS  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1964 "test.tab.c"
        break;

    case YYSYMBOL_AST: /* AST  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1970 "test.tab.c"
        break;

    case YYSYMBOL_BS: /* BS  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1976 "test.tab.c"
        break;

    case YYSYMBOL_ASSIGN: /* ASSIGN  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1982 "test.tab.c"
        break;

    case YYSYMBOL_COMMA: /* COMMA  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1988 "test.tab.c"
        break;

    case YYSYMBOL_SEMICOLON: /* SEMICOLON  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1994 "test.tab.c"
        break;

    case YYSYMBOL_OR: /* OR  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2000 "test.tab.c"
        break;

    case YYSYMBOL_AND: /* AND  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2006 "test.tab.c"
        break;

    case YYSYMBOL_NOT: /* NOT  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2012 "test.tab.c"
        break;

    case YYSYMBOL_AMP: /* AMP  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2018 "test.tab.c"
        break;

    case YYSYMBOL_PCENT: /* PCENT  */
#line 358 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2024 "test.tab.c"
        break;

    case YYSYMBOL_statement: /* statement  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2030 "test.tab.c"
        break;

    case YYSYMBOL_single_line_statement: /* single_line_statement  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2036 "test.tab.c"
        break;

    case YYSYMBOL_comparg: /* comparg  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2042 "test.tab.c"
        break;

    case YYSYMBOL_comparison: /* comparison  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2048 "test.tab.c"
        break;

    case YYSYMBOL_read: /* read  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2054 "test.tab.c"
        break;

    case YYSYMBOL_write: /* write  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2060 "test.tab.c"
        break;

    case YYSYMBOL_writeln: /* writeln  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2066 "test.tab.c"
        break;

    case YYSYMBOL_return: /* return  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2072 "test.tab.c"
        break;

    case YYSYMBOL_for: /* for  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2078 "test.tab.c"
        break;

    case YYSYMBOL_if: /* if  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2084 "test.tab.c"
        break;

    case YYSYMBOL_else: /* else  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2090 "test.tab.c"
        break;

    case YYSYMBOL_conjuntoop: /* conjuntoop  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2096 "test.tab.c"
        break;

    case YYSYMBOL_conjuntoop1: /* conjuntoop1  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2102 "test.tab.c"
        break;

    case YYSYMBOL_pertinencia: /* pertinencia  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2108 "test.tab.c"
        break;

    case YYSYMBOL_tipagem: /* tipagem  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2114 "test.tab.c"
        break;

    case YYSYMBOL_inclusao: /* inclusao  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2120 "test.tab.c"
        break;

    case YYSYMBOL_remocao: /* remocao  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2126 "test.tab.c"
        break;

    case YYSYMBOL_selecao: /* selecao  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2132 "test.tab.c"
        break;

    case YYSYMBOL_iteracao: /* iteracao  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2138 "test.tab.c"
        break;

    case YYSYMBOL_function_call: /* function_call  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2144 "test.tab.c"
        break;

    case YYSYMBOL_args: /* args  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2150 "test.tab.c"
        break;

    case YYSYMBOL_args1: /* args1  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2156 "test.tab.c"
        break;

    case YYSYMBOL_funcargs: /* funcargs  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2162 "test.tab.c"
        break;

    case YYSYMBOL_function_declaration: /* function_declaration  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2168 "test.tab.c"
        break;

    case YYSYMBOL_assignment: /* assignment  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2174 "test.tab.c"
        break;

    case YYSYMBOL_variable_declaration: /* variable_declaration  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2180 "test.tab.c"
        break;

    case YYSYMBOL_mathop: /* mathop  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2186 "test.tab.c"
        break;

    case YYSYMBOL_mathop1: /* mathop1  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2192 "test.tab.c"
        break;

    case YYSYMBOL_mathop2: /* mathop2  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2198 "test.tab.c"
        break;

    case YYSYMBOL_matharg: /* matharg  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2204 "test.tab.c"
        break;

    case YYSYMBOL_type: /* type  */
#line 358 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2210 "test.tab.c"
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
#line 381 "test.y"
                                                                                        {
													raiz = (yyvsp[0].node);
													//(*raiz).escopo = 1;
												}
#line 2497 "test.tab.c"
    break;

  case 3: /* statement: single_line_statement statement  */
#line 392 "test.y"
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
													(yyval.node) = ancora;
												}
#line 2516 "test.tab.c"
    break;

  case 4: /* $@1: %empty  */
#line 407 "test.y"
                                                                                                {
													escopoCounter++;
													Push(pilhaEscopo,CriarStack(escopoCounter));
													(yyvsp[0].text) = NULL;
												}
#line 2526 "test.tab.c"
    break;

  case 5: /* statement: OPENCURLY $@1 statement CLOSECURLY  */
#line 412 "test.y"
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
													Pop(pilhaEscopo);
												}
#line 2547 "test.tab.c"
    break;

  case 6: /* statement: function_declaration statement  */
#line 429 "test.y"
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
													(yyval.node) = ancora;
												}
#line 2566 "test.tab.c"
    break;

  case 7: /* statement: for statement  */
#line 459 "test.y"
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
													(yyval.node) = ancora;
												}
#line 2585 "test.tab.c"
    break;

  case 8: /* statement: if statement  */
#line 473 "test.y"
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
													(yyval.node) = ancora;
												}
#line 2604 "test.tab.c"
    break;

  case 9: /* statement: iteracao statement  */
#line 487 "test.y"
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
													(yyval.node) = ancora;
												}
#line 2623 "test.tab.c"
    break;

  case 10: /* statement: variable_declaration SEMICOLON statement  */
#line 501 "test.y"
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
												}
#line 2643 "test.tab.c"
    break;

  case 11: /* statement: %empty  */
#line 608 "test.y"
                                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 0;
													char ancora2[] = "epsilon";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
												}
#line 2658 "test.tab.c"
    break;

  case 12: /* single_line_statement: return SEMICOLON  */
#line 623 "test.y"
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
												}
#line 2676 "test.tab.c"
    break;

  case 13: /* single_line_statement: assignment SEMICOLON  */
#line 637 "test.y"
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
												}
#line 2694 "test.tab.c"
    break;

  case 14: /* single_line_statement: write SEMICOLON  */
#line 651 "test.y"
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
												}
#line 2712 "test.tab.c"
    break;

  case 15: /* single_line_statement: writeln SEMICOLON  */
#line 665 "test.y"
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
												}
#line 2730 "test.tab.c"
    break;

  case 16: /* single_line_statement: read SEMICOLON  */
#line 679 "test.y"
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
												}
#line 2748 "test.tab.c"
    break;

  case 17: /* single_line_statement: conjuntoop SEMICOLON  */
#line 705 "test.y"
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
												}
#line 2766 "test.tab.c"
    break;

  case 18: /* comparg: ID  */
#line 723 "test.y"
                                                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(yyval.node) = ancora;
													(*ancora).tipo = YYSYMBOL_comparg;
													(*ancora).numFilhos = 0;
													(*ancora).valor = strdup((yyvsp[0].text));
													char ancora2[] = "ID";
													(*ancora).nome = strdup(ancora2);
													simbolo *ancoraSimb = ProcurarTabela((yyvsp[0].text));
													if(ancoraSimb != NULL){ //Vamos começar a usar a tabela de simbolos! Se não acharmos este ID na tabela, devemos colocar-lo lá, mas sem valor! Só em assignment a gente coloca valor
														(*ancora).refereTabela = ancoraSimb;
													}
													else{
														//(*ancora).refereTabela = CriarSimbolo($1,0,NULL,escopoCounter); Tirei essa linha pq é hora de acusar erros semânticos! Se acharmos um ID que não foi declarado, temos q dar erro!
														printf("ERRO SEMANTICO! ID %s USADO ANTES DE SER DECLARADO!\n",(yyvsp[0].text));
													}
													
													free((yyvsp[0].text));

													

												}
#line 2793 "test.tab.c"
    break;

  case 19: /* comparg: OPENPAR comparison CLOSEPAR  */
#line 745 "test.y"
                                                                        {	
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[-1].node);
													(*ancora).numFilhos = 1;
													(*ancora).tipo = YYSYMBOL_comparg;
													char ancora2[] = "comparison";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
													(yyvsp[-2].text) = NULL;
													(yyvsp[0].text) = NULL;
												}
#line 2811 "test.tab.c"
    break;

  case 20: /* comparg: NUM  */
#line 758 "test.y"
                                                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 0;
													(*ancora).tipo = YYSYMBOL_comparg;
													char ancora2[] = "NUM";
													(*ancora).nome = strdup(ancora2);
													(*ancora).valor = strdup((yyvsp[0].text));
													(*ancora).refereTabela = NULL;
													(yyval.node) = ancora;
													free((yyvsp[0].text));
												}
#line 2827 "test.tab.c"
    break;

  case 21: /* comparg: function_call  */
#line 769 "test.y"
                                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[0].node);
													(*ancora).tipo = YYSYMBOL_comparg;
													char ancora2[] = "function_call";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
												}
#line 2843 "test.tab.c"
    break;

  case 22: /* comparison: NOT comparg  */
#line 783 "test.y"
                                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[0].node);
													(*ancora).numFilhos = 1;
													(*ancora).tipo = YYSYMBOL_comparison;
													char ancora2[] = "not";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
													(yyvsp[-1].text) = NULL;
												}
#line 2860 "test.tab.c"
    break;

  case 23: /* comparison: comparg AND comparg  */
#line 795 "test.y"
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
													(yyval.node) = ancora;
													(yyvsp[-1].text) = NULL;
												}
#line 2878 "test.tab.c"
    break;

  case 24: /* comparison: comparg OR comparg  */
#line 808 "test.y"
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
													(yyval.node) = ancora;
													(yyvsp[-1].text) = NULL;
												}
#line 2896 "test.tab.c"
    break;

  case 25: /* comparison: comparg GREATER comparg  */
#line 821 "test.y"
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
													(yyval.node) = ancora;
													(yyvsp[-1].text) = NULL;
												}
#line 2914 "test.tab.c"
    break;

  case 26: /* comparison: comparg GE comparg  */
#line 834 "test.y"
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
													(yyval.node) = ancora;
													(yyvsp[-1].text) = NULL;
												}
#line 2932 "test.tab.c"
    break;

  case 27: /* comparison: comparg LESS comparg  */
#line 847 "test.y"
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
													(yyval.node) = ancora;
													(yyvsp[-1].text) = NULL;
												}
#line 2950 "test.tab.c"
    break;

  case 28: /* comparison: comparg LE comparg  */
#line 860 "test.y"
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
													(yyval.node) = ancora;
													(yyvsp[-1].text) = NULL;
												}
#line 2968 "test.tab.c"
    break;

  case 29: /* comparison: comparg EQ comparg  */
#line 873 "test.y"
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
													(yyval.node) = ancora;
													(yyvsp[-1].text) = NULL;
												}
#line 2986 "test.tab.c"
    break;

  case 30: /* comparison: comparg NEQ comparg  */
#line 886 "test.y"
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
													(yyval.node) = ancora;
													(yyvsp[-1].text) = NULL;
												}
#line 3004 "test.tab.c"
    break;

  case 31: /* read: READ OPENPAR ID CLOSEPAR  */
#line 902 "test.y"
                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 0;
													(*ancora).tipo = YYSYMBOL_read;
													char ancora2[] = "read";
													(*ancora).nome = strdup(ancora2);
													simbolo *ancoraSimb = ProcurarTabela((yyvsp[-3].text));
													if(ancoraSimb != NULL){ 
														(*ancora).refereTabela = ancoraSimb;
													}
													else{
														//(*ancora).refereTabela = CriarSimbolo($1,0,NULL);
														printf("ERRO SEMANTICO! ID %s USADO ANTES DE SER DECLARADO!\n",(yyvsp[-1].text));

													}
													(yyval.node) = ancora;
													(*ancora).valor = strdup((yyvsp[-1].text));
													free((yyvsp[-1].text));
													(yyvsp[-3].text) = NULL;
													(yyvsp[-2].text) = NULL;
													(yyvsp[0].text) = NULL;
												}
#line 3031 "test.tab.c"
    break;

  case 32: /* write: WRITE OPENPAR mathop CLOSEPAR  */
#line 928 "test.y"
                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[-1].node);
													(*ancora).numFilhos = 1;
													(*ancora).tipo = YYSYMBOL_write;
													char ancora2[] = "mathop";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
													(yyvsp[-3].text) = NULL;
													(yyvsp[-2].text) = NULL;
													(yyvsp[0].text) = NULL;
												}
#line 3050 "test.tab.c"
    break;

  case 33: /* write: WRITE OPENPAR STRING CLOSEPAR  */
#line 943 "test.y"
                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 0;
													(*ancora).tipo = YYSYMBOL_write;
													char ancora2[] = "string";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = strdup((yyvsp[-1].text));
													(yyval.node) = ancora;
													free((yyvsp[-1].text));
													(yyvsp[-3].text) = NULL;
													(yyvsp[-2].text) = NULL;
													(yyvsp[0].text) = NULL;
												}
#line 3069 "test.tab.c"
    break;

  case 34: /* write: WRITE OPENPAR CHAR CLOSEPAR  */
#line 957 "test.y"
                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 0;
													(*ancora).tipo = YYSYMBOL_write;
													char ancora2[] = "char";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = strdup((yyvsp[-1].text));
													(yyval.node) = ancora;
													free((yyvsp[-1].text));
													(yyvsp[-3].text) = NULL;
													(yyvsp[-2].text) = NULL;
													(yyvsp[0].text) = NULL;
												}
#line 3088 "test.tab.c"
    break;

  case 35: /* writeln: WRITELN OPENPAR mathop CLOSEPAR  */
#line 976 "test.y"
                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[-1].node);
													(*ancora).numFilhos = 1;
													(*ancora).tipo = YYSYMBOL_writeln;
													char ancora2[] = "mathop";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
													(yyvsp[-3].text) = NULL;
													(yyvsp[-2].text) = NULL;
													(yyvsp[0].text) = NULL;
												}
#line 3107 "test.tab.c"
    break;

  case 36: /* writeln: WRITELN OPENPAR STRING CLOSEPAR  */
#line 991 "test.y"
                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 0;
													(*ancora).tipo = YYSYMBOL_writeln;
													char ancora2[] = "string";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = strdup((yyvsp[-1].text));
													(yyval.node) = ancora;
													free((yyvsp[-1].text));
													(yyvsp[-3].text) = NULL;
													(yyvsp[-2].text) = NULL;
													(yyvsp[0].text) = NULL;
												}
#line 3126 "test.tab.c"
    break;

  case 37: /* return: RETURN comparison  */
#line 1014 "test.y"
                                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[0].node);
													(*ancora).numFilhos = 1;
													(*ancora).tipo = YYSYMBOL_return;
													char ancora2[] = "comparison";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
													(yyvsp[-1].text) = NULL;
													//free($3);
												}
#line 3144 "test.tab.c"
    break;

  case 38: /* return: RETURN mathop  */
#line 1027 "test.y"
                                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).filhos[0] = (yyvsp[0].node);
													(*ancora).numFilhos = 1;
													(*ancora).tipo = YYSYMBOL_return;
													char ancora2[] = "mathop";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
													(yyvsp[-1].text) = NULL;
													//free($3);
												}
#line 3162 "test.tab.c"
    break;

  case 39: /* return: RETURN  */
#line 1040 "test.y"
                                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 0;
													(*ancora).tipo = YYSYMBOL_return;
													char ancora2[] = "null";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
													(yyvsp[0].text) = NULL;
													//free($2);
												}
#line 3179 "test.tab.c"
    break;

  case 40: /* $@2: %empty  */
#line 1055 "test.y"
                                                                                                                                                        {
																													escopoCounter++;
																													Push(pilhaEscopo,CriarStack(escopoCounter));
																													(yyvsp[-7].text) = NULL;
																													(yyvsp[-6].text) = NULL;
																													(yyvsp[-4].text) = NULL;
																													(yyvsp[-2].text) = NULL;
																													(yyvsp[0].text) = NULL;
																													
																													
																												}
#line 3195 "test.tab.c"
    break;

  case 41: /* for: FOR OPENPAR assignment SEMICOLON comparison SEMICOLON assignment CLOSEPAR $@2 OPENCURLY statement CLOSECURLY  */
#line 1066 "test.y"
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
																													(yyvsp[0].text) = NULL;
																													(yyvsp[-2].text) = NULL;
																													(yyval.node) = ancora;
																													Pop(pilhaEscopo);
																												}
#line 3218 "test.tab.c"
    break;

  case 42: /* $@3: %empty  */
#line 1089 "test.y"
                                                                                                                                                                                                                        {
																												escopoCounter++;
																												Push(pilhaEscopo,CriarStack(escopoCounter));
																												
																											}
#line 3228 "test.tab.c"
    break;

  case 43: /* if: IF OPENPAR comparison CLOSEPAR OPENCURLY $@3 statement CLOSECURLY else  */
#line 1094 "test.y"
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
																												(yyvsp[-1].text) = NULL;
																												Pop(pilhaEscopo);
																												(yyval.node) = ancora;
																											}
#line 3252 "test.tab.c"
    break;

  case 44: /* if: IF OPENPAR comparison CLOSEPAR single_line_statement else  */
#line 1113 "test.y"
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
																												(yyval.node) = ancora;
																												(yyvsp[-5].text) = NULL;
																												(yyvsp[-4].text) = NULL;
																												(yyvsp[-2].text) = NULL;
																											}
#line 3273 "test.tab.c"
    break;

  case 45: /* else: ELSE if  */
#line 1132 "test.y"
                                                                                                {
														no* ancora = (no*)malloc(sizeof(no));
														(*ancora).filhos[0] = (yyvsp[0].node);
														(*ancora).numFilhos = 1;
														char ancora2[] = "if";
														(*ancora).nome = strdup(ancora2);
														(*ancora).tipo = YYSYMBOL_else;
														(*ancora).refereTabela = NULL;
														(*ancora).valor = NULL;
														(yyval.node) = ancora;
														free((yyvsp[-1].text));
													}
#line 3290 "test.tab.c"
    break;

  case 46: /* else: ELSE single_line_statement  */
#line 1144 "test.y"
                                                                                {
														no* ancora = (no*)malloc(sizeof(no));
														(*ancora).filhos[0] = (yyvsp[0].node);
														(*ancora).numFilhos = 1;
														char ancora2[] = "single_line_statement";
														(*ancora).nome = strdup(ancora2);
														(*ancora).tipo = YYSYMBOL_else;
														(*ancora).refereTabela = NULL;
														(*ancora).valor = NULL;
														(yyval.node) = ancora;
														(yyvsp[-1].text) = NULL;
													}
#line 3307 "test.tab.c"
    break;

  case 47: /* $@4: %empty  */
#line 1156 "test.y"
                                                                                        {
														escopoCounter++;
														Push(pilhaEscopo,CriarStack(escopoCounter));
														(yyvsp[-1].text) = NULL;
														(yyvsp[0].text) = NULL;
													}
#line 3318 "test.tab.c"
    break;

  case 48: /* else: ELSE OPENCURLY $@4 statement CLOSECURLY  */
#line 1162 "test.y"
                                                                                {
														no* ancora = (no*)malloc(sizeof(no));
														(*ancora).filhos[0] = (yyvsp[-1].node);
														(*ancora).numFilhos = 1;
														char ancora2[] = "curly";
														(*ancora).nome = strdup(ancora2);
														(*ancora).tipo = YYSYMBOL_else;
														(*ancora).refereTabela = NULL;
														(*ancora).valor = NULL;
														(yyval.node) = ancora;
														(yyvsp[0].text) = NULL;
														Pop(pilhaEscopo);
													}
#line 3336 "test.tab.c"
    break;

  case 49: /* else: %empty  */
#line 1175 "test.y"
                                                                                                {
														no* ancora = (no*)malloc(sizeof(no));
														(*ancora).numFilhos = 0;
														char ancora2[] = "epsilon";
														(*ancora).nome = strdup(ancora2);
														(*ancora).tipo = YYSYMBOL_else;
														(*ancora).refereTabela = NULL;
														(*ancora).valor = NULL;
														(yyval.node) = ancora;
													}
#line 3351 "test.tab.c"
    break;

  case 50: /* conjuntoop: pertinencia  */
#line 1188 "test.y"
                                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[0].node);
													(*ancora).tipo = YYSYMBOL_conjuntoop;
													char ancora2[] = "pertinencia";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
												}
#line 3367 "test.tab.c"
    break;

  case 51: /* conjuntoop: tipagem  */
#line 1199 "test.y"
                                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[0].node);
													(*ancora).tipo = YYSYMBOL_conjuntoop;
													char ancora2[] = "tipagem";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
												}
#line 3383 "test.tab.c"
    break;

  case 52: /* conjuntoop: inclusao  */
#line 1210 "test.y"
                                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[0].node);
													(*ancora).tipo = YYSYMBOL_conjuntoop;
													char ancora2[] = "inclusao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
												}
#line 3399 "test.tab.c"
    break;

  case 53: /* conjuntoop: remocao  */
#line 1221 "test.y"
                                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[0].node);
													(*ancora).tipo = YYSYMBOL_conjuntoop;
													char ancora2[] = "remocao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
												}
#line 3415 "test.tab.c"
    break;

  case 54: /* conjuntoop: selecao  */
#line 1232 "test.y"
                                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[0].node);
													(*ancora).tipo = YYSYMBOL_conjuntoop;
													char ancora2[] = "selecao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
												}
#line 3431 "test.tab.c"
    break;

  case 55: /* conjuntoop1: pertinencia  */
#line 1246 "test.y"
                                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[0].node);
													(*ancora).tipo = YYSYMBOL_conjuntoop1;
													char ancora2[] = "pertinencia";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
												}
#line 3447 "test.tab.c"
    break;

  case 56: /* conjuntoop1: tipagem  */
#line 1257 "test.y"
                                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[0].node);
													(*ancora).tipo = YYSYMBOL_conjuntoop1;
													char ancora2[] = "tipagem";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
												}
#line 3463 "test.tab.c"
    break;

  case 57: /* conjuntoop1: inclusao  */
#line 1268 "test.y"
                                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[0].node);
													(*ancora).tipo = YYSYMBOL_conjuntoop1;
													char ancora2[] = "inclusao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
												}
#line 3479 "test.tab.c"
    break;

  case 58: /* conjuntoop1: remocao  */
#line 1279 "test.y"
                                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[0].node);
													(*ancora).tipo = YYSYMBOL_conjuntoop1;
													char ancora2[] = "remocao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
												}
#line 3495 "test.tab.c"
    break;

  case 59: /* conjuntoop1: selecao  */
#line 1290 "test.y"
                                                                                        {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[0].node);
													(*ancora).tipo = YYSYMBOL_conjuntoop1;
													char ancora2[] = "selecao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
												}
#line 3511 "test.tab.c"
    break;

  case 60: /* conjuntoop1: ID  */
#line 1301 "test.y"
                                                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 0;
													(*ancora).tipo = YYSYMBOL_conjuntoop1;
													char ancora2[] = "ID";
													(*ancora).nome = strdup(ancora2);
													simbolo *ancoraSimb = ProcurarTabela((yyvsp[0].text));
													if(ancoraSimb != NULL){
														(*ancora).refereTabela = ancoraSimb;
													}
													else{
														//(*ancora).refereTabela = CriarSimbolo($1,0,NULL);
														printf("ERRO SEMANTICO! ID %s USADO ANTES DE SER DECLARADO!\n",(yyvsp[0].text));
													}
													(*ancora).valor = strdup((yyvsp[0].text));
													free((yyvsp[0].text));
													(yyval.node) = ancora;
												}
#line 3534 "test.tab.c"
    break;

  case 61: /* pertinencia: mathop IN conjuntoop1  */
#line 1322 "test.y"
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
													(yyval.node) = ancora;
													(yyvsp[-1].text) = NULL;
												}
#line 3552 "test.tab.c"
    break;

  case 62: /* pertinencia: selecao IN conjuntoop1  */
#line 1335 "test.y"
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
													(yyval.node) = ancora;
													(yyvsp[-1].text) = NULL;
												}
#line 3570 "test.tab.c"
    break;

  case 63: /* tipagem: IS_SET OPENPAR conjuntoop1 CLOSEPAR  */
#line 1351 "test.y"
                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[-1].node);
													(*ancora).tipo = YYSYMBOL_tipagem;
													char ancora2[] = "tipagem";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
													(yyvsp[-3].text) = NULL;
													(yyvsp[-2].text) = NULL;
													(yyvsp[0].text) = NULL;
												}
#line 3589 "test.tab.c"
    break;

  case 64: /* inclusao: ADD OPENPAR pertinencia CLOSEPAR  */
#line 1368 "test.y"
                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[-1].node);
													(*ancora).tipo = YYSYMBOL_inclusao;
													char ancora2[] = "inclusao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
													(yyvsp[-3].text) = NULL;
													(yyvsp[-2].text) = NULL;
													(yyvsp[0].text) = NULL;
												}
#line 3608 "test.tab.c"
    break;

  case 65: /* remocao: REMOVE OPENPAR pertinencia CLOSEPAR  */
#line 1385 "test.y"
                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[-1].node);
													(*ancora).tipo = YYSYMBOL_remocao;
													char ancora2[] = "remocao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
													(yyvsp[-3].text) = NULL;
													(yyvsp[-2].text) = NULL;
													(yyvsp[0].text) = NULL;
												}
#line 3627 "test.tab.c"
    break;

  case 66: /* selecao: EXISTS OPENPAR pertinencia CLOSEPAR  */
#line 1403 "test.y"
                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 1;
													(*ancora).filhos[0] = (yyvsp[-1].node);
													(*ancora).tipo = YYSYMBOL_conjuntoop1;
													char ancora2[] = "iteracao";
													(*ancora).nome = strdup(ancora2);
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
													(yyvsp[-3].text) = NULL;
													(yyvsp[-2].text) = NULL;
													(yyvsp[0].text) = NULL;
												}
#line 3646 "test.tab.c"
    break;

  case 67: /* $@5: %empty  */
#line 1421 "test.y"
                                                                                                                                                        {
																				escopoCounter++;
																				Push(pilhaEscopo,CriarStack(escopoCounter));
																				
																			}
#line 3656 "test.tab.c"
    break;

  case 68: /* iteracao: FORALL OPENPAR pertinencia CLOSEPAR OPENCURLY $@5 statement CLOSECURLY  */
#line 1426 "test.y"
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
																				(yyvsp[0].text) = NULL;
																				Pop(pilhaEscopo);
																				(yyval.node) = ancora;
																			}
#line 3679 "test.tab.c"
    break;

  case 69: /* iteracao: FORALL OPENPAR pertinencia CLOSEPAR single_line_statement  */
#line 1444 "test.y"
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
																				(yyval.node) = ancora;
																				(yyvsp[-4].text) = NULL;
																				(yyvsp[-3].text) = NULL;
																				(yyvsp[-1].text) = NULL;
																			}
#line 3699 "test.tab.c"
    break;

  case 70: /* function_call: ID OPENPAR args CLOSEPAR  */
#line 1462 "test.y"
                                                                                                                                {
																				no* ancora = (no*)malloc(sizeof(no));
																				(*ancora).numFilhos = 1;
																				(*ancora).filhos[0] = (yyvsp[-1].node);
																				(*ancora).tipo = YYSYMBOL_function_call;
																				char ancora2[] = "function_call";
																				(*ancora).nome = strdup(ancora2);
																				(*ancora).valor = strdup((yyvsp[-3].text));
																				simbolo *ancoraSimb = ProcurarTabela((yyvsp[-3].text));
																				if(ancoraSimb != NULL){
																					(*ancora).refereTabela = ancoraSimb;
																				}
																				else{
																					//(*ancora).refereTabela = CriarSimbolo($1,FUNC_TABLE,NULL);
																					printf("ERRO SEMANTICO! ID %s USADO ANTES DE SER DECLARADO!\n",(yyvsp[-3].text));
																				}
																				(yyval.node) = ancora;
																				(yyvsp[-3].text) = NULL;
																				(yyvsp[-2].text) = NULL;
																				(yyvsp[0].text) = NULL;
																			}
#line 3725 "test.tab.c"
    break;

  case 71: /* args: ID args1  */
#line 1493 "test.y"
                                        {
							no* ancora = (no*)malloc(sizeof(no));
							(*ancora).numFilhos = 1;
							(*ancora).filhos[0] = (yyvsp[0].node);
							(*ancora).tipo = YYSYMBOL_args;
							char ancora2[] = "ID";
							(*ancora).nome = strdup(ancora2);
							simbolo *ancoraSimb = ProcurarTabela((yyvsp[-1].text));
							if(ancoraSimb != NULL){
								(*ancora).refereTabela = ancoraSimb;
							}
							else{
								//(*ancora).refereTabela = CriarSimbolo($1,FUNC_TABLE,NULL);
								printf("ERRO SEMANTICO! ID %s USADO ANTES DE SER DECLARADO!\n",(yyvsp[-1].text));

							}
							(*ancora).valor = strdup((yyvsp[-1].text));
							free((yyvsp[-1].text));
							(yyval.node) = ancora;
						}
#line 3750 "test.tab.c"
    break;

  case 72: /* args: NUM args1  */
#line 1513 "test.y"
                                        {	
							no* ancora = (no*)malloc(sizeof(no));
							(*ancora).numFilhos = 1;
							(*ancora).filhos[0] = (yyvsp[0].node);
							(*ancora).tipo = YYSYMBOL_args;
							char ancora2[] = "NUM";
							(*ancora).nome = strdup(ancora2);
							(*ancora).refereTabela = NULL;
							(*ancora).valor = strdup((yyvsp[-1].text));
							free((yyvsp[-1].text));
							(yyval.node) = ancora;
						}
#line 3767 "test.tab.c"
    break;

  case 73: /* args1: COMMA args  */
#line 1528 "test.y"
                                        {
							no* ancora = (no*)malloc(sizeof(no));
							(*ancora).numFilhos = 1;
							(*ancora).filhos[0] = (yyvsp[0].node);
							(*ancora).tipo = YYSYMBOL_args1;
							char ancora2[] = "comma";
							(*ancora).nome = strdup(ancora2);
							(*ancora).refereTabela = NULL;
							(*ancora).valor = NULL;
							(yyval.node) = ancora;
							(yyvsp[-1].text) = NULL;
						}
#line 3784 "test.tab.c"
    break;

  case 74: /* args1: %empty  */
#line 1540 "test.y"
                                        {
							no* ancora = (no*)malloc(sizeof(no));
							(*ancora).numFilhos = 0;
							char ancora2[] = "epsilon";
							(*ancora).nome = strdup(ancora2);
							(*ancora).tipo = YYSYMBOL_args1;
							(*ancora).refereTabela = NULL;
							(*ancora).valor = NULL;
							(yyval.node) = ancora;
						}
#line 3799 "test.tab.c"
    break;

  case 75: /* funcargs: type ID  */
#line 1553 "test.y"
                                                        {
									no* ancora = (no*)malloc(sizeof(no));
									(*ancora).numFilhos = 1;
									(*ancora).filhos[0] = (yyvsp[-1].node);
									(*ancora).tipo = YYSYMBOL_funcargs;
									char ancora2[] = "single";
									(*ancora).nome = strdup(ancora2);
									simbolo *ancoraSimb = ProcurarTabela((yyvsp[0].text));
									if(ancoraSimb != NULL){
										(*ancora).refereTabela = ancoraSimb;
									}
									else{
										//(*ancora).refereTabela = CriarSimbolo($2,0,NULL);
										printf("ERRO SEMANTICO! ID %s USADO ANTES DE SER DECLARADO!\n",(yyvsp[0].text));
									}
									(*ancora).valor = strdup((yyvsp[0].text));
									free((yyvsp[0].text));
									(yyval.node) = ancora;
								}
#line 3823 "test.tab.c"
    break;

  case 76: /* funcargs: type ID COMMA funcargs  */
#line 1572 "test.y"
                                        {
									no* ancora = (no*)malloc(sizeof(no));
									(*ancora).numFilhos = 2;
									(*ancora).filhos[0] = (yyvsp[-3].node);
									(*ancora).filhos[1] = (yyvsp[0].node);
									(*ancora).tipo = YYSYMBOL_funcargs;
									char ancora2[] = "comma";
									(*ancora).nome = strdup(ancora2);
									//printf("\n\nOI %s OI\n\n",$2);
									simbolo *ancoraSimb = ProcurarTabela((yyvsp[-2].text));
									if(ancoraSimb != NULL){
										(*ancora).refereTabela = ancoraSimb;
									}
									else{
										//(*ancora).refereTabela = CriarSimbolo($2,0,NULL);
										printf("ERRO SEMANTICO! ID %s USADO ANTES DE SER DECLARADO!\n",(yyvsp[-2].text));
									}
									(*ancora).valor = strdup((yyvsp[-2].text));
									free((yyvsp[-2].text));
									(yyvsp[-1].text) = NULL;
									(yyval.node) = ancora;
								}
#line 3850 "test.tab.c"
    break;

  case 77: /* funcargs: %empty  */
#line 1594 "test.y"
                                                        {
									no* ancora = (no*)malloc(sizeof(no));
									(*ancora).numFilhos = 0;
									char ancora2[] = "epsilon";
									(*ancora).nome = strdup(ancora2);
									(*ancora).tipo = YYSYMBOL_funcargs;
									(*ancora).refereTabela = NULL;
									(*ancora).valor = NULL;
									(yyval.node) = ancora;
								}
#line 3865 "test.tab.c"
    break;

  case 78: /* $@6: %empty  */
#line 1609 "test.y"
                                                                                                                                                        {
																				escopoCounter++;
																				Push(pilhaEscopo,CriarStack(escopoCounter));
																			}
#line 3874 "test.tab.c"
    break;

  case 79: /* function_declaration: type ID OPENPAR funcargs CLOSEPAR OPENCURLY $@6 statement CLOSECURLY  */
#line 1613 "test.y"
                                                                                                                                {
																				(yyvsp[-6].text) = NULL;
																				(yyvsp[-4].text) = NULL;
																				(yyvsp[-3].text) = NULL;
																				no* ancora = (no*)malloc(sizeof(no));
																				(*ancora).numFilhos = 3;
																				(*ancora).filhos[0] = (yyvsp[-8].node);
																				(*ancora).filhos[1] = (yyvsp[-5].node);
																				
																				(*ancora).tipo = YYSYMBOL_function_declaration;
																				char ancora2[] = "function_declaration";
																				(*ancora).nome = strdup(ancora2);
																				simbolo *ancoraSimb = ProcurarTabela((yyvsp[-7].text));
																				if(ancoraSimb != NULL){
																					(*ancora).refereTabela = ancoraSimb;
																					(*ancoraSimb).tipo = FUNC_TABLE;
																				}
																				else{
																					(*ancora).refereTabela = CriarSimbolo((yyvsp[-7].text),FUNC_TABLE,NULL,escopoCounter);
																				}
																				(*ancora).valor = strdup((yyvsp[-7].text));
																				free((yyvsp[-7].text));
																				(*ancora).filhos[2] = (yyvsp[-1].node);
																				(yyvsp[0].text) = NULL;
																				Pop(pilhaEscopo);
																				(yyval.node) = ancora;
																			}
#line 3906 "test.tab.c"
    break;

  case 80: /* assignment: ID ASSIGN mathop  */
#line 1644 "test.y"
                                                                                                                                        {
																				no* ancora = (no*)malloc(sizeof(no));
																				(*ancora).numFilhos = 1;
																				(*ancora).tipo = YYSYMBOL_assignment;
																				(*ancora).filhos[0] = (yyvsp[0].node);
																				char ancora2[] = "ID";
																				(*ancora).nome = strdup(ancora2);
																				simbolo *ancoraSimb = ProcurarTabela((yyvsp[-2].text));
																				if(ancoraSimb != NULL){
																					(*ancora).refereTabela = ancoraSimb;
																				}
																				else{
																					//(*ancora).refereTabela = CriarSimbolo($1,0,NULL);
																					printf("ERRO SEMANTICO! ID %s USADO ANTES DE SER DECLARADO!\n",(yyvsp[-2].text));
																				}
																				(*ancora).valor = strdup((yyvsp[-2].text));
																				free((yyvsp[-2].text));
																				(yyvsp[-1].text) = NULL;
																				(yyval.node) = ancora;
																			}
#line 3931 "test.tab.c"
    break;

  case 81: /* variable_declaration: type ID  */
#line 1667 "test.y"
                                                                                                                                        {
																			no* ancora = (no*)malloc(sizeof(no));
																			(*ancora).numFilhos = 1;
																			(*ancora).filhos[0] = (yyvsp[-1].node);
																			(*ancora).tipo = YYSYMBOL_variable_declaration;
																			char ancora2[] = "variable_declaration";
																			(*ancora).nome = strdup(ancora2);
																			simbolo *ancoraSimb = ProcurarTabela((yyvsp[0].text));
																			if(ancoraSimb != NULL){
																				(*ancora).refereTabela = ancoraSimb;
																				(*ancoraSimb).tipo = atoi(((no*)(yyvsp[-1].node))->valor);
																			}
																			else{
																				(*ancora).refereTabela = CriarSimbolo((yyvsp[0].text),atoi(((no*)(yyvsp[-1].node))->valor),NULL,escopoCounter);
																			}
																			(*ancora).valor = strdup((yyvsp[0].text));
																			free((yyvsp[0].text));
																			(yyval.node) = ancora;
																		}
#line 3955 "test.tab.c"
    break;

  case 82: /* mathop: mathop PLUS mathop1  */
#line 1693 "test.y"
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
										(yyval.node) = ancora;
										(yyvsp[-1].text) = NULL;
									}
#line 3973 "test.tab.c"
    break;

  case 83: /* mathop: mathop MINUS mathop1  */
#line 1706 "test.y"
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
										(yyval.node) = ancora;
										(yyvsp[-1].text) = NULL;
									}
#line 3991 "test.tab.c"
    break;

  case 84: /* mathop: mathop1  */
#line 1719 "test.y"
                                                                {
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 1;
										(*ancora).filhos[0] = (yyvsp[0].node);
										char ancora2[] = "mathop1";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_mathop;
										(*ancora).refereTabela = NULL;
										(*ancora).valor = NULL;
										(yyval.node) = ancora;
									}
#line 4007 "test.tab.c"
    break;

  case 85: /* mathop1: mathop1 AST mathop2  */
#line 1733 "test.y"
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
										(yyval.node) = ancora;
										(yyvsp[-1].text) = NULL;
									}
#line 4025 "test.tab.c"
    break;

  case 86: /* mathop1: mathop1 BS mathop2  */
#line 1746 "test.y"
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
										(yyval.node) = ancora;
										(yyvsp[-1].text) = NULL;
									}
#line 4043 "test.tab.c"
    break;

  case 87: /* mathop1: mathop2  */
#line 1759 "test.y"
                                                                {
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 1;
										(*ancora).filhos[0] = (yyvsp[0].node);
										char ancora2[] = "mathop2";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_mathop1;
										(*ancora).refereTabela = NULL;
										(*ancora).valor = NULL;
										(yyval.node) = ancora;
									}
#line 4059 "test.tab.c"
    break;

  case 88: /* mathop2: matharg  */
#line 1774 "test.y"
                                                                {
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 1;
										(*ancora).filhos[0] = (yyvsp[0].node);
										char ancora2[] = "matharg";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_mathop2;
										(*ancora).refereTabela = NULL;
										(*ancora).valor = NULL;
										(yyval.node) = ancora;
									}
#line 4075 "test.tab.c"
    break;

  case 89: /* mathop2: OPENPAR mathop CLOSEPAR  */
#line 1785 "test.y"
                                                {
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 1;
										(*ancora).filhos[0] = (yyvsp[-1].node);
										char ancora2[] = "pars";
										(*ancora).nome = strdup(ancora2);
										(*ancora).tipo = YYSYMBOL_mathop2;
										(*ancora).refereTabela = NULL;
										(*ancora).valor = NULL;
										(yyval.node) = ancora;
										(yyvsp[-2].text) = NULL;
										(yyvsp[0].text) = NULL;
									}
#line 4093 "test.tab.c"
    break;

  case 90: /* matharg: ID  */
#line 1801 "test.y"
                                                                        {
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 0;
										(*ancora).tipo = YYSYMBOL_matharg;
										char ancora2[] = "ID";
										(*ancora).nome = strdup(ancora2);
										simbolo *ancoraSimb = ProcurarTabela((yyvsp[0].text));
										if(ancoraSimb != NULL){
											(*ancora).refereTabela = ancoraSimb;
										}
										else{
											//(*ancora).refereTabela = CriarSimbolo($1,0,NULL);
											printf("ERRO SEMANTICO! ID %s USADO ANTES DE SER DECLARADO!\n",(yyvsp[0].text));
										}
										(*ancora).valor = strdup((yyvsp[0].text));
										free((yyvsp[0].text));
										(yyval.node) = ancora;																
									}
#line 4116 "test.tab.c"
    break;

  case 91: /* matharg: NUM  */
#line 1820 "test.y"
                                                                {
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 0;
										(*ancora).tipo = YYSYMBOL_matharg;
										char ancora2[] = "NUM";
										(*ancora).nome = strdup(ancora2);
										(*ancora).valor = strdup((yyvsp[0].text));
										(*ancora).refereTabela = NULL;
										free((yyvsp[0].text));
										(yyval.node) = ancora;																
									}
#line 4132 "test.tab.c"
    break;

  case 92: /* matharg: function_call  */
#line 1831 "test.y"
                                                        {
										no* ancora = (no*)malloc(sizeof(no));
										(*ancora).numFilhos = 1;
										(*ancora).filhos[0] = (yyvsp[0].node);
										(*ancora).tipo = YYSYMBOL_matharg;
										char ancora2[] = "function_call";
										(*ancora).nome = strdup(ancora2);
										(*ancora).valor = NULL;
										(*ancora).refereTabela = NULL;
										(yyval.node) = ancora;																
									}
#line 4148 "test.tab.c"
    break;

  case 93: /* type: SET  */
#line 1845 "test.y"
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
										(yyval.node) = ancora;
										(yyvsp[0].text) = NULL;
									}
#line 4166 "test.tab.c"
    break;

  case 94: /* type: INT  */
#line 1858 "test.y"
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
										(yyval.node) = ancora;
										(yyvsp[0].text) = NULL;
									}
#line 4184 "test.tab.c"
    break;

  case 95: /* type: ELEM  */
#line 1871 "test.y"
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
										(yyval.node) = ancora;
										(yyvsp[0].text) = NULL;
									}
#line 4202 "test.tab.c"
    break;

  case 96: /* type: FLOAT  */
#line 1884 "test.y"
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
										(yyval.node) = ancora;
										(yyvsp[0].text) = NULL;
									}
#line 4220 "test.tab.c"
    break;


#line 4224 "test.tab.c"

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

#line 1899 "test.y"

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
	EscreverArvore(raiz,1);
	//yylex_destroy();
	ApagarTabela();
	LimparStack(pilhaEscopo);
	return 0;
}

