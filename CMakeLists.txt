cmake_minimum_required(VERSION 2.6)

# Tested with compilers
set(CMAKE_CXX_COMPILER g++)
#set(CMAKE_CXX_COMPILER clang)
#set(CMAKE_CXX_COMPILER icpc)
#set(CMAKE_CXX_COMPILER cl.exe)

set(CMAKE_BUILD_TYPE Release)


add_executable(metafactor metafactor.cpp)
add_executable(metaprimes metaprimes.cpp)

# Set default values 
if(NOT DEFINED ${CPP})
set(CPP "11" CACHE STRING "")
endif(NOT DEFINED ${CPP})

if(NOT DEFINED ${MODE})
set(MODE "3" CACHE STRING "")
endif(NOT DEFINED ${MODE})

if(NOT DEFINED ${NUM})
set(NUM "0" CACHE STRING "")
endif(NOT DEFINED ${NUM})

if(NOT DEFINED ${LIMIT})
set(NUM "100" CACHE STRING "")
endif(NOT DEFINED ${LIMIT})


if(CMAKE_CXX_COMPILER MATCHES "cl.exe")

### MSVC compiler
add_definitions(/D CPP=${CPP} /D NUM=${NUM} /D MODE=${MODE} /D LIMIT=${LIMIT})


else(CMAKE_CXX_COMPILER MATCHES "cl.exe")

### GCC, Clang and Intel compilers
add_definitions(-std=c++14 -O3 -Wall -ftemplate-depth-1000000 -DCPP=${CPP} -DNUM=${NUM} -DMODE=${MODE} -DLIMIT=${LIMIT})

target_link_libraries(metafactor stdc++)
target_link_libraries(metaprimes stdc++)

endif(CMAKE_CXX_COMPILER MATCHES "cl.exe")

