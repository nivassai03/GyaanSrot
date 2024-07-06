set(MODELS_SRC_FILES source.h source.cpp category.h category.cpp article.h article.cpp dbmanager.h dbmanager.cpp)

add_library(Database ${MODELS_SRC_FILES})

target_link_libraries(Database Qt6::Core Qt6::Sql)