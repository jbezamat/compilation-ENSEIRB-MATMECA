Production de code:

1: EXPINT, compilation d’expression arithmétique sur int
2: EXPMIXTE, compilation d’expression arithmétique sur int et float
3,4,5,6: CONTROL, compilation des conditonelles et boucles
7: PARESSE, compilation paresseuses des booleens, *
8: FONCTION, compilation des fonctions, *

Analyse:

9: TYPEA, analyse des types des expressions arithmetiques
10: TYPEB, analyse des types des expressions booléennes
11,12: PORTEE, analyse de la portée des déclarations,
13: MASQUAGE, analyse du masquage des déclarations, *
14: TYPEC, sous-typage avec cast implicite (a expliciter dans le code cible): int + float -> float, *
15: POINTEUR, expressions avec pointeur, *
16,17,18: TYPEF, vérification des types lors des appels de fonctions, *

Les tests 11,12,16,17 doivent provoquer une erreur à la compilation.


