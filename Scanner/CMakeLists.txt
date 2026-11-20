cmake_minimum_required(VERSION 3.20)
project(Scanner)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/main.cpp)
file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/src/*.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

set_target_properties(Scanner PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

if(MINGW)
    target_link_libraries(${PROJECT_NAME} PRIVATE kernel32)
endif()