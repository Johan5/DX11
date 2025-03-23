


set(SOURCES
        3d_math.cpp
        assert.cpp
        batch_render_helper.cpp
        camera_base.cpp
        constant_buffer.cpp
        cube.cpp
        d3dclass.cpp
        depth_stencil_view.cpp
        game_application.cpp
        game_object.cpp
        geometry_shader.cpp
        graphics.cpp
        index_buffer.cpp
        input_handler.cpp
        input_handler_types.cpp
        light_source.cpp
        logger.cpp
        mesh.cpp
        mesh_loader.cpp
        misc_math.cpp
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

add_library(renderer STATIC ${SOURCES} ${HEADERS})
add_library(renderer::renderer ALIAS renderer)

target_link_libraries(renderer assimp)

