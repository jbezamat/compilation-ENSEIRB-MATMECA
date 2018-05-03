#include "symbol_table.h"
#include "list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static list_t S_stack;

/* Noeuds qui correspondent aux elements de la liste */
typedef struct data_s *data_t;

struct data_s {
  char *id;
  symb_value_t value;
};

static data_t copy_data(data_t d) {
  data_t res = malloc(sizeof (struct data_s));
  res->id = strdup(d->id);
  res->value = d->value; /* cassage encapsulation volontaire */

  return d;
}


static int free_data(data_t d) {
  free(d->value);
  free(d);

  return 1;
}


static int print_data(data_t d) {
  printf("ID : %s\n", d->id);
  return 1;
}


/* Table des symboles à plusieurs niveaux à proprement parler */
/* La pile qui contient les differents scopes est une variable globale (statique) */
void ST_create_symbol_table(void) {
  S_stack = list_create((copy_function_t) list_copy,     /* STATIC */
  (free_function_t) list_free,
  (print_function_t) list_print);
  ST_increase_scope();
}

void ST_free_symbol_table(void) {
  list_free(S_stack);
}

/* On realise un parcours du plus local au moins local */
symb_value_t ST_get_symbol_value(char *id) {
  list_t current_scope;
  data_t current_node;

  /* On parcourt la pile */
  list_begin(S_stack);
  while(!list_isend(S_stack)) {
    current_scope = (list_t) list_getdata(S_stack);

    /* On parcourt un scope */
    list_begin(current_scope);
    while(!list_isend(current_scope)) {
      current_node = (data_t) list_getdata(current_scope);

      if (!strcmp(current_node->id, id)) {
        return current_node->value;
      }
      list_next(current_scope);
    }

    list_next(S_stack);
  }

  fprintf(stderr, "Symbol not found : %s\n", id);
  exit(1);
}

/* On ajoute à la portée la plus locale */
void ST_set_symbol_value(char *id, symb_value_t value) {
  list_t current_scope;
  data_t current_node;

  /* On verifie que le symbole n'est pas deja present dans le scope courant */
  list_begin(S_stack);
  current_scope = (list_t) list_getdata(S_stack);

  list_begin(current_scope);
  while(!list_isend(current_scope)) {
    current_node = (data_t) list_getdata(current_scope);

    if (!strcmp(current_node->id, id)) {
      fprintf(stderr, "%s already declared in this scope\n", id);
      exit(1);
    }

    list_next(current_scope);
  }

  /* On ajoute le nouveau noeud à la liste */
  data_t d = malloc(sizeof (struct data_s));
  d->id = strdup(id);
  d->value = value;
  list_add_head(current_scope, (void *) d);
}

void ST_increase_scope(void) {
  list_add_head(S_stack, list_create((copy_function_t) copy_data,
  (free_function_t) free_data,
  (print_function_t) print_data));
}

void ST_decrease_scope(void) {
  list_remove_head(S_stack);
}
