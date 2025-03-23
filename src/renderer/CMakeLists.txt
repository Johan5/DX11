
set(SOURCES
        src/cube.cpp
        src/game_application.cpp
        src/game_object.cpp
        src/light_source.cpp
        src/systemclass.cpp
        src/world.cpp
)
set(HEADERS
        include/renderer/cube.h
        include/renderer/game_application.h
        include/renderer/game_object.h
        include/renderer/light_source.h
        include/renderer/systemclass.h
        include/renderer/world.h
)

add_library(renderer STATIC ${SOURCES} ${HEADERS})
add_library(renderer::renderer ALIAS renderer)

target_link_libraries(renderer
        PUBLIC
        core::core
        graphics::graphics
        input::input
        utils::utils
        assimp
)
target_include_directories(renderer PUBLIC include)

