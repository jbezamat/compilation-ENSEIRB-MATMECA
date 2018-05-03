flex_target(LEXER ${CMAKE_CURRENT_SOURCE_DIR}/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.c COMPILE_FLAGS "--header-file=${CMAKE_CURRENT_BINARY_DIR}/lexer.h")
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES lexer.h)

if(${CMAKE_BUILD_TYPE} STREQUAL "DEBUG")
  bison_target(PARSER ${CMAKE_CURRENT_SOURCE_DIR}/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c COMPILE_FLAGS "-v")
  set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES parser.output)
else(${CMAKE_BUILD_TYPE} STREQUAL "DEBUG")
  bison_target(PARSER ${CMAKE_CURRENT_SOURCE_DIR}/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c)
endif(${CMAKE_BUILD_TYPE} STREQUAL "DEBUG")

add_flex_bison_dependency(LEXER PARSER)
add_executable(main main.c ${CMAKE_CURRENT_BINARY_DIR}/parser.c ${CMAKE_CURRENT_BINARY_DIR}/lexer.c type_attributs.c utilitaire.c list.c symbol_table.c)
target_link_libraries(main ${FLEX_LIBRARIES} ${BISON_LIBRARIES})
install(TARGETS main RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin)
