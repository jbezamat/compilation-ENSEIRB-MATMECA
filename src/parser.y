%code top {
#include <stdio.h>
}

%code requires {
#include "utile.h"
#include "symbol_table.h"
#include "type_attributs.h"
#include "utilitaire.h"
}

%code {
int yylex(void);
void yyerror (char* s);
void yywrap (void);
}

/* Le type union (plus ou moins mal définissable en C) permet de représenter l'union disjointe
de deux types.En maths, on peut poser A + B = ({1} x A) U ({2} x B).

Un élement de A + B est  alors de la forme (t,v) avec t = 1 ou 2. La valeur de t permet de savoir
si la valeur v est de type A  (lorsque t = 1) ou de type A (lorsque t = 2).

Le type union ci-dessous fait (presque) la même chose pour le type attribut, le "typage" d'un
attribut se faisant à l'utilisation par $<type>n. Example: $<sid>-3 indique que l'atrribut $-3
sera lue comme un char * (le type de sid). */


%union {
    int n;
    float f;
    int reg;
    char *id;
    label lab;
    type_exp_t t_exp;
    type_t t_type;
    list_t t_list;
}

%token <n> CONSTANTI /* attribut d’une constante entière = int */
%token <f> CONSTANTF /* attribut d’une constante flottante = float */

%token <id> ID  /* attribut d’un registre = sid */

%token IF ELSE


%token <t_type> INT FLOAT VOID

%token VIR PV AO AF PO PF
%token UNTIL DO WHILE RETURN

%token PLUS MOINS STAR DIV  EQ ESP
%token INF EQUAL SUP DIFF
%token AND OR NOT

%left PLUS MOINS
%left REF STAR DIV    /* * et / plus prioritaires que + et - */

%nonassoc NOT
%left OR
%left AND
%nonassoc UNA    /* pseudo token pour assurer une priorite locale */
%nonassoc ELSE

%type <t_exp> exp bool fun_app /* attribut d’une expr = valeur entiere */
%type <lab> if else while do_while
%type <t_type> typename type var_decl aff aff_pointer
%type <t_list> param_list arglist args

%start prog



%%

prog : block;


block : decl_list inst_list
      ;


// Declarations

decl_list : decl decl_list
          |
          ;


decl : var_decl PV
     | fun_decl
     ;


var_decl : type vlist            /* On transmet par héritage la valeur de type en utilisant la pile (parcours LR unique), équivalent de $2 = $1 */


fun_decl : type fun
         ;

fun : fun_head fun_body


fun_head : ID PO PF                { display_funhead($<t_type>0, $1, list_param_create()); }
         | ID PO param_list PF     { display_funhead($<t_type>0, $1, $3); }
         ;
/* Le type de retour de la fonction est le dernier elem sur la pile */


fun_body : ao { reset_label(); } block af  { printf("}\n") ;}


ao: AO                             { ST_increase_scope(); }


af: AF                             { ST_decrease_scope(); }


type : typename pointer            { $$ = convert_to_pointer($1); }
     | typename                    { $$ = $1;  }
     ;


typename : INT                     { $$ = $1; }
         | FLOAT                   { $$ = $1; }
         | VOID                    { $$ = $1; }
         ;


pointer : pointer STAR
        | STAR
        ;


param_list : type ID vir param_list          { $$ = $4; list_param_add_head($$, $1, $2);   }
           | type ID                         { $$ = list_param_create(); list_param_add_head($$, $1, $2); }
           ;


vlist : ID vir { $<t_type>$ = $<t_type>0; set_type($1, $<t_type>$); } vlist     /* On utilise un attribut hérité (le type) qu'on récupère sur la pile $0       */
      | ID     { set_type($1, $<t_type>0); }                                    /* L'action $$ = $0 est nécessaire pour remettre l'attribut type comme dernier */
      ;                                                                         /* élément de la pile (pour que la suite de la liste puisse également hériter) */


vir : VIR;

// Intructions

inst_list : inst inst_list
          |
          ;


inst : exp PV
     | ao block af
     | cond
     | loop
     | aff
     | aff_pointer
     | ret
     | PV
     ;


loop : while bool_cond DO ao block af                 { printf("  br label %%L%i\n", $1.start_while); printf("L%i:\n", $1.two); }
     | do_while ao block af WHILE PO bool PF PV       { printf("  br i1 %%r%d, label %%L%d, label %%L%d\n", $7->num_reg, $1.one, $1.two);
                                                        printf("L%i:\n", $1.two); }
     ;


while : WHILE                          { $<lab>$ = new_label(1); printf("L%i:\n", $<lab>$.start_while);  } /* Creation de trois labels */


do_while : DO                          { $<lab>$ = new_label(0); printf("L%i:\n", $<lab>$.one); } /* Creation de deux labels */


fun_app : ID PO args PF                { $$ = mk_type_exp(get_return_type($1)); display_funapp($$, $1, $3); }


args : arglist                         { $$ = $1; }
     |                                 { $$ = list_arg_create(); }
     ;


arglist : exp VIR arglist              { $$ = $3; list_add_head($$, (void *) $1); }
        | exp                          { $$ = list_arg_create(); list_add_head($$, (void *) $1); }
        ;


aff : ID EQ exp PV                     { store_id($1, $3); } //printf("%s = R%i;\n", $1, $3); }


aff_pointer: ID EQ REF ID PV           { store_id_pointer($1, $4); }


ret : RETURN PV
    | RETURN exp PV                    { display_ret($2);}
    ;


cond : if bool_cond inst %prec UNA     { printf("  br label L%i\n", $1.two); printf("L%i:\n", $1.two); }
     | if bool_cond inst else inst     { printf("  br label L%i\n", $4.one); printf("L%i:\n", $4.one); } /* Label qu'on vient de generer dans else */
     ;


bool_cond : PO bool PF                 { printf("  br i1 %%r%d, label %%L%d, label %%L%d\n", $2->num_reg, $<lab>0.one, $<lab>0.two);
                                         printf("L%i:\n", $<lab>0.one); }
// L'attribut du if est juste avant sur la pile.


if : IF                                { $$ = new_label(0); }


else : ELSE                            { $$ = new_label(0); printf("  br label L%i\n", $$.one); printf("L%i:\n", $<lab>-2.two); }
// L'attribut du if se trouve à trois niveau en dessus, sur la pile,
// en effet, le else apparait sur la pile toujorus trois coups après le if (voir rêgle du cond)


bool : exp INF exp      { $$ = mk_type_exp(INT_T); display_bool(INF_T, $$, $1, $3); }
     | exp SUP exp      { $$ = mk_type_exp(INT_T); display_bool(SUP_T, $$, $1, $3);}
     | exp EQUAL exp    { $$ = mk_type_exp(INT_T); display_bool(EQUAL_T, $$, $1, $3); }
     | exp DIFF exp     { $$ = mk_type_exp(INT_T); display_bool(DIFF_T, $$, $1, $3); }
     | bool AND { $<lab>$ = new_single_label(); printf("  br i1 %%r%d, label %%L%d, label %%L%d\n", $1->num_reg, $<lab>$.one, $<lab>-1.two);
                  printf("L%i:\n", $<lab>$.one); } bool { $$ = $4; }
     | bool OR  { $<lab>$ = new_single_label(); printf("  br i1 %%r%d, label %%L%d, label %%L%d\n", $1->num_reg, $<lab>-1.one, $<lab>$.one);
                  printf("L%i:\n", $<lab>$.one); } bool { $$ = $1; }
     | NOT bool         { $$ = mk_type_exp(INT_T); display_bool(NOT_T, $$, $2, NULL); }
     | PO bool PF       { $$ = $2; }
     ;

/* and: AND et or: OR ont été remplacés par des middle/rules car sinon il y avait un conflit shift/reduce */

exp : MOINS exp %prec UNA  { $$ = mk_type_exp($2->input_type); display_exp(MOINS_T, $$, $2, NULL); }
    | exp PLUS exp         { $$ = mk_type_exp(solve_type($1, $3)); display_exp(PLUS_T, $$, $1, $3); }
    | exp MOINS exp        { $$ = mk_type_exp(solve_type($1, $3)); display_exp(MOINS_T, $$, $1, $3); }
    | exp STAR exp         { $$ = mk_type_exp(solve_type($1, $3)); display_exp(STAR_T, $$, $1, $3); }
    | exp DIV exp          { $$ = mk_type_exp(solve_type($1, $3)); display_exp(DIV_T, $$, $1, $3); }
    | PO exp PF            { $$ = $2; }
    | ID                   { $$ = mk_type_exp(get_type($1)); load_id($$, $1); }
    | STAR ID              { $$ = mk_type_exp(get_type($2)); load_id($$, $2); }
    | CONSTANTI            { $$ = mk_type_exp(INT_T); display_const(CONSTANTI_T, $$, (void *) &$1); } // Polymorphisme
    | CONSTANTF            { $$ = mk_type_exp(FLOAT_T); display_const(CONSTANTF_T, $$, (void *) &$1); }
    | fun_app              { $$ = $1; }
    ;

%%

void yyerror (char* s) {
    fprintf (stderr, "%s\n", s);
}
