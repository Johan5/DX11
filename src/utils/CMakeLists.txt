


set(SOURCES
        src/assert.cpp
        src/logger.cpp
        src/string_util.cpp
)
set(HEADERS
        include/utils/assert.h
        include/utils/logger.h
        include/utils/stb_image.h
        include/utils/stb_image_include.h
        include/utils/string_util.h
)

add_library(utils STATIC ${SOURCES} ${HEADERS})
add_library(utils::utils ALIAS utils)

target_include_directories(utils PUBLIC include)
