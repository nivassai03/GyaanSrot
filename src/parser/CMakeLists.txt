
set(PARSER_SRC_FILES feedparser.h Qtfeedparser.h Qtfeedparser.cpp)

add_library(Parser ${PARSER_SRC_FILES})

target_link_libraries(Parser Qt6::Core)