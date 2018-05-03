#ifndef _UTILITAIRE_H
#define _UTILITAIRE_H

#include "type_attributs.h"
#include "utile.h"
#include "list.h"

int new_reg(void);
label new_label(int is_while);
label new_single_label(void);
label reset_label(void);

type_t solve_type(type_exp_t reg_1, type_exp_t reg_2);
type_t get_type(char *id);
type_t get_return_type(char *id);
void set_type(char *id, type_t t);
void set_type_param(char *id, type_t t);

void display_exp(token_exp_t operateur,type_exp_t new_exp, type_exp_t exp1, type_exp_t exp2);
void display_bool(token_bool_t operateur,type_exp_t new_bool, type_exp_t bool1, type_exp_t bool2);
void display_id(type_exp_t new_exp, char *str);
void display_const(token_exp_t operateur, type_exp_t new_exp, void *const_value_p);

void display_funhead(type_t type_f, char* id, list_t list_param);
void display_funapp(type_exp_t new_exp, char *id_function, list_t list_arg);
void display_ret(type_exp_t ret);

type_t convert_to_pointer(type_t t);

void store_id(char *id, type_exp_t exp);
void store_id_param(char *id, type_t t_param);
void store_id_pointer(char *id_ptr, char *id_ref);
void load_id(type_exp_t exp, char *id);


/* Liste chainées et fonctions associées */

list_t list_param_create(void);
void list_param_add_head(list_t list, type_t type, char *id);
list_t list_arg_create(void);


void affect_pointer(char *id_ptr, char *id);

#endif /* _UTILITAIRE_H */
