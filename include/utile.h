#ifndef _UTILE_H
#define _UTILE_H

/* bloody C and its incapacity to simply encode pairs */

typedef struct {
  int start_while;
  int one;
  int two;
} label;

enum type_enum { INT_T, FLOAT_T, VOID_T, INT_T_P, FLOAT_T_P, VOID_T_P };
typedef enum type_enum type_t;

enum token_exp_enum { PLUS_T, MOINS_T, DIV_T, STAR_T, CONSTANTI_T, CONSTANTF_T, ID_T, PO_T, PF_T};
typedef enum token_exp_enum token_exp_t;

enum token_bool_enum { INF_T, SUP_T, EQUAL_T, DIFF_T, AND_T, OR_T, NOT_T };
typedef enum token_bool_enum token_bool_t;

#endif /* _UTILE_H */
