#include <stdlib.h>
#include <stdio.h>
#include "type_attributs.h"
#include "utilitaire.h"

type_exp_t mk_type_exp(type_t input_type) {
  type_exp_t t_exp = malloc(sizeof (struct type_exp_s));
  t_exp->num_reg = new_reg();
  t_exp->input_type = input_type;

  return t_exp;
}
