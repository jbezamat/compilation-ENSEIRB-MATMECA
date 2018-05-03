# Fichier : run_test.sh
# Date de dernière édition : 10/12/2017
# Auteur : PARPAITE Thibault
# Description : Script permettant d'executer les tests

#!/bin/bash

#############
# Fonctions #
#############

usage() {
    echo "usage: $0 test tests_path"
    echo "test is the filename of the test (without .c)"
    exit 1
}


############################
# Boucle principale (main) #
############################

TESTS_PATH=$2
RES_FILE="${TESTS_PATH}/$1.ll"
EXP_FILE="${TESTS_PATH}/$1.exp"
ERR_FILE="${TESTS_PATH}/$1.log"

if [ $# -ne 2 ]
then
    usage
else
    ${TESTS_PATH}/main < "${TESTS_PATH}/$1.c" > ${RES_FILE} 2> ${ERR_FILE}
    RES_DIFF=$(diff -i ${EXP_FILE} ${RES_FILE})
    if [ -z ${RES_DIFF} ]
    then
        exit 0
    else
        exit 1
    fi
fi
