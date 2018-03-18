cmake_minimum_required(VERSION 3.9)
project(testProject)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(CLUtils)
add_subdirectory(PTest)
add_subdirectory(tests)
add_executable(testProject main.cpp)

set(LIBRARIES
        LIB_PT
        LIB_CLUTILS
)

target_link_libraries(testProject
  ${LIBRARIES}
)

file(COPY kernels DESTINATION ./)