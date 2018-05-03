**PROJET SGBD : COMPILATION**
=======================
*2ème année école d'ingénieur ENSEIRB-MATMECA*
=======================

Fait par
=======================
Parpaite Thibault
Bezamat Jérémy

Description des dossiers
=======================

  * include      : Contient tous les .h
  * src          : Contient toutes les sources du compilateur
  * test         : Contient tous les fichiers de test (.c et.exp)
  * test/resultat: Contient tous les fichiers de test demandés par le sujet


Run
=======================
  * $ mkdir build
  * $ cd build
  * $ cmake ..
  * $ make install

L'éxecutable main se trouve dans /build/bin

Run test
=======================
  * $ mkdir build
  * $ cd build
  * $ cmake ..
  * $ make install
  * $ make test

Les resultats des tests se trouvent dans /build/bin/tests/
Vous y trouverez les fichiers originaux (.c), les résultats renvoyés par notre compilateur (.ll), les resultats attendus (.exp pour expected) et les log de la compilation (.log) qui sont vides si la compilation s'est déroulée correctement et qui contiennent les erreurs sinon.

Tous les tests doivent passer sauf :
-----------------------

  * Le test 11 provoque un "Symbol not found : a" car on fait un if(a == 1) alors que a n'est pas défini (voir log)
  * Le test 12 provoque un "Symbol not found : a" même chose que pour le test 11 (voir log)
  * Le test 16 provoque un "Bad function call [f] expected 1 arguments, but 2 were given" car on essaye d'envoyer 2 arguments à une fonction qui n'en contient qu'un (voir log)
  * Le test 17 provoque un "Bad type affectation [b] got void instead of i32" car on fait int b = f(a); alors que void f(int), on essaye de mettre un "void" dans un int
