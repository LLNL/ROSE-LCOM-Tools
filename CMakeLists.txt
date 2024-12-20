cmake_minimum_required(VERSION 3.14)
project(lcom-metric-analyzer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DROOT_DIR=\"${CMAKE_SOURCE_DIR}\")

find_package(Boost REQUIRED COMPONENTS
    date_time
    thread
    filesystem
    program_options
    regex
    system
    serialization
    wave
    iostreams
    chrono
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  # NOTE: ENV Not working here in Docker.
  $ENV{ROSE_HOME}/include/rose
  # You must manually specify the path for cmake to work in Docker.
  /rose/install_tree/include/rose
  $ENV{BOOST_HOME}/include/boost
)
link_libraries(
    rose
    Boost::date_time
    Boost::thread
    Boost::filesystem
    Boost::program_options
    Boost::regex
    Boost::system
    Boost::serialization
    Boost::wave
    Boost::iostreams
    Boost::chrono
    dl
    m
    quadmath
    asis_adapter
    stdc++fs
)
link_directories(
    $ENV{ROSE_HOME}/lib
    $ENV{BOOST_HOME}/lib
    $ENV{ASIS_ADAPTER}/lib
)
SET(CMAKE_INSTALL_RPATH "$ENV{BOOST_HOME}/lib;$ENV{ASIS_ADAPTER}/lib;$ENV{ROSE_HOME}/lib")


# lcom.out
add_executable(lcom.out src/lcom.cpp)
set_source_files_properties(src/lcom.cpp PROPERTIES COMPILE_OPTIONS "-O0;-ggdb;-march=native;-Wall;-Wextra;-Wno-misleading-indentation;-Wno-unused-parameter;-pthread")

# lcom-dot.out
add_executable(lcom-dot.out src/lcom-dot.cpp)
set_source_files_properties(src/lcom-dot.cpp PROPERTIES COMPILE_OPTIONS "-O0;-ggdb;-march=native;-Wall;-Wextra;-Wno-misleading-indentation;-Wno-unused-parameter;-pthread")

# lcom-unittest
# GoogleTest components
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
add_executable(lcom-unittest src/lcom-unittest.cpp)
set_source_files_properties(src/lcom-unittest.cpp PROPERTIES COMPILE_OPTIONS "-O0;-ggdb;-O0;-march=native;-Wall;-Wextra;-Wno-misleading-indentation;-Wno-unused-parameter;-pthread")
target_link_libraries(lcom-unittest GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(lcom-unittest)