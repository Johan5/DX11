
set(SOURCES
        src/camera_base.cpp
        src/input_handler.cpp
        src/input_handler_types.cpp
        src/orthographic_camera.cpp
        src/perspective_camera.cpp
)
set(HEADERS
        include/input/camera_base.h
        include/input/input_enums.h
        include/input/input_handler.h
        include/input/input_handler_types.h
        include/input/orthographic_camera.h
        include/input/perspective_camera.h
)

add_library(input STATIC ${SOURCES} ${HEADERS})
add_library(input::input ALIAS input)

target_link_libraries(input PUBLIC core::core utils::utils)
target_include_directories(input PUBLIC include)
