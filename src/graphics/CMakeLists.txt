
set(SOURCES
        src/batch_render_helper.cpp
        src/constant_buffer.cpp
        src/d3dclass.cpp
        src/depth_stencil_view.cpp
        src/geometry_shader.cpp
        src/graphics.cpp
        src/index_buffer.cpp
        src/mesh.cpp
        src/mesh_loader.cpp
        src/pixel_shader.cpp
        src/render_context.cpp
        src/render_packet.cpp
        src/render_target_view.cpp
        src/sampler_state.cpp
        src/shadow_handler.cpp
        src/texture.cpp
        src/texture_view.cpp
        src/vertex_buffer.cpp
        src/vertex_shader.cpp
)
set(HEADERS
        include/graphics/batch_render_helper.h
        include/graphics/constant_buffer.h
        include/graphics/d3dclass.h
        include/graphics/default_object_constant_buffer.h
        include/graphics/depth_stencil_view.h
        include/graphics/geometry_shader.h
        include/graphics/graphic_resources.h
        include/graphics/graphics.h
        include/graphics/graphics_enums.h
        include/graphics/index_buffer.h
        include/graphics/material.h
        include/graphics/mesh.h
        include/graphics/mesh_loader.h
        include/graphics/pixel_shader.h
        include/graphics/render_context.h
        include/graphics/render_packet.h
        include/graphics/render_target_view.h
        include/graphics/sampler_state.h
        include/graphics/shadow_handler.h
        include/graphics/texture.h
        include/graphics/texture_view.h
        include/graphics/vertex_buffer.h
        include/graphics/vertex_shader.h
)

add_library(graphics STATIC ${SOURCES} ${HEADERS})
add_library(graphics::graphics ALIAS graphics)

target_link_libraries(graphics
        PUBLIC
        core::core
        utils::utils
        PRIVATE
        input::input
        assimp
)
target_include_directories(graphics PUBLIC include)
