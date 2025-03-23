
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
add_subdirectory(src)

add_executable(dx11 WIN32 main.cpp)

target_link_libraries(dx11 renderer::renderer)

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
