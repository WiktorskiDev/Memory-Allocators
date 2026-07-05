cmake_minimum_required(VERSION 3.20)

project(
    MemoryAlloc
    VERSION 0.1.0
    DESCRIPTION "A C++ memory allocator project"
    LANGUAGES CXX
)

include(FetchContent)

FetchContent_Declare(
    googleBench
    URL https://github.com/google/benchmark/archive/refs/tags/v1.9.5.zip
)

set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    googleTest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

FetchContent_MakeAvailable(googleTest)
FetchContent_MakeAvailable(googleBench)

add_executable(memory_manager
    src/main.cpp
    src/MemoryController.cpp

    include/Arena.h
    include/ArenaFast.h
    include/Slots.h
    include/Pool.h
)

add_executable(memory_manager_benchmark
    src/allocator_benchmark.cpp
    include/BenchStructs.h
    include/Slots.h
    include/ArenaFast.h
    include/Pool.h
)

add_executable(memory_manager_test
    src/allocator_test.cpp
    include/BenchStructs.h
    include/Slots.h
    include/ArenaFast.h
    include/Pool.h
)

target_link_libraries(memory_manager_benchmark benchmark::benchmark)

target_link_libraries(memory_manager_test GTest::gtest_main)

target_include_directories(memory_manager
    PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
)

target_include_directories(memory_manager_benchmark
    PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
)


target_include_directories(memory_manager_test
    PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
)
    

target_compile_features(memory_manager PRIVATE cxx_std_17)
target_compile_features(memory_manager_benchmark PRIVATE cxx_std_17)
target_compile_features(memory_manager_test PRIVATE cxx_std_17)

set_target_properties(memory_manager PROPERTIES
    CXX_EXTENSIONS OFF
)

set_target_properties(memory_manager_benchmark PROPERTIES
    CXX_EXTENSIONS OFF
)

set_target_properties(memory_manager_test PROPERTIES
    CXX_EXTENSIONS OFF
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(memory_manager PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
        target_compile_options(memory_manager_benchmark PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
        target_compile_options(memory_manager_test PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()