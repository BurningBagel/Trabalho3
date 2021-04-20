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


#line 338 "test.tab.c"

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
  YYSYMBOL_64_4 = 64,                      /* $@4  */
  YYSYMBOL_else = 65,                      /* else  */
  YYSYMBOL_66_5 = 66,                      /* $@5  */
  YYSYMBOL_conjuntoop = 67,                /* conjuntoop  */
  YYSYMBOL_conjuntoop1 = 68,               /* conjuntoop1  */
  YYSYMBOL_pertinencia = 69,               /* pertinencia  */
  YYSYMBOL_tipagem = 70,                   /* tipagem  */
  YYSYMBOL_inclusao = 71,                  /* inclusao  */
  YYSYMBOL_remocao = 72,                   /* remocao  */
  YYSYMBOL_selecao = 73,                   /* selecao  */
  YYSYMBOL_iteracao = 74,                  /* iteracao  */
  YYSYMBOL_75_6 = 75,                      /* $@6  */
  YYSYMBOL_76_7 = 76,                      /* $@7  */
  YYSYMBOL_function_call = 77,             /* function_call  */
  YYSYMBOL_args = 78,                      /* args  */
  YYSYMBOL_args1 = 79,                     /* args1  */
  YYSYMBOL_funcargs = 80,                  /* funcargs  */
  YYSYMBOL_function_declaration = 81,      /* function_declaration  */
  YYSYMBOL_82_8 = 82,                      /* $@8  */
  YYSYMBOL_assignment = 83,                /* assignment  */
  YYSYMBOL_variable_declaration = 84,      /* variable_declaration  */
  YYSYMBOL_mathop = 85,                    /* mathop  */
  YYSYMBOL_mathop1 = 86,                   /* mathop1  */
  YYSYMBOL_mathop2 = 87,                   /* mathop2  */
  YYSYMBOL_matharg = 88,                   /* matharg  */
  YYSYMBOL_type = 89                       /* type  */
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
#define YYFINAL  70
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   395

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  41
/* YYNRULES -- Number of rules.  */
#define YYNRULES  107
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  226

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
       0,   383,   383,   394,   409,   408,   431,   461,   475,   489,
     503,   610,   625,   639,   653,   667,   681,   707,   721,   738,
     760,   773,   784,   798,   810,   823,   836,   849,   862,   875,
     888,   901,   917,   943,   958,   972,   991,  1006,  1029,  1042,
    1055,  1070,  1070,  1104,  1103,  1128,  1145,  1144,  1169,  1188,
    1200,  1212,  1212,  1231,  1244,  1255,  1266,  1277,  1288,  1302,
    1313,  1324,  1335,  1346,  1357,  1378,  1391,  1407,  1424,  1441,
    1459,  1477,  1476,  1500,  1516,  1515,  1539,  1557,  1588,  1608,
    1620,  1631,  1645,  1657,  1670,  1689,  1711,  1721,  1737,  1736,
    1772,  1795,  1814,  1833,  1846,  1859,  1873,  1886,  1899,  1914,
    1925,  1941,  1960,  1971,  1985,  1998,  2011,  2024
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
  "writeln", "return", "for", "$@2", "if", "$@3", "$@4", "else", "$@5",
  "conjuntoop", "conjuntoop1", "pertinencia", "tipagem", "inclusao",
  "remocao", "selecao", "iteracao", "$@6", "$@7", "function_call", "args",
  "args1", "funcargs", "function_declaration", "$@8", "assignment",
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

#define YYPACT_NINF (-173)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-104)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     183,   -17,  -173,   -12,  -173,  -173,  -173,  -173,    25,    37,
      63,    99,   101,   111,   117,   120,   123,   135,    13,   100,
    -173,   172,  -173,   142,   136,   138,   139,   144,   142,   142,
     152,  -173,  -173,  -173,  -173,   161,   142,  -173,   142,   168,
     169,    -9,    58,  -173,  -173,   121,  -173,   174,   100,     8,
     213,   218,    38,   130,   349,   265,   265,   265,   241,   274,
     335,    13,   105,   350,  -173,   298,    16,   208,   103,   207,
    -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,  -173,   349,
    -173,  -173,  -173,   142,   349,   100,   100,   100,   100,  -173,
     212,   196,   196,    78,   196,   214,    16,   216,  -173,   208,
      33,   221,  -173,   202,   220,   226,   231,   242,   255,   244,
     279,    64,   245,  -173,  -173,  -173,  -173,   161,   250,   161,
     262,   268,   269,   270,   293,  -173,   105,   105,   105,   105,
     105,   105,   105,   105,  -173,   239,  -173,  -173,  -173,    58,
      58,  -173,  -173,    84,   174,  -173,  -173,  -173,  -173,  -173,
     258,   282,    33,  -173,  -173,  -173,  -173,  -173,  -173,  -173,
    -173,  -173,  -173,   306,   330,  -173,  -173,  -173,  -173,  -173,
    -173,  -173,  -173,  -173,  -173,  -173,   294,   284,  -173,  -173,
     316,  -173,   316,   296,  -173,  -173,  -173,  -173,   314,   300,
     207,   234,  -173,   207,  -173,   213,   207,   207,  -173,    84,
     320,  -173,  -173,  -173,   331,   318,   334,   336,   207,  -173,
     316,   207,   316,  -173,  -173,  -173,   337,  -173,   338,  -173,
     340,  -173,  -173,   207,   352,  -173
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,   102,   101,   105,   107,   106,   104,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    40,     0,
       4,     0,     2,     0,     0,     0,     0,     0,     0,     0,
       0,    54,    55,    56,    57,    58,     0,   103,     0,     0,
       0,     0,    95,    98,    99,     0,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
      19,     0,     0,     0,    38,    22,    39,   101,     0,     0,
       1,     3,    16,    14,    15,    12,     7,     8,    17,     0,
       9,     6,    13,     0,     0,     0,     0,     0,     0,    92,
      91,    83,    83,    83,    83,     0,    90,     0,    21,    19,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,    59,    60,    61,    62,    63,     0,     0,
       0,     0,     0,     0,     0,    23,     0,     0,     0,     0,
       0,     0,     0,     0,   100,     0,    66,    10,    65,    93,
      94,    96,    97,     0,     0,    81,    79,    78,    80,    77,
       0,     0,     0,    32,    34,    35,    33,    37,    36,    67,
      68,    69,    70,     0,     0,    20,    28,    29,    30,    31,
      26,    27,    25,    24,     5,    87,     0,     0,    82,    46,
      53,    43,    53,     0,    74,    76,    71,    73,     0,    84,
       0,     0,    48,     0,    45,     0,     0,     0,    88,     0,
       0,    51,    50,    49,     0,     0,     0,     0,     0,    85,
      53,     0,    53,    41,    75,    72,     0,    47,     0,    44,
       0,    89,    52,     0,     0,    42
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -173,  -173,   -23,  -173,  -133,   -60,   -15,  -173,  -173,  -173,
    -173,  -173,  -173,   166,  -173,  -173,  -172,  -173,  -173,    92,
      20,   -34,    11,    42,    26,  -173,  -173,  -173,   -14,   232,
     -69,   163,  -173,  -173,   -49,  -173,     3,    15,    44,  -173,
    -136
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    21,    22,    69,    23,    63,   124,    24,    25,    26,
      27,    28,   220,    29,   193,   190,   192,   211,    30,   112,
      31,    32,    33,    34,    35,    36,   197,   196,    37,    95,
     145,   176,    38,   208,    39,    40,    41,    42,    43,    44,
      45
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      71,   104,   125,    64,    65,    76,    77,   177,    84,    97,
     194,    98,    47,    80,    99,    81,    59,   180,   182,    60,
     114,    66,    68,   146,   147,   148,    46,    85,    86,    48,
     185,   187,   100,    94,   101,   102,    98,    61,   217,    99,
     219,     2,   106,   107,    67,   114,   135,    65,   102,    49,
     114,    96,    85,    86,    62,   108,   110,   100,   202,    62,
     137,    50,    19,   177,    68,   115,   166,   167,   168,   169,
     170,   171,   172,   173,   113,   118,   120,   121,   123,    62,
     117,   119,   119,   119,   119,   175,   102,    51,    47,   -64,
     115,     4,     5,     6,     7,   115,   116,    87,    88,   113,
     139,   140,    47,     2,   113,   117,    67,   -64,    98,   -86,
     117,    99,   102,   102,   102,   102,   102,   102,   102,   102,
     144,   116,    89,    52,    19,    53,   116,    90,   134,   100,
      94,   141,   142,     2,   109,    54,    67,   183,   102,    85,
      86,    55,   -11,     1,    56,     2,   205,    57,     3,     4,
       5,     6,     7,     8,    19,     9,    10,    11,    12,    58,
      13,    14,    15,    16,    17,    18,    19,   200,    20,   -11,
     204,   136,    70,   206,   207,    91,   138,    92,    79,    72,
      93,    73,    74,   -11,     1,   216,     2,    75,   218,     3,
       4,     5,     6,     7,     8,    78,     9,    10,    11,    12,
     224,    13,    14,    15,    16,    17,    18,    19,     1,    20,
       2,    82,    83,     3,     4,     5,     6,     7,     8,   103,
       9,    10,    11,    12,   105,    13,    14,    15,    16,    17,
      18,    19,    47,    20,   -11,     1,   143,     2,   144,   149,
       3,   150,   122,    48,     2,     8,   151,    67,    10,    11,
      12,   153,    13,    14,    15,    16,   154,    18,    19,     1,
     201,     2,    16,   152,     3,    19,   174,   155,     2,   157,
     159,    67,    10,    11,    12,   160,    13,    14,    15,    16,
     156,    18,    19,     1,   179,     2,    16,   161,     3,    19,
     189,    85,    86,   162,   163,   164,    10,    11,    12,  -102,
      13,    14,    15,    16,   158,    18,    19,     1,   181,     2,
    -102,  -102,     3,  -102,  -102,    85,    86,  -102,   165,   188,
      10,    11,    12,  -103,    13,    14,    15,    16,   191,    18,
      19,     1,   184,     2,  -103,  -103,     3,  -103,  -103,   195,
     198,  -103,   199,   213,    10,    11,    12,   210,    13,    14,
      15,    16,     2,    18,    19,   111,   186,   203,   212,    47,
    -101,   214,   209,   215,   221,   222,   223,    13,    14,    15,
      16,  -101,  -101,    19,  -101,  -101,   178,     0,  -101,   225,
     126,   127,   128,   129,   130,   131,     0,     0,     0,     0,
       0,     0,     0,     0,   132,   133
};

static const yytype_int16 yycheck[] =
{
      23,    50,    62,    18,    18,    28,    29,   143,    17,     1,
     182,     3,    24,    36,     6,    38,     3,   150,   151,     6,
      54,    18,    19,    92,    93,    94,    43,    36,    37,    41,
     163,   164,    24,    47,    49,    49,     3,    24,   210,     6,
     212,     3,     4,     5,     6,    79,    69,    61,    62,    24,
      84,    48,    36,    37,    46,    52,    53,    24,   191,    46,
      83,    24,    24,   199,    61,    54,   126,   127,   128,   129,
     130,   131,   132,   133,    54,    55,    56,    57,    58,    46,
      54,    55,    56,    57,    58,     1,   100,    24,    24,    25,
      79,     7,     8,     9,    10,    84,    54,    39,    40,    79,
      85,    86,    24,     3,    84,    79,     6,    43,     3,    25,
      84,     6,   126,   127,   128,   129,   130,   131,   132,   133,
      42,    79,     1,    24,    24,    24,    84,     6,    25,    24,
     144,    87,    88,     3,     4,    24,     6,   152,   152,    36,
      37,    24,     0,     1,    24,     3,   195,    24,     6,     7,
       8,     9,    10,    11,    24,    13,    14,    15,    16,    24,
      18,    19,    20,    21,    22,    23,    24,   190,    26,    27,
     193,    79,     0,   196,   197,     1,    84,     3,    17,    43,
       6,    43,    43,     0,     1,   208,     3,    43,   211,     6,
       7,     8,     9,    10,    11,    43,    13,    14,    15,    16,
     223,    18,    19,    20,    21,    22,    23,    24,     1,    26,
       3,    43,    43,     6,     7,     8,     9,    10,    11,     6,
      13,    14,    15,    16,     6,    18,    19,    20,    21,    22,
      23,    24,    24,    26,    27,     1,    24,     3,    42,    25,
       6,    25,     1,    41,     3,    11,    25,     6,    14,    15,
      16,    25,    18,    19,    20,    21,    25,    23,    24,     1,
      26,     3,    21,    43,     6,    24,    27,    25,     3,    25,
      25,     6,    14,    15,    16,    25,    18,    19,    20,    21,
      25,    23,    24,     1,    26,     3,    21,    25,     6,    24,
       6,    36,    37,    25,    25,    25,    14,    15,    16,    25,
      18,    19,    20,    21,    25,    23,    24,     1,    26,     3,
      36,    37,     6,    39,    40,    36,    37,    43,    25,    25,
      14,    15,    16,    25,    18,    19,    20,    21,    12,    23,
      24,     1,    26,     3,    36,    37,     6,    39,    40,    43,
      26,    43,    42,    25,    14,    15,    16,    27,    18,    19,
      20,    21,     3,    23,    24,     6,    26,   191,    27,    24,
      25,    27,   199,    27,    27,    27,    26,    18,    19,    20,
      21,    36,    37,    24,    39,    40,   144,    -1,    43,    27,
      30,    31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    44,    45
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     1,     3,     6,     7,     8,     9,    10,    11,    13,
      14,    15,    16,    18,    19,    20,    21,    22,    23,    24,
      26,    50,    51,    53,    56,    57,    58,    59,    60,    62,
      67,    69,    70,    71,    72,    73,    74,    77,    81,    83,
      84,    85,    86,    87,    88,    89,    43,    24,    41,    24,
      24,    24,    24,    24,    24,    24,    24,    24,    24,     3,
       6,    24,    46,    54,    55,    77,    85,     6,    85,    52,
       0,    51,    43,    43,    43,    43,    51,    51,    43,    17,
      51,    51,    43,    43,    17,    36,    37,    39,    40,     1,
       6,     1,     3,     6,    77,    78,    85,     1,     3,     6,
      24,    55,    77,     6,    83,     6,     4,     5,    85,     4,
      85,     6,    68,    69,    70,    71,    72,    73,    69,    73,
      69,    69,     1,    69,    55,    54,    30,    31,    32,    33,
      34,    35,    44,    45,    25,    51,    68,    51,    68,    86,
      86,    87,    87,    24,    42,    79,    79,    79,    79,    25,
      25,    25,    43,    25,    25,    25,    25,    25,    25,    25,
      25,    25,    25,    25,    25,    25,    54,    54,    54,    54,
      54,    54,    54,    54,    27,     1,    80,    89,    78,    26,
      53,    26,    53,    55,    26,    53,    26,    53,    25,     6,
      64,    12,    65,    63,    65,    43,    76,    75,    26,    42,
      51,    26,    53,    62,    51,    83,    51,    51,    82,    80,
      27,    66,    27,    25,    27,    27,    51,    65,    51,    65,
      61,    27,    27,    26,    51,    27
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    49,    50,    51,    52,    51,    51,    51,    51,    51,
      51,    51,    53,    53,    53,    53,    53,    53,    53,    54,
      54,    54,    54,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    56,    57,    57,    57,    58,    58,    59,    59,
      59,    61,    60,    63,    62,    62,    64,    62,    62,    65,
      65,    66,    65,    65,    67,    67,    67,    67,    67,    68,
      68,    68,    68,    68,    68,    69,    69,    70,    71,    72,
      73,    75,    74,    74,    76,    74,    74,    77,    78,    78,
      78,    78,    79,    79,    80,    80,    80,    80,    82,    81,
      83,    84,    84,    85,    85,    85,    86,    86,    86,    87,
      87,    88,    88,    88,    89,    89,    89,    89
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     4,     2,     2,     2,     2,
       3,     0,     2,     2,     2,     2,     2,     2,     2,     1,
       3,     1,     1,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     4,     4,     4,     4,     4,     4,     2,     2,
       1,     0,    12,     0,     9,     6,     0,     9,     6,     2,
       2,     0,     5,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     4,     4,     4,
       4,     0,     8,     5,     0,     8,     5,     4,     2,     2,
       2,     2,     2,     0,     2,     4,     0,     1,     0,     9,
       3,     2,     2,     3,     3,     1,     3,     3,     1,     1,
       3,     1,     1,     1,     1,     1,     1,     1
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
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1791 "test.tab.c"
        break;

    case YYSYMBOL_STRING: /* STRING  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1797 "test.tab.c"
        break;

    case YYSYMBOL_CHAR: /* CHAR  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1803 "test.tab.c"
        break;

    case YYSYMBOL_ID: /* ID  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1809 "test.tab.c"
        break;

    case YYSYMBOL_INT: /* INT  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1815 "test.tab.c"
        break;

    case YYSYMBOL_FLOAT: /* FLOAT  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1821 "test.tab.c"
        break;

    case YYSYMBOL_ELEM: /* ELEM  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1827 "test.tab.c"
        break;

    case YYSYMBOL_SET: /* SET  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1833 "test.tab.c"
        break;

    case YYSYMBOL_IF: /* IF  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1839 "test.tab.c"
        break;

    case YYSYMBOL_ELSE: /* ELSE  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1845 "test.tab.c"
        break;

    case YYSYMBOL_FOR: /* FOR  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1851 "test.tab.c"
        break;

    case YYSYMBOL_READ: /* READ  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1857 "test.tab.c"
        break;

    case YYSYMBOL_WRITE: /* WRITE  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1863 "test.tab.c"
        break;

    case YYSYMBOL_WRITELN: /* WRITELN  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1869 "test.tab.c"
        break;

    case YYSYMBOL_IN: /* IN  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1875 "test.tab.c"
        break;

    case YYSYMBOL_IS_SET: /* IS_SET  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1881 "test.tab.c"
        break;

    case YYSYMBOL_ADD: /* ADD  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1887 "test.tab.c"
        break;

    case YYSYMBOL_REMOVE: /* REMOVE  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1893 "test.tab.c"
        break;

    case YYSYMBOL_EXISTS: /* EXISTS  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1899 "test.tab.c"
        break;

    case YYSYMBOL_FORALL: /* FORALL  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1905 "test.tab.c"
        break;

    case YYSYMBOL_RETURN: /* RETURN  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1911 "test.tab.c"
        break;

    case YYSYMBOL_OPENPAR: /* OPENPAR  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1917 "test.tab.c"
        break;

    case YYSYMBOL_CLOSEPAR: /* CLOSEPAR  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1923 "test.tab.c"
        break;

    case YYSYMBOL_OPENCURLY: /* OPENCURLY  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1929 "test.tab.c"
        break;

    case YYSYMBOL_CLOSECURLY: /* CLOSECURLY  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1935 "test.tab.c"
        break;

    case YYSYMBOL_OPENBRAC: /* OPENBRAC  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1941 "test.tab.c"
        break;

    case YYSYMBOL_CLOSEBRAC: /* CLOSEBRAC  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1947 "test.tab.c"
        break;

    case YYSYMBOL_LESS: /* LESS  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1953 "test.tab.c"
        break;

    case YYSYMBOL_LE: /* LE  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1959 "test.tab.c"
        break;

    case YYSYMBOL_EQ: /* EQ  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1965 "test.tab.c"
        break;

    case YYSYMBOL_NEQ: /* NEQ  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1971 "test.tab.c"
        break;

    case YYSYMBOL_GREATER: /* GREATER  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1977 "test.tab.c"
        break;

    case YYSYMBOL_GE: /* GE  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1983 "test.tab.c"
        break;

    case YYSYMBOL_PLUS: /* PLUS  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1989 "test.tab.c"
        break;

    case YYSYMBOL_MINUS: /* MINUS  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 1995 "test.tab.c"
        break;

    case YYSYMBOL_DBS: /* DBS  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2001 "test.tab.c"
        break;

    case YYSYMBOL_AST: /* AST  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2007 "test.tab.c"
        break;

    case YYSYMBOL_BS: /* BS  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2013 "test.tab.c"
        break;

    case YYSYMBOL_ASSIGN: /* ASSIGN  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2019 "test.tab.c"
        break;

    case YYSYMBOL_COMMA: /* COMMA  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2025 "test.tab.c"
        break;

    case YYSYMBOL_SEMICOLON: /* SEMICOLON  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2031 "test.tab.c"
        break;

    case YYSYMBOL_OR: /* OR  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2037 "test.tab.c"
        break;

    case YYSYMBOL_AND: /* AND  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2043 "test.tab.c"
        break;

    case YYSYMBOL_NOT: /* NOT  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2049 "test.tab.c"
        break;

    case YYSYMBOL_AMP: /* AMP  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2055 "test.tab.c"
        break;

    case YYSYMBOL_PCENT: /* PCENT  */
#line 360 "test.y"
            {((*yyvaluep).text) = NULL;}
#line 2061 "test.tab.c"
        break;

    case YYSYMBOL_statement: /* statement  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2067 "test.tab.c"
        break;

    case YYSYMBOL_single_line_statement: /* single_line_statement  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2073 "test.tab.c"
        break;

    case YYSYMBOL_comparg: /* comparg  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2079 "test.tab.c"
        break;

    case YYSYMBOL_comparison: /* comparison  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2085 "test.tab.c"
        break;

    case YYSYMBOL_read: /* read  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2091 "test.tab.c"
        break;

    case YYSYMBOL_write: /* write  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2097 "test.tab.c"
        break;

    case YYSYMBOL_writeln: /* writeln  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2103 "test.tab.c"
        break;

    case YYSYMBOL_return: /* return  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2109 "test.tab.c"
        break;

    case YYSYMBOL_for: /* for  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2115 "test.tab.c"
        break;

    case YYSYMBOL_if: /* if  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2121 "test.tab.c"
        break;

    case YYSYMBOL_else: /* else  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2127 "test.tab.c"
        break;

    case YYSYMBOL_conjuntoop: /* conjuntoop  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2133 "test.tab.c"
        break;

    case YYSYMBOL_conjuntoop1: /* conjuntoop1  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2139 "test.tab.c"
        break;

    case YYSYMBOL_pertinencia: /* pertinencia  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2145 "test.tab.c"
        break;

    case YYSYMBOL_tipagem: /* tipagem  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2151 "test.tab.c"
        break;

    case YYSYMBOL_inclusao: /* inclusao  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2157 "test.tab.c"
        break;

    case YYSYMBOL_remocao: /* remocao  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2163 "test.tab.c"
        break;

    case YYSYMBOL_selecao: /* selecao  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2169 "test.tab.c"
        break;

    case YYSYMBOL_iteracao: /* iteracao  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2175 "test.tab.c"
        break;

    case YYSYMBOL_function_call: /* function_call  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2181 "test.tab.c"
        break;

    case YYSYMBOL_args: /* args  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2187 "test.tab.c"
        break;

    case YYSYMBOL_args1: /* args1  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2193 "test.tab.c"
        break;

    case YYSYMBOL_funcargs: /* funcargs  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2199 "test.tab.c"
        break;

    case YYSYMBOL_function_declaration: /* function_declaration  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2205 "test.tab.c"
        break;

    case YYSYMBOL_assignment: /* assignment  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2211 "test.tab.c"
        break;

    case YYSYMBOL_variable_declaration: /* variable_declaration  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2217 "test.tab.c"
        break;

    case YYSYMBOL_mathop: /* mathop  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2223 "test.tab.c"
        break;

    case YYSYMBOL_mathop1: /* mathop1  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2229 "test.tab.c"
        break;

    case YYSYMBOL_mathop2: /* mathop2  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2235 "test.tab.c"
        break;

    case YYSYMBOL_matharg: /* matharg  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2241 "test.tab.c"
        break;

    case YYSYMBOL_type: /* type  */
#line 360 "test.y"
            {((*yyvaluep).node) = NULL;}
#line 2247 "test.tab.c"
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
#line 383 "test.y"
                                                                                        {
													raiz = (yyvsp[0].node);
													//(*raiz).escopo = 1;
												}
#line 2534 "test.tab.c"
    break;

  case 3: /* statement: single_line_statement statement  */
#line 394 "test.y"
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
#line 2553 "test.tab.c"
    break;

  case 4: /* $@1: %empty  */
#line 409 "test.y"
                                                                                                {
													escopoCounter++;
													Push(pilhaEscopo,CriarStack(escopoCounter));
													(yyvsp[0].text) = NULL;
												}
#line 2563 "test.tab.c"
    break;

  case 5: /* statement: OPENCURLY $@1 statement CLOSECURLY  */
#line 414 "test.y"
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
#line 2584 "test.tab.c"
    break;

  case 6: /* statement: function_declaration statement  */
#line 431 "test.y"
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
#line 2603 "test.tab.c"
    break;

  case 7: /* statement: for statement  */
#line 461 "test.y"
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
#line 2622 "test.tab.c"
    break;

  case 8: /* statement: if statement  */
#line 475 "test.y"
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
#line 2641 "test.tab.c"
    break;

  case 9: /* statement: iteracao statement  */
#line 489 "test.y"
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
#line 2660 "test.tab.c"
    break;

  case 10: /* statement: variable_declaration SEMICOLON statement  */
#line 503 "test.y"
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
#line 2680 "test.tab.c"
    break;

  case 11: /* statement: %empty  */
#line 610 "test.y"
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
#line 2695 "test.tab.c"
    break;

  case 12: /* single_line_statement: return SEMICOLON  */
#line 625 "test.y"
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
#line 2713 "test.tab.c"
    break;

  case 13: /* single_line_statement: assignment SEMICOLON  */
#line 639 "test.y"
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
#line 2731 "test.tab.c"
    break;

  case 14: /* single_line_statement: write SEMICOLON  */
#line 653 "test.y"
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
#line 2749 "test.tab.c"
    break;

  case 15: /* single_line_statement: writeln SEMICOLON  */
#line 667 "test.y"
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
#line 2767 "test.tab.c"
    break;

  case 16: /* single_line_statement: read SEMICOLON  */
#line 681 "test.y"
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
#line 2785 "test.tab.c"
    break;

  case 17: /* single_line_statement: conjuntoop SEMICOLON  */
#line 707 "test.y"
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
#line 2803 "test.tab.c"
    break;

  case 18: /* single_line_statement: error SEMICOLON  */
#line 721 "test.y"
                                                                                {
													no* ancora = (no*)malloc(sizeof(no));
													(*ancora).numFilhos = 0;
													char ancora2[] = "ERROR";
													(*ancora).nome = strdup(ancora2);
													(*ancora).tipo = YYSYMBOL_single_line_statement;
													(*ancora).refereTabela = NULL;
													(*ancora).valor = NULL;
													(yyval.node) = ancora;
													(yyvsp[0].text) = NULL;
													yyerrok;
												}
#line 2820 "test.tab.c"
    break;

  case 19: /* comparg: ID  */
#line 738 "test.y"
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
#line 2847 "test.tab.c"
    break;

  case 20: /* comparg: OPENPAR comparison CLOSEPAR  */
#line 760 "test.y"
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
#line 2865 "test.tab.c"
    break;

  case 21: /* comparg: NUM  */
#line 773 "test.y"
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
#line 2881 "test.tab.c"
    break;

  case 22: /* comparg: function_call  */
#line 784 "test.y"
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
#line 2897 "test.tab.c"
    break;

  case 23: /* comparison: NOT comparg  */
#line 798 "test.y"
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
#line 2914 "test.tab.c"
    break;

  case 24: /* comparison: comparg AND comparg  */
#line 810 "test.y"
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
#line 2932 "test.tab.c"
    break;

  case 25: /* comparison: comparg OR comparg  */
#line 823 "test.y"
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
#line 2950 "test.tab.c"
    break;

  case 26: /* comparison: comparg GREATER comparg  */
#line 836 "test.y"
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
#line 2968 "test.tab.c"
    break;

  case 27: /* comparison: comparg GE comparg  */
#line 849 "test.y"
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
#line 2986 "test.tab.c"
    break;

  case 28: /* comparison: comparg LESS comparg  */
#line 862 "test.y"
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
#line 3004 "test.tab.c"
    break;

  case 29: /* comparison: comparg LE comparg  */
#line 875 "test.y"
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
#line 3022 "test.tab.c"
    break;

  case 30: /* comparison: comparg EQ comparg  */
#line 888 "test.y"
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
#line 3040 "test.tab.c"
    break;

  case 31: /* comparison: comparg NEQ comparg  */
#line 901 "test.y"
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
#line 3058 "test.tab.c"
    break;

  case 32: /* read: READ OPENPAR ID CLOSEPAR  */
#line 917 "test.y"
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
#line 3085 "test.tab.c"
    break;

  case 33: /* write: WRITE OPENPAR mathop CLOSEPAR  */
#line 943 "test.y"
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
#line 3104 "test.tab.c"
    break;

  case 34: /* write: WRITE OPENPAR STRING CLOSEPAR  */
#line 958 "test.y"
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
#line 3123 "test.tab.c"
    break;

  case 35: /* write: WRITE OPENPAR CHAR CLOSEPAR  */
#line 972 "test.y"
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
#line 3142 "test.tab.c"
    break;

  case 36: /* writeln: WRITELN OPENPAR mathop CLOSEPAR  */
#line 991 "test.y"
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
#line 3161 "test.tab.c"
    break;

  case 37: /* writeln: WRITELN OPENPAR STRING CLOSEPAR  */
#line 1006 "test.y"
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
#line 3180 "test.tab.c"
    break;

  case 38: /* return: RETURN comparison  */
#line 1029 "test.y"
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
#line 3198 "test.tab.c"
    break;

  case 39: /* return: RETURN mathop  */
#line 1042 "test.y"
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
#line 3216 "test.tab.c"
    break;

  case 40: /* return: RETURN  */
#line 1055 "test.y"
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
#line 3233 "test.tab.c"
    break;

  case 41: /* $@2: %empty  */
#line 1070 "test.y"
                                                                                                                                                        {
																													escopoCounter++;
																													Push(pilhaEscopo,CriarStack(escopoCounter));
																													(yyvsp[-7].text) = NULL;
																													(yyvsp[-6].text) = NULL;
																													(yyvsp[-4].text) = NULL;
																													(yyvsp[-2].text) = NULL;
																													(yyvsp[0].text) = NULL;
																													
																													
																												}
#line 3249 "test.tab.c"
    break;

  case 42: /* for: FOR OPENPAR assignment SEMICOLON comparison SEMICOLON assignment CLOSEPAR $@2 OPENCURLY statement CLOSECURLY  */
#line 1081 "test.y"
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
#line 3272 "test.tab.c"
    break;

  case 43: /* $@3: %empty  */
#line 1104 "test.y"
                                                                                                                                                                                                                        {
																												escopoCounter++;
																												Push(pilhaEscopo,CriarStack(escopoCounter));
																												
																											}
#line 3282 "test.tab.c"
    break;

  case 44: /* if: IF OPENPAR comparison CLOSEPAR OPENCURLY $@3 statement CLOSECURLY else  */
#line 1109 "test.y"
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
#line 3306 "test.tab.c"
    break;

  case 45: /* if: IF OPENPAR comparison CLOSEPAR single_line_statement else  */
#line 1128 "test.y"
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
#line 3327 "test.tab.c"
    break;

  case 46: /* $@4: %empty  */
#line 1145 "test.y"
                                                                                                                                                                                                                        {
																												escopoCounter++;
																												Push(pilhaEscopo,CriarStack(escopoCounter));
																												
																											}
#line 3337 "test.tab.c"
    break;

  case 47: /* if: IF OPENPAR error CLOSEPAR OPENCURLY $@4 statement CLOSECURLY else  */
#line 1150 "test.y"
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
																												(yyvsp[-1].text) = NULL;
																												Pop(pilhaEscopo);
																												(yyval.node) = ancora;
																												yyerrok;
																											}
#line 3361 "test.tab.c"
    break;

  case 48: /* if: IF OPENPAR error CLOSEPAR single_line_statement else  */
#line 1169 "test.y"
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
																												(yyval.node) = ancora;
																												(yyvsp[-5].text) = NULL;
																												(yyvsp[-4].text) = NULL;
																												(yyvsp[-2].text) = NULL;
																												yyerrok;
																											}
#line 3382 "test.tab.c"
    break;

  case 49: /* else: ELSE if  */
#line 1188 "test.y"
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
#line 3399 "test.tab.c"
    break;

  case 50: /* else: ELSE single_line_statement  */
#line 1200 "test.y"
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
#line 3416 "test.tab.c"
    break;

  case 51: /* $@5: %empty  */
#line 1212 "test.y"
                                                                                        {
														escopoCounter++;
														Push(pilhaEscopo,CriarStack(escopoCounter));
														(yyvsp[-1].text) = NULL;
														(yyvsp[0].text) = NULL;
													}
#line 3427 "test.tab.c"
    break;

  case 52: /* else: ELSE OPENCURLY $@5 statement CLOSECURLY  */
#line 1218 "test.y"
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
#line 3445 "test.tab.c"
    break;

  case 53: /* else: %empty  */
#line 1231 "test.y"
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
#line 3460 "test.tab.c"
    break;

  case 54: /* conjuntoop: pertinencia  */
#line 1244 "test.y"
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
#line 3476 "test.tab.c"
    break;

  case 55: /* conjuntoop: tipagem  */
#line 1255 "test.y"
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
#line 3492 "test.tab.c"
    break;

  case 56: /* conjuntoop: inclusao  */
#line 1266 "test.y"
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
#line 3508 "test.tab.c"
    break;

  case 57: /* conjuntoop: remocao  */
#line 1277 "test.y"
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
#line 3524 "test.tab.c"
    break;

  case 58: /* conjuntoop: selecao  */
#line 1288 "test.y"
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
#line 3540 "test.tab.c"
    break;

  case 59: /* conjuntoop1: pertinencia  */
#line 1302 "test.y"
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
#line 3556 "test.tab.c"
    break;

  case 60: /* conjuntoop1: tipagem  */
#line 1313 "test.y"
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
#line 3572 "test.tab.c"
    break;

  case 61: /* conjuntoop1: inclusao  */
#line 1324 "test.y"
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
#line 3588 "test.tab.c"
    break;

  case 62: /* conjuntoop1: remocao  */
#line 1335 "test.y"
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
#line 3604 "test.tab.c"
    break;

  case 63: /* conjuntoop1: selecao  */
#line 1346 "test.y"
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
#line 3620 "test.tab.c"
    break;

  case 64: /* conjuntoop1: ID  */
#line 1357 "test.y"
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
#line 3643 "test.tab.c"
    break;

  case 65: /* pertinencia: mathop IN conjuntoop1  */
#line 1378 "test.y"
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
#line 3661 "test.tab.c"
    break;

  case 66: /* pertinencia: selecao IN conjuntoop1  */
#line 1391 "test.y"
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
#line 3679 "test.tab.c"
    break;

  case 67: /* tipagem: IS_SET OPENPAR conjuntoop1 CLOSEPAR  */
#line 1407 "test.y"
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
#line 3698 "test.tab.c"
    break;

  case 68: /* inclusao: ADD OPENPAR pertinencia CLOSEPAR  */
#line 1424 "test.y"
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
#line 3717 "test.tab.c"
    break;

  case 69: /* remocao: REMOVE OPENPAR pertinencia CLOSEPAR  */
#line 1441 "test.y"
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
#line 3736 "test.tab.c"
    break;

  case 70: /* selecao: EXISTS OPENPAR pertinencia CLOSEPAR  */
#line 1459 "test.y"
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
#line 3755 "test.tab.c"
    break;

  case 71: /* $@6: %empty  */
#line 1477 "test.y"
                                                                                                                                                        {
																				escopoCounter++;
																				Push(pilhaEscopo,CriarStack(escopoCounter));
																				
																			}
#line 3765 "test.tab.c"
    break;

  case 72: /* iteracao: FORALL OPENPAR pertinencia CLOSEPAR OPENCURLY $@6 statement CLOSECURLY  */
#line 1482 "test.y"
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
#line 3788 "test.tab.c"
    break;

  case 73: /* iteracao: FORALL OPENPAR pertinencia CLOSEPAR single_line_statement  */
#line 1500 "test.y"
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
#line 3808 "test.tab.c"
    break;

  case 74: /* $@7: %empty  */
#line 1516 "test.y"
                                                                                                                                                        {
																				escopoCounter++;
																				Push(pilhaEscopo,CriarStack(escopoCounter));
																				
																			}
#line 3818 "test.tab.c"
    break;

  case 75: /* iteracao: FORALL OPENPAR error CLOSEPAR OPENCURLY $@7 statement CLOSECURLY  */
#line 1521 "test.y"
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
																				(yyvsp[0].text) = NULL;
																				Pop(pilhaEscopo);
																				(yyval.node) = ancora;
																				yyerrok;
																			}
#line 3841 "test.tab.c"
    break;

  case 76: /* iteracao: FORALL OPENPAR error CLOSEPAR single_line_statement  */
#line 1539 "test.y"
                                                                                                {
																				no* ancora = (no*)malloc(sizeof(no));
																				(*ancora).numFilhos = 1;
																				(*ancora).filhos[0] = (yyvsp[0].node);
																				(*ancora).tipo = YYSYMBOL_iteracao;
																				char ancora2[] = "ERROR SINGLE";
																				(*ancora).nome = strdup(ancora2);
																				(*ancora).refereTabela = NULL;
																				(*ancora).valor = NULL;
																				(yyval.node) = ancora;
																				(yyvsp[-4].text) = NULL;
																				(yyvsp[-3].text) = NULL;
																				(yyvsp[-1].text) = NULL;
																				yyerrok;
																			}
#line 3861 "test.tab.c"
    break;

  case 77: /* function_call: ID OPENPAR args CLOSEPAR  */
#line 1557 "test.y"
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
#line 3887 "test.tab.c"
    break;

  case 78: /* args: ID args1  */
#line 1588 "test.y"
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
#line 3912 "test.tab.c"
    break;

  case 79: /* args: NUM args1  */
#line 1608 "test.y"
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
#line 3929 "test.tab.c"
    break;

  case 80: /* args: function_call args1  */
#line 1620 "test.y"
                                    {	
								no* ancora = (no*)malloc(sizeof(no));
								(*ancora).numFilhos = 2;
								(*ancora).filhos[0] = (yyvsp[-1].node);
								(*ancora).filhos[0] = (yyvsp[0].node);
								(*ancora).tipo = YYSYMBOL_args;
								char ancora2[] = "function_call";
								(*ancora).nome = strdup(ancora2);
								(*ancora).refereTabela = NULL;
								(yyval.node) = ancora;
							}
#line 3945 "test.tab.c"
    break;

  case 81: /* args: error args1  */
#line 1631 "test.y"
                                        {	
								no* ancora = (no*)malloc(sizeof(no));
								(*ancora).numFilhos = 1;
								(*ancora).filhos[0] = (yyvsp[0].node);
								(*ancora).tipo = YYSYMBOL_args;
								char ancora2[] = "ERROR";
								(*ancora).nome = strdup(ancora2);
								(*ancora).refereTabela = NULL;
								(yyval.node) = ancora;
								yyerrok;
							}
#line 3961 "test.tab.c"
    break;

  case 82: /* args1: COMMA args  */
#line 1645 "test.y"
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
#line 3978 "test.tab.c"
    break;

  case 83: /* args1: %empty  */
#line 1657 "test.y"
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
#line 3993 "test.tab.c"
    break;

  case 84: /* funcargs: type ID  */
#line 1670 "test.y"
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
#line 4017 "test.tab.c"
    break;

  case 85: /* funcargs: type ID COMMA funcargs  */
#line 1689 "test.y"
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
#line 4044 "test.tab.c"
    break;

  case 86: /* funcargs: %empty  */
#line 1711 "test.y"
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
#line 4059 "test.tab.c"
    break;

  case 87: /* funcargs: error  */
#line 1721 "test.y"
                                                        {
									no* ancora = (no*)malloc(sizeof(no));
									(*ancora).numFilhos = 0;
									char ancora2[] = "ERROR";
									(*ancora).nome = strdup(ancora2);
									(*ancora).tipo = YYSYMBOL_funcargs;
									(*ancora).refereTabela = NULL;
									(*ancora).valor = NULL;
									(yyval.node) = ancora;
									yyerrok;
								}
#line 4075 "test.tab.c"
    break;

  case 88: /* $@8: %empty  */
#line 1737 "test.y"
                                                                                                                                                        {
																				escopoCounter++;
																				Push(pilhaEscopo,CriarStack(escopoCounter));
																			}
#line 4084 "test.tab.c"
    break;

  case 89: /* function_declaration: type ID OPENPAR funcargs CLOSEPAR OPENCURLY $@8 statement CLOSECURLY  */
#line 1741 "test.y"
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
#line 4116 "test.tab.c"
    break;

  case 90: /* assignment: ID ASSIGN mathop  */
#line 1772 "test.y"
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
#line 4141 "test.tab.c"
    break;

  case 91: /* variable_declaration: type ID  */
#line 1795 "test.y"
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
#line 4165 "test.tab.c"
    break;

  case 92: /* variable_declaration: type error  */
#line 1814 "test.y"
                                                                                                                                                {
																			no* ancora = (no*)malloc(sizeof(no));
																			(*ancora).numFilhos = 1;
																			(*ancora).filhos[0] = (yyvsp[-1].node);
																			char ancora2[] = "ERROR";
																			(*ancora).nome = strdup(ancora2);
																			(*ancora).tipo = YYSYMBOL_variable_declaration;
																			(*ancora).refereTabela = NULL;
																			(*ancora).valor = NULL;
																			(yyval.node) = ancora;
																			yyerrok;
																		}
#line 4182 "test.tab.c"
    break;

  case 93: /* mathop: mathop PLUS mathop1  */
#line 1833 "test.y"
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
#line 4200 "test.tab.c"
    break;

  case 94: /* mathop: mathop MINUS mathop1  */
#line 1846 "test.y"
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
#line 4218 "test.tab.c"
    break;

  case 95: /* mathop: mathop1  */
#line 1859 "test.y"
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
#line 4234 "test.tab.c"
    break;

  case 96: /* mathop1: mathop1 AST mathop2  */
#line 1873 "test.y"
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
#line 4252 "test.tab.c"
    break;

  case 97: /* mathop1: mathop1 BS mathop2  */
#line 1886 "test.y"
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
#line 4270 "test.tab.c"
    break;

  case 98: /* mathop1: mathop2  */
#line 1899 "test.y"
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
#line 4286 "test.tab.c"
    break;

  case 99: /* mathop2: matharg  */
#line 1914 "test.y"
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
#line 4302 "test.tab.c"
    break;

  case 100: /* mathop2: OPENPAR mathop CLOSEPAR  */
#line 1925 "test.y"
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
#line 4320 "test.tab.c"
    break;

  case 101: /* matharg: ID  */
#line 1941 "test.y"
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
#line 4343 "test.tab.c"
    break;

  case 102: /* matharg: NUM  */
#line 1960 "test.y"
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
#line 4359 "test.tab.c"
    break;

  case 103: /* matharg: function_call  */
#line 1971 "test.y"
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
#line 4375 "test.tab.c"
    break;

  case 104: /* type: SET  */
#line 1985 "test.y"
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
#line 4393 "test.tab.c"
    break;

  case 105: /* type: INT  */
#line 1998 "test.y"
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
#line 4411 "test.tab.c"
    break;

  case 106: /* type: ELEM  */
#line 2011 "test.y"
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
#line 4429 "test.tab.c"
    break;

  case 107: /* type: FLOAT  */
#line 2024 "test.y"
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
#line 4447 "test.tab.c"
    break;


#line 4451 "test.tab.c"

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

#line 2039 "test.y"

/*
void yyerror(char *s){
	extern int yylineno;
	extern char *yytext;

	printf("ERRO %s NO SIMBOLO <%s> na linha <%d>\n",s,yytext,yylineno);
	exit(1);
}
*/

void yyerror(char const *s){
	printf("At line %d, column %d: %s\n",linhaCount,colunaCount,s);

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

