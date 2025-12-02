add_executable(task1_benchmark main.cpp)
add_executable(task2_data_generator gen_data.cpp)

target_compile_features(task1_benchmark PRIVATE cxx_std_20)
find_package(Threads REQUIRED)
target_link_libraries(task1_benchmark PRIVATE Threads::Threads)
target_include_directories(task1_benchmark PRIVATE ${CMAKE_SOURCE_DIR}/task1/include)

if(MSVC)
    target_compile_options(task1_benchmark PRIVATE /O2)
else()
    target_compile_options(task1_benchmark PRIVATE -O3 -march=native)
endif()
