
cmake_minimum_required(VERSION 3.12)
project(TelAzCpp)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_BUILD_TYPE "Release")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ggdb -DNDEBUG")


include_directories(/Users/mac/Software/boost_1_68_0)
include_directories(${PROJECT_SOURCE_DIR}/include)
add_subdirectory (src)


## DEBUG/RELEASE LINES
#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
#add_executable(run ${PROJECT_SOURCE_DIR}/src/main.cpp)
#target_link_libraries (run SOURCE_FILES)


## TESTING LINES
add_subdirectory (test/src)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/test/bin/${CMAKE_BUILD_TYPE})
include_directories(${PROJECT_SOURCE_DIR}/test/include)
add_executable(runTest ${PROJECT_SOURCE_DIR}/test/src/testMain.cpp)
target_link_libraries (runTest SOURCE_FILES TEST_SOURCE_FILES)

#install(TARGETS TelAzCpp DESTINATION bin)
