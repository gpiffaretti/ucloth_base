add_library(ucloth_c_api SHARED
    uclothexport.h
    uclothhandles.h
    uclothstructures.h
    uclothinterface.h
    uclothinterface.cpp
)

set_target_properties(ucloth_c_api
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/../UnityUCloth/Assets/UCloth/Plugins/x64"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/../UnityUCloth/Assets/UCloth/Plugins/x64"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/../UnityUCloth/Assets/UCloth/Plugins/x64"
)

target_link_libraries(ucloth_c_api
    PRIVATE ucloth
)

target_include_directories(ucloth_c_api PUBLIC .)
