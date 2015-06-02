cmake_minimum_required(VERSION 2.6)
project(metafactor)

set(CMAKE_CXX_COMPILER g++)

set(CMAKE_BUILD_TYPE Release)

add_executable(metafactor metafactor.cpp)
add_executable(metaprimes metaprimes.cpp)

# Set default values 
if(NOT DEFINED ${CPP})
set(CPP "11" CACHE STRING "")
endif(NOT DEFINED ${CPP})

if(NOT DEFINED ${NUM})
set(NUM "0" CACHE STRING "")
endif(NOT DEFINED ${NUM})

if(NOT DEFINED ${MODE})
set(MODE "3" CACHE STRING "")
endif(NOT DEFINED ${MODE})


if(CMAKE_CXX_COMPILER MATCHES "cl.exe")

### MSVC compiler
set(CMAKE_BUILD_TYPE Release)
add_definitions(/D CPP=${CPP} /D NUM=${NUM} /D MODE=${MODE})

else(CMAKE_CXX_COMPILER MATCHES "cl.exe")

### GCC, Clang and Intel compilers
add_definitions(-time -O3 -std=c++11 -ftemplate-depth-1000000 -DCPP=${CPP} -DNUM=${NUM} -DMODE=${MODE})

endif(CMAKE_CXX_COMPILER MATCHES "cl.exe")

target_link_libraries(metafactor stdc++)
target_link_libraries(metaprimes stdc++)
