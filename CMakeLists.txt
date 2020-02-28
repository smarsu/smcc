cmake_minimum_required(VERSION 3.5.1)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_compile_options(-std=c++11)
add_compile_options(-O0)
add_compile_options(-Wall)
add_compile_options(-Werror)
add_compile_options(-g)

set(__smcc_lib CACHE INTERNAL "__smcc_lib")
function(smcc_library TARGET SRC)
  add_library(${TARGET} STATIC ${SRC})
  set(__smcc_lib "${__smcc_lib};${TARGET}" CACHE INTERNAL "")
endfunction()

include_directories(.)
add_subdirectory(core)

add_executable(smcc smcc.cc)
target_link_libraries(smcc ${__smcc_lib})
target_link_libraries(smcc libglog.so)
