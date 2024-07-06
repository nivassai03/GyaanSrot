set(UTILS_SRC_FILES util.h util.cpp dirutil.h Qtdirutil.h Qtdirutil.cpp)
add_library(Util ${UTILS_SRC_FILES})
target_link_libraries(Util Qt6::Core)