#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utilitaire.h"
#include "symbol_table.h"
#include "parser.h"
#define BUFFER_SIZE 256

/* Structure, element (data) de la liste chainee param */
typedef struct node_param_s *node_param_t;

struct node_param_s {
  type_t type_param;
  char *id;
};

/* VARIABLES STATIQUES */
static int S_n = 0; /* Numero de label */
static int S_num_reg = 0; /* Numero de registre */

static char *type_to_char(type_t t);

/* CODE FOURNI */
static char *float_to_hex(float f)
{
  char *s = NULL;
  union {
    double a;
    long long int b;
  } u;
  u.a = (double) f;
  asprintf(&s, "%#016llx", u.b);
  return s;
}

static char *double_to_hex(double d)
{
  char *s = NULL;
  union {
    double a;
    long long int b;
  } u;
  u.a = d;
  asprintf(&s, "%#08llx", u.b);
  return s;
}


/* Generation de numero de registre */

int new_reg() {
  return S_num_reg++;
}

/* Generation de numero de label */
label new_label(int is_while) {
  label result;

  if (is_while)
  result.start_while = S_n++;
  else
  result.start_while = -1;

  result.one = S_n++;
  result.two = S_n++;
  return result;
}

label new_single_label(void) {
  label result;
  result.one = S_n++;
  result.two = -1;

  return result;
}


/* Utilise pour reinitialiser le label et les registress a l'entree d'une nouvelle fonction */
label reset_label(void) {
  S_num_reg = 0;
  S_n = 1;
  label result;

  result.start_while = 0;

  result.one = 1;
  result.two = 1;
  return result;
}
/* FIN CODE FOURNI */

/* Dans cette fonction on s'occupe de generer le code pour les casts implicites */
type_t solve_type(type_exp_t reg_1, type_exp_t reg_2) {
  if(reg_1->input_type == INT_T && reg_2->input_type == INT_T) {
    return INT_T;
  }
  else if(reg_1->input_type == FLOAT_T && reg_2->input_type == FLOAT_T) {
    return FLOAT_T;
  }
  else if(reg_1->input_type == INT_T && reg_2->input_type == FLOAT_T) {
    return FLOAT_T;
  }
  else if(reg_1->input_type == FLOAT_T && reg_2->input_type == INT_T) {
    return FLOAT_T;
  }

  return VOID_T;
}


type_t get_type(char *id) {
  return ST_get_symbol_value(id)->type_value;
}


type_t get_return_type(char *id) {
  symb_value_t s_value = ST_get_symbol_value(id);
  //printf("return_type : %d\n", s_value->return_type);
  return s_value->return_type;
}


void set_type(char *id, type_t t) {
  symb_value_t s_value = malloc(sizeof (struct symb_value_s));
  s_value->type_value = t;
  s_value->is_param = 0;
  s_value->int_value = 0;
  s_value->float_value = 0;
  s_value->return_type = 0;
  s_value->list_param = NULL;

  if ((t == INT_T_P) | (t == FLOAT_T_P) | (t == VOID_T_P)) {
    s_value->is_pointer = 1;
  }
  else{
    s_value->is_pointer = 0;
  }
  ST_set_symbol_value(id, s_value);
  printf("  %%%s = alloca %s\n", id, type_to_char(t));
}

void set_type_param(char *id, type_t t) {
  symb_value_t s_value = malloc(sizeof (struct symb_value_s));
  s_value->type_value = t;
  s_value->is_param = 1;
  s_value->is_pointer = 0;
  s_value->int_value = 0;
  s_value->float_value = 0;
  s_value->return_type = 0;
  s_value->list_param = NULL;

  ST_set_symbol_value(id, s_value);
  printf("  %%%s.addr = alloca %s\n", id, type_to_char(t));
}

/* A DEPLACER */

static char *type_to_char(type_t t) {
  switch (t) {
    case INT_T:
    return "i32";
    break;
    case FLOAT_T:
    return "float";
    break;
    case INT_T_P:
    return "i32*";
    break;
    case FLOAT_T_P:
    return "float*";
    break;
    case VOID_T:
    return "void";
    break;
    default:
    return "unknown_type";
    break;
  }
}

/* A DEPLACER */

void display_exp(token_exp_t operateur, type_exp_t new_exp, type_exp_t exp1, type_exp_t exp2){
  char str_operateur[BUFFER_SIZE];
  char str_type[BUFFER_SIZE];

  /* Solve type, cast implicite */
  if(exp2 == NULL){
    /* Cas du MOINS UNA */
  }
  else if(exp1->input_type == INT_T && exp2->input_type == FLOAT_T) {
    printf("  %%r%d = sitofp i32 %%r%d to float\n", new_exp->num_reg, exp1->num_reg); //  %%r2 = sitofp i32 %%r1 to float

    exp1->num_reg = new_exp->num_reg;
    exp1->input_type = FLOAT_T;
    new_exp->num_reg = new_reg();
  }
  else if(exp1->input_type == FLOAT_T && exp2->input_type == INT_T) {
    printf("  %%r%d = sitofp i32 %%r%d to float\n", new_exp->num_reg, exp2->num_reg); //  %%r2 = sitofp i32 %%r2 to float

    exp2->num_reg = new_exp->num_reg;
    exp2->input_type = FLOAT_T;
    new_exp->num_reg = new_reg();
  }

  switch (operateur) {
    case PLUS_T:
    if(new_exp->input_type == INT_T){
      strcpy(str_operateur, "add");
      strcpy(str_type, "i32");
    }
    else if(new_exp->input_type == FLOAT_T){
      strcpy(str_operateur, "fadd");
      strcpy(str_type, "float");
    }
    break;

    case MOINS_T:
    if(new_exp->input_type == INT_T){
      strcpy(str_operateur, "sub");
      strcpy(str_type, "i32");
    }
    else if(new_exp->input_type == FLOAT_T){
      strcpy(str_operateur, "fsub");
      strcpy(str_type, "float");
    }
    if( exp2 == NULL ){
      //cas MOINS UNA
      printf("  %%r%d = %s %s %%r%d, 0\n", new_exp->num_reg, str_operateur, str_type, exp1->num_reg);
      return;
    }
    break;

    case STAR_T:
    if(new_exp->input_type == INT_T){
      strcpy(str_operateur, "mul");
      strcpy(str_type, "i32");
    }
    else if(new_exp->input_type == FLOAT_T){
      strcpy(str_operateur, "fmul");
      strcpy(str_type, "float");
    }
    break;

    case DIV_T:
    if(new_exp->input_type == INT_T){
      strcpy(str_operateur, "sdiv");
      strcpy(str_type, "i32");
    }
    else if(new_exp->input_type == FLOAT_T){
      strcpy(str_operateur, "fdiv");
      strcpy(str_type, "float");
    }
    break;
    default:
    break;
  }
  printf("  %%r%d = %s %s %%r%d, %%r%d\n", new_exp->num_reg, str_operateur, str_type, exp1->num_reg, exp2->num_reg);
}


void display_id(type_exp_t new_exp, char *str){
  printf("  %%r%d = %s;\n", new_exp->num_reg, str);
}

void display_const(token_exp_t operateur, type_exp_t new_exp, void *const_value_p){
  if (operateur == CONSTANTI_T){
    int const_value = *((int *) const_value_p);
    printf("  %%r%d = add i32 %d, 0\n", new_exp->num_reg, const_value);
  }
  else if (operateur == CONSTANTF_T){
    float const_value = *((float *) const_value_p);
    printf("  %%r%d = fadd float %s, 0x%s\n", new_exp->num_reg, float_to_hex(const_value), float_to_hex(0.0));
  }
}


type_t convert_to_pointer(type_t t) {
  switch (t) {
    case INT_T:
    return INT_T_P;
    break;
    case FLOAT_T:
    return FLOAT_T_P;
    break;
    case VOID_T:
    default:
    return VOID_T_P;
    break;
  }
}

void store_id(char *id, type_exp_t exp) {
  symb_value_t s_value = ST_get_symbol_value(id);

  if (s_value->type_value == FLOAT_T && exp->input_type == INT_T) {
    type_exp_t new_exp = malloc(sizeof(struct type_exp_s));
    new_exp->num_reg = new_reg();
    new_exp->input_type = FLOAT_T;
    printf("  %%r%d = sitofp i32 %%r%d to float\n", new_exp->num_reg, exp->num_reg);
    exp = new_exp;
  }
  else if (s_value->type_value == INT_T && exp->input_type == FLOAT_T) {
    type_exp_t new_exp = malloc(sizeof(struct type_exp_s));
    new_exp->num_reg = new_reg();
    new_exp->input_type = INT_T;
    printf("  %%r%d = fptosi float %%r%d to i32\n", new_exp->num_reg, exp->num_reg);
    exp = new_exp;
  }

  if (s_value->type_value != exp->input_type) {
    fprintf(stderr, "Bad type affectation [%s] got %s instead of %s\n", id, type_to_char(exp->input_type), type_to_char(s_value->type_value));
    exit(1);
  }

  printf("  store %s %%r%d, %s* %%%s\n", type_to_char(exp->input_type), exp->num_reg, type_to_char(exp->input_type), id);
}

void store_id_param(char *id, type_t t_param) {
  symb_value_t s_value = ST_get_symbol_value(id);

  if (s_value->type_value != t_param) {
    fprintf(stderr, "Bad type affectation [%s] got %s instead of %s\n", id, type_to_char(t_param), type_to_char(s_value->type_value));
    exit(1);
  }

  printf("  store %s %%%s, %s* %%%s.addr\n", type_to_char(t_param), id, type_to_char(t_param), id);
}


void store_id_pointer(char *id_ptr, char *id_ref) {
  symb_value_t s_value = ST_get_symbol_value(id_ptr);
  type_t t_param = s_value->type_value;

  printf("  store %s %%%s, %s* %%%s\n", type_to_char(t_param), id_ptr, type_to_char(t_param), id_ref);
}


void load_id(type_exp_t exp, char *id){
  symb_value_t s_value = ST_get_symbol_value(id);

  /* Utile pour load %n.addr au lieu de %n dans les fonctions
  Lorsque n est un paramètre et pas un symbole déclaré de manière classique */
  if (s_value->is_param)
  printf("  %%r%d = load %s, %s* %%%s.addr\n", exp->num_reg, type_to_char(exp->input_type), type_to_char(exp->input_type), id);
  else
  printf("  %%r%d = load %s, %s* %%%s\n", exp->num_reg, type_to_char(exp->input_type), type_to_char(exp->input_type), id);
}


void display_bool(token_bool_t operateur,type_exp_t new_bool, type_exp_t bool1, type_exp_t bool2){
  char str_cond[BUFFER_SIZE];
  char str_inst[BUFFER_SIZE];
  char str_type[BUFFER_SIZE];

  if (bool1->input_type != bool2->input_type) {
    perror("Comparison between float and int");
    exit(1);
  }

  if (bool1->input_type == INT_T) {
    strcpy(str_inst, "icmp");
    strcpy(str_type, "i32");
  } else {
    strcpy(str_inst, "fcmp");
    strcpy(str_type, "float"); /* or double ? */
  }

  switch (operateur) {
    case INF_T:
    if(bool1->input_type == INT_T){
      strcpy(str_cond, "slt");
      strcpy(str_type, "i32");
    }
    else if(bool1->input_type == FLOAT_T){
      strcpy(str_cond, "olt");
      strcpy(str_type, "float");
    }
    break;

    case SUP_T:
    if(bool1->input_type == INT_T){
      strcpy(str_cond, "sgt");
      strcpy(str_type, "i32");
    }
    else if(bool1->input_type == FLOAT_T){
      strcpy(str_cond, "ogt");
      strcpy(str_type, "float");
    }
    break;

    case EQUAL_T:
    if(bool1->input_type == INT_T){
      strcpy(str_cond, "eq");
      strcpy(str_type, "i32");
    }
    else if(bool1->input_type == FLOAT_T){
      strcpy(str_cond, "oeq");
      strcpy(str_type, "float");
    };
    break;

    case DIFF_T:
    if(bool1->input_type == INT_T){
      strcpy(str_cond, "ne");
      strcpy(str_type, "i32");
    }
    else if(bool1->input_type == FLOAT_T){
      strcpy(str_cond, "one");
      strcpy(str_type, "float");
    }
    break;

    case AND_T:
    if(bool1->input_type == INT_T){
      strcpy(str_cond, "aa");
      strcpy(str_type, "i32");
    }
    else if(bool1->input_type == FLOAT_T){
      strcpy(str_cond, "zzz");
      strcpy(str_type, "float");
    }
    break;

    case OR_T:
    if(bool1->input_type == INT_T){
      strcpy(str_cond, "bb");
      strcpy(str_type, "i32");
    }
    else if(bool1->input_type == FLOAT_T){
      strcpy(str_cond, "ff");
      strcpy(str_type, "float");
    }
    break;

    case NOT_T:
    if(bool1->input_type == INT_T){
      strcpy(str_cond, "cc");
      strcpy(str_type, "i32");
    }
    else if(bool1->input_type == FLOAT_T){
      strcpy(str_cond, "ss");
      strcpy(str_type, "float");
    }
    break;
    default:
    break;
  }
  printf("  %%r%d = %s %s %s %%r%d, %%r%d\n", new_bool->num_reg, str_inst, str_cond, str_type, bool1->num_reg, bool2->num_reg);
}


void display_funhead(type_t type_f, char* id, list_t list_param){
  /* On ajoute la fonction à la table des symboles */
  symb_value_t s_value = malloc(sizeof (struct symb_value_s));
  s_value->list_param = list_param;
  s_value->return_type = type_f;
  s_value->type_value = 0;
  s_value->is_param = 0;
  s_value->is_pointer = 0;
  s_value->int_value = 0;
  s_value->float_value = 0;

  ST_set_symbol_value(id, s_value);

  /* Puis on l'affiche */
  char *type_f_str = type_to_char(type_f);
  node_param_t current_param;

  printf("define %s @%s(", type_f_str, id);

  list_begin(list_param);

  /* Premier affichage sans la virgule */
  if (!list_isend(list_param)) {
    current_param = (node_param_t) list_getdata(list_param);
    printf("%s %%%s", type_to_char(current_param->type_param), current_param->id);
    list_next(list_param);
  }

  while(!list_isend(list_param)) {
    current_param = (node_param_t) list_getdata(list_param);
    printf(", %s %%%s",type_to_char(current_param->type_param), current_param->id);
    list_next(list_param);
  }

  printf(") {\n");
  printf("L0:\n");

  /* On alloue les params sur la pile */
  list_begin(list_param);

  while(!list_isend(list_param)) {
    current_param = (node_param_t) list_getdata(list_param);
    set_type_param(current_param->id, current_param->type_param);
    store_id_param(current_param->id, current_param->type_param);
    list_next(list_param);
  }
}


void display_ret(type_exp_t ret){
  printf("  ret %s %%r%d\n", type_to_char(ret->input_type), ret->num_reg);
}


void display_funapp(type_exp_t new_exp, char *id_function, list_t list_arg) {
  type_exp_t current_arg;
  node_param_t current_param;
  symb_value_t s_value = ST_get_symbol_value(id_function);

  /* Premier parcours de verification et de cast */
  list_t list_param = s_value->list_param;

  if (list_size(list_arg) != list_size(list_param)) {
    fprintf(stderr, "Bad function call [%s] expected %zu arguments, but %zu were given\n", id_function, list_size(list_param), list_size(list_arg));
    exit(1);
  }

  list_begin(list_arg);
  list_begin(list_param);

  while (!list_isend(list_arg)) {
    current_arg = (type_exp_t) list_getdata(list_arg);
    current_param = (node_param_t) list_getdata(list_param);

    if (current_arg->input_type == INT_T && current_param->type_param == FLOAT_T) {
      int n_reg = new_reg();
      printf("  %%r%d = sitofp i32 %%r%d to float\n", n_reg, current_arg->num_reg);
      current_arg->num_reg = n_reg;
      current_arg->input_type = FLOAT_T;
    } else if (current_arg->input_type == FLOAT_T && current_param->type_param == INT_T) {
      int n_reg = new_reg();
      printf("  %%r%d = fptosi float %%r%d to i32\n", n_reg, current_arg->num_reg);
      current_arg->num_reg = n_reg;
      current_arg->input_type = INT_T;
    }

    if (current_arg->input_type != current_param->type_param) {
      fprintf(stderr, "Bad function call [%s] expected argument of type %s but got %s\n", id_function, type_to_char(current_param->type_param), type_to_char(current_arg->input_type));
      exit(1);
    }

    list_next(list_arg);
    list_next(list_param);
  }

  /* Deuxieme parcourt : on ecrit l'appel */
  printf("  %%r%d = call %s @%s(", new_exp->num_reg, type_to_char(s_value->return_type), id_function);

  list_begin(list_arg);

  /* Premier argument sans la virgule */
  if (!list_isend(list_arg)) {
    current_arg = (type_exp_t) list_getdata(list_arg);
    printf("%s %%r%d", type_to_char(current_arg->input_type), current_arg->num_reg);
    list_next(list_arg);
  }

  /* Parcours de la liste d'arguments */
  while(!list_isend(list_arg)) {
    current_arg = (type_exp_t) list_getdata(list_arg);
    printf(", %s %%r%d", type_to_char(current_arg->input_type), current_arg->num_reg);
    list_next(list_arg);
  }

  printf(")\n");
}


/* Liste param les elements sont des node_param_t */

static node_param_t node_copy(node_param_t n) {
  node_param_t new_node = malloc(sizeof(struct node_param_s));
  new_node->type_param = n->type_param;
  new_node->id = strdup(n->id);
  return new_node;
}


static int node_free(node_param_t n) {
  free(n->id);
  free(n);
  return 1;
}

static int node_print(node_param_t n) {
  printf("[NODE_param] type_param : %d, ID : %s\n", n->type_param, n->id);
  return 1;
}

list_t list_param_create(void) {
  list_t res = list_create((copy_function_t) node_copy,
  (free_function_t) node_free,
  (print_function_t) node_print);
  return res;
}


void list_param_add_head(list_t list, type_t type, char *id) {
  node_param_t n = malloc(sizeof(struct node_param_s));
  n->type_param = type;
  n->id = id;

  list_add_head(list, (void *) n);
  node_free(n);
}

/* Liste arg (les elements sont des expressions) */

static type_exp_t exp_copy(type_exp_t exp) {
  type_exp_t res = malloc(sizeof (struct type_exp_s));
  res->num_reg = exp->num_reg;
  res->input_type = exp->input_type;

  return res;
}


static int exp_free(type_exp_t exp) {
  free(exp);
  return 1;
}


static int exp_print(type_exp_t exp) {
  printf("[EXP] Num reg : %d, Input type : %d\n", exp->num_reg, exp->input_type);
  return 1;
}

list_t list_arg_create(void) {
  list_t res = list_create((copy_function_t) exp_copy,
  (free_function_t) exp_free,
  (print_function_t) exp_print);
  return res;
}
