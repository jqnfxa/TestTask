add_library(task2_lib STATIC
    core/filter.cpp
)

target_include_directories(task2_lib PUBLIC include)
add_executable(task2_cli main.cpp)
target_link_libraries(task2_cli PRIVATE task2_lib)

target_compile_features(task2_cli PRIVATE cxx_std_20)
