install(DIRECTORY . DESTINATION ${TESTS_PATH}/resultat FILES_MATCHING PATTERN "*.c")
install(DIRECTORY . DESTINATION ${TESTS_PATH}/resultat FILES_MATCHING PATTERN "*.exp")

add_test(NAME test1 COMMAND ${TESTS_PATH}/run_test.sh test1 ${TESTS_PATH}/resultat)
add_test(NAME test2 COMMAND ${TESTS_PATH}/run_test.sh test2 ${TESTS_PATH}/resultat)
add_test(NAME test3 COMMAND ${TESTS_PATH}/run_test.sh test3 ${TESTS_PATH}/resultat)
add_test(NAME test4 COMMAND ${TESTS_PATH}/run_test.sh test4 ${TESTS_PATH}/resultat)
add_test(NAME test5 COMMAND ${TESTS_PATH}/run_test.sh test5 ${TESTS_PATH}/resultat)
add_test(NAME test6 COMMAND ${TESTS_PATH}/run_test.sh test6 ${TESTS_PATH}/resultat)
add_test(NAME test7 COMMAND ${TESTS_PATH}/run_test.sh test7 ${TESTS_PATH}/resultat)
add_test(NAME test8 COMMAND ${TESTS_PATH}/run_test.sh test8 ${TESTS_PATH}/resultat)
add_test(NAME test11 COMMAND ${TESTS_PATH}/run_test.sh test11 ${TESTS_PATH}/resultat)
add_test(NAME test12 COMMAND ${TESTS_PATH}/run_test.sh test12 ${TESTS_PATH}/resultat)
add_test(NAME test13 COMMAND ${TESTS_PATH}/run_test.sh test13 ${TESTS_PATH}/resultat)
add_test(NAME test15 COMMAND ${TESTS_PATH}/run_test.sh test15 ${TESTS_PATH}/resultat)
add_test(NAME test16 COMMAND ${TESTS_PATH}/run_test.sh test16 ${TESTS_PATH}/resultat)
add_test(NAME test17 COMMAND ${TESTS_PATH}/run_test.sh test17 ${TESTS_PATH}/resultat)
add_test(NAME test18 COMMAND ${TESTS_PATH}/run_test.sh test18 ${TESTS_PATH}/resultat)
