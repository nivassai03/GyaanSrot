set(GUI_SRC_FILES "feeditem.h" "feeditem.cpp" "feedlist.h" "feedlist.cpp" "elidedlabel.h" "elidedlabel.cpp" "sourcetree.h" "sourcetree.cpp" "webview.h" "webview.cpp")

add_library(Gui ${GUI_SRC_FILES})

target_link_libraries(Gui Qt6::Core Qt6::Widgets Qt6::WebEngineWidgets Database Network)