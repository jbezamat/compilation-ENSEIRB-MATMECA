#ifndef _TYPE_ATTRIBUTS_H
#define _TYPE_ATTRIBUTS_H

#include "utile.h"

struct type_exp_s {
  int num_reg;
  type_t input_type; //voir y.tab.h
};

typedef struct type_exp_s *type_exp_t;

type_exp_t mk_type_exp(type_t input_type);

#endif /* _TYPE_ATTRIBUTS_H */
