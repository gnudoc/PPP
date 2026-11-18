cmake_minimum_required(VERSION 4.1)

project(PPP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# every program gets these:
add_library(exercise INTERFACE)
target_compile_options(exercise INTERFACE
  -Wall
  -Wextra
  -Wpedantic
  -Wshadow
  -Wconversion
  -Wsign-conversion
  -Werror
)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(exercise INTERFACE -fsanitize=address,undefined -fno-omit-frame-pointer)
  target_link_options(exercise INTERFACE -fsanitize=address,undefined)
endif()

# `add_program(ch6/ex03 ch6/ex03.cpp)` builds `build/ch6/ex03`
function(add_program path)
  cmake_path(GET path PARENT_PATH chapter)
  cmake_path(GET path FILENAME name)
  add_executable(${chapter}-${name} ${ARGN})
  target_link_libraries(${chapter}-${name} PRIVATE exercise)
  set_target_properties(${chapter}-${name} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${chapter}
    OUTPUT_NAME ${name})
endfunction()

# .cpp file in a chap dir is a program of its own
file(GLOB sources CONFIGURE_DEPENDS RELATIVE ${CMAKE_SOURCE_DIR} ch*/*.cpp)
foreach(source IN LISTS sources)
  cmake_path(REMOVE_EXTENSION source LAST_ONLY OUTPUT_VARIABLE path)
  add_program(${path} ${source})
endforeach()

# subdir in a chap dir is a program built from all its .cpp files
file(GLOB nested CONFIGURE_DEPENDS RELATIVE ${CMAKE_SOURCE_DIR} ch*/*/*.cpp)
set(directories "")
foreach(source IN LISTS nested)
  cmake_path(GET source PARENT_PATH directory)
  list(APPEND directories ${directory})
endforeach()
list(REMOVE_DUPLICATES directories)
foreach(directory IN LISTS directories)
  file(GLOB parts CONFIGURE_DEPENDS ${directory}/*.cpp)
  add_program(${directory} ${parts})
endforeach()
