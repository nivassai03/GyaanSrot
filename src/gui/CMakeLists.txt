set(GUI_SRC_FILES "mainwindow.h" "mainwindow.cpp" "feeditem.h" "feeditem.cpp" "feedlist.h" "feedlist.cpp")

add_library(Gui ${GUI_SRC_FILES})

target_link_libraries(Gui Qt6::Core Qt6::Widgets)