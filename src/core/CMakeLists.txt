
set(SOURCES
        src/3d_math.cpp
        src/misc_math.cpp
        src/quaternion.cpp
)

set(HEADERS
        include/core/3d_math.h
        include/core/graphic_defines.h
        include/core/matrix.h
        include/core/misc_math.h
        include/core/quaternion.h
        include/core/vector.h
)

add_library(core STATIC ${HEADERS} ${SOURCES})
add_library(core::core ALIAS core)

target_link_libraries(core PRIVATE utils::utils)
target_include_directories(core PUBLIC include)
