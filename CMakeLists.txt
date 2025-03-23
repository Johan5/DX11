
cmake_minimum_required(VERSION 3.26)

project(dx11)

set(CMAKE_CXX_STANDARD 23)
# Fail configuration if compiler does not support the specified CMAKE_CXX_STANDARD
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Disable compiler-specific extensions
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    # Disable non-conforming compiler behaviors
    add_compile_options(/permissive-)
    # Disable non-conforming pre-processor behaviors
    add_compile_options(/Zc:preprocessor)
endif ()

add_subdirectory(thirdparty)

set(SOURCES
        3d_math.cpp
        assert.cpp
        batch_render_helper.cpp
        camera_base.cpp
        constant_buffer.cpp
        cube.cpp
        d3dclass.cpp
        depth_stencil_view.cpp
        dx11test.cpp
        game_application.cpp
        game_object.cpp
        geometry_shader.cpp
        graphics.cpp
        index_buffer.cpp
        input_handler.cpp
        input_handler_types.cpp
        light_source.cpp
        logger.cpp
        main.cpp
        mesh.cpp
        mesh_loader.cpp
        misc_math.cpp
        NatvisFile.natvis
        orthographic_camera.cpp
        perspective_camera.cpp
        pixel_shader.cpp
        quaternion.cpp
        render_context.cpp
        render_packet.cpp
        render_target_view.cpp
        sampler_state.cpp
        shadow_handler.cpp
        string_util.cpp
        systemclass.cpp
        texture.cpp
        texture_view.cpp
        vector.cpp
        vertex_buffer.cpp
        vertex_shader.cpp
        world.cpp
)
set(HEADERS
        3d_math.h
        assert.h
        batch_render_helper.h
        camera_base.h
        constant_buffer.h
        cube.h
        d3dclass.h
        default_object_constant_buffer.h
        depth_stencil_view.h
        game_application.h
        game_object.h
        geometry_shader.h
        graphic_resources.h
        graphics.h
        graphics_enums.h
        index_buffer.h
        input_enums.h
        input_handler.h
        input_handler_types.h
        light_source.h
        logger.h
        material.h
        matrix.h
        mesh.h
        mesh_loader.h
        misc_math.h
        NatvisFile.natvis
        orthographic_camera.h
        perspective_camera.h
        pixel_shader.h
        quaternion.h
        render_context.h
        render_packet.h
        render_target_view.h
        sampler_state.h
        shadow_handler.h
        stb_image.h
        stb_image_include.h
        string_util.h
        systemclass.h
        texture.h
        texture_view.h
        utils.h
        vector.h
        vertex_buffer.h
        vertex_shader.h
        world.h
)

add_executable(dx11 WIN32 ${HEADERS} ${SOURCES})

target_link_libraries(dx11 assimp)

# Copy shaders after build
add_custom_command(TARGET dx11 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:dx11>/shaders
        COMMENT "Copying shaders..."
)
# Copy shaders after build
add_custom_command(TARGET dx11 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:dx11>/assets
        COMMENT "Copying assets..."
)
