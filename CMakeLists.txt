cmake_minimum_required(VERSION 3.5)

project(libcbs)
add_compile_options(-std=c2x -Wall -Werror -Wno-unused-function)

file(GLOB_RECURSE src src/cbs/*.c)

add_library(lib_static STATIC ${src})
set_target_properties(lib_static PROPERTIES OUTPUT_NAME cbs)
target_include_directories(lib_static PUBLIC src)

add_library(lib_shared SHARED ${src})
set_target_properties(lib_shared PROPERTIES OUTPUT_NAME cbs)
target_include_directories(lib_shared PUBLIC src)

add_executable(test ${src} src/main.c)
target_compile_options(test PUBLIC -fsanitize=undefined -O2 -g)
target_include_directories(test PUBLIC src)
target_link_libraries(test ubsan)

file(GLOB inc src/cbs/*.h)
install(FILES ${inc} DESTINATION include/cbs)
install(FILES build/libcbs.a DESTINATION lib)
install(FILES build/libcbs.so DESTINATION lib)
