cmake_minimum_required (VERSION 3.0)

# Definitions
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(SOURCES src/main.c src/logic.c src/graphics.c)

project(minesweeper)

add_executable(minesweeper ${SOURCES})

configure_file(tileset.png ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tileset.png COPYONLY)

if(UNIX)
    execute_process(COMMAND sdl-config --cflags --libs OUTPUT_VARIABLE SDL_CONFIG)
    add_definitions(${SDL_CONFIG})
    find_package(SDL REQUIRED)
    find_package(SDL_image REQUIRED)
    include_directories(${SDL_INCLUDE_DIR})
    include_directories(${SDL_IMAGE_INCLUDE_DIRS})
    target_link_libraries(minesweeper
        ${SDL_LIBRARY}
        ${SDL_IMAGE_LIBRARIES}
        SDL_gfx
        m
    )
endif()

if(WIN32)
    configure_file(deps/win32/bin/SDL.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/SDL.dll COPYONLY)
    configure_file(deps/win32/bin/SDL_gfx.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/SDL_gfx.dll COPYONLY)
    configure_file(deps/win32/bin/SDL_image.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/SDL_image.dll COPYONLY)
    configure_file(deps/win32/bin/libpng15-15.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/libpng15-15.dll COPYONLY)
    configure_file(deps/win32/bin/zlib1.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/zlib1.dll COPYONLY)

    include_directories(
        deps/win32/include/
        deps/win32/include/SDL
        deps/win32/include/SDL_gfx
    )

    find_library(SDL SDL HINTS deps/win32/bin)
    find_library(SDL_gfx SDL_gfx HINTS deps/win32/bin)
    find_library(SDL_image SDL_image HINTS deps/win32/bin)
    find_library(SDLmain SDLmain HINTS deps/win32/bin)
    
    target_link_libraries(minesweeper
        ${SDL}
        ${SDL_gfx}
        ${SDL_image}
        ${SDLmain}
    )
endif()
