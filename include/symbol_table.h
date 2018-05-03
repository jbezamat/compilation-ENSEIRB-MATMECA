#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "utile.h"
#include "list.h"

/* Une table des symboles est une pile. Chaque niveau correspond à une visibilité de block (liste chainée) */

/* Un symbole est caracterise par son id. Un symbole peut-être une variable, auquel cas on sauvegarde son type, sa valeur, etc.
 * Un symbole peut aussi être une fonction, dans ce cas on sauvegarde son prototype.
 * On utilise une union pour contenir ses informations */
struct symb_value_s {
	/* SYMBOLE = variable */
	type_t type_value;
	int int_value;
	float float_value;
	int is_param; /* Utile pour load %n.addr au lieu de %n dans les fonctions */
	int is_pointer;
	/* SYMBOLE = fonction */
	type_t return_type;
	list_t list_param;
};

typedef struct symb_value_s *symb_value_t;

void ST_create_symbol_table(void);
void ST_free_symbol_table(void);
void ST_increase_scope(void);
void ST_decrease_scope(void);

symb_value_t ST_get_symbol_value(char *id);
void ST_set_symbol_value(char *id, symb_value_t value);

#endif
