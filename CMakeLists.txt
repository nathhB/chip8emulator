cmake_minimum_required(VERSION 3.0)
project(chip-8)
enable_testing()

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")

add_compile_options(-Wall -Wextra -Wpedantic -Wno-gnu-binary-literal)

add_executable(disassembler disassembler.c chip-8.c)
add_executable(tests tests.c chip-8.c)
add_executable(emulator emulator.c chip-8.c rom_picker.c)

add_test(NAME tests COMMAND tests)

target_link_libraries(emulator ${RAYLIB_LIBRARY_PATH} m)
target_include_directories(emulator PUBLIC "${RAYLIB_INCLUDE_PATH}")

if(WIN32)
    target_link_libraries(raylib_client wsock32 ws2_32 opengl32 gdi32 winmm)
    target_link_libraries(raylib_server wsock32 ws2_32)
endif()

if (APPLE)
    target_link_libraries(emulator "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo")
endif (APPLE)

if (EMSCRIPTEN)
    set_target_properties(emulator PROPERTIES LINK_FLAGS "-s USE_GLFW=3 \
    --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/shell.html \
    -s ASYNCIFY \
    -s ALLOW_MEMORY_GROWTH=1 \
    --preload-file ${ROMS_DIR}@roms")

    set_target_properties(emulator PROPERTIES SUFFIX ".html")
    add_compile_definitions(ROMS_DIR_PATH="roms")
else ()
    add_compile_definitions(ROMS_DIR_PATH="${ROMS_DIR}")
endif (EMSCRIPTEN)
