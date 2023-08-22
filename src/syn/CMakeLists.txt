add_library(syn STATIC syntax_analysis.cpp)

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/syn ${PROJECT_SOURCE_DIR}/lex)