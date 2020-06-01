add_library(ucloth STATIC
    uclothcommon.hpp
    world.hpp
    world.cpp
    pbdsimulation.hpp
    pbdsimulation.cpp
    constraint.hpp
    constraint.cpp
    uclothdebug.h
    uclothdebug.cpp
)

target_link_libraries(ucloth
    PUBLIC umath
)

target_include_directories(ucloth PUBLIC .)