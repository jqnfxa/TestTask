add_library(task1_lib INTERFACE)

target_include_directories(task1_lib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(task1_lib INTERFACE cxx_std_20)
