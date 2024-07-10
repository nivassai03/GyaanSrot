set(NETWORK_SRC_FILES feedfetcher.h Qtfeedfetcher.h Qtfeedfetcher.cpp feedimagedownloader.h Qtfeedimagedownloader.h Qtfeedimagedownloader.cpp)

add_library(Network ${NETWORK_SRC_FILES})

target_link_libraries(Network PRIVATE Qt6::Core Qt6::Gui Qt6::Network Database)