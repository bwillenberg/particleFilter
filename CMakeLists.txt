cmake_minimum_required (VERSION 2.6)
project (ParticleFilter)

# make the project c++11 ready
set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")

set(SOURCES "main.cxx" "particle.cxx" "filter.cxx" "ioInterfaces.cxx")
set(HEADERS "particle.h" "filter.h" "ioInterfaces.h")

source_group("Source Files" FILES ${SOURCES})
source_group("Header Files" FILES ${HEADERS})

add_executable(ParticleFilter ${SOURCES} ${HEADERS})
