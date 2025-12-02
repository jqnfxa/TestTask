add_library(task3_lib STATIC
    src/geometry.cpp
    src/concave_hull.cpp
)

target_include_directories(task3_lib PUBLIC include)
find_package(Threads REQUIRED)
target_link_libraries(task3_lib PRIVATE Threads::Threads)

add_executable(task3_cli src/cli.cpp)
target_link_libraries(task3_cli PRIVATE task3_lib)

find_package(TBB QUIET)
if(TBB_FOUND)
    target_link_libraries(task3_lib PRIVATE TBB::tbb)
endif()

target_compile_features(task3_lib PRIVATE cxx_std_20)

# GUI
find_package(Qt5 COMPONENTS Core Gui Qml Quick REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(task3_gui
    src/main.cpp
    src/qt/hull_controller.cpp
    src/qt/resources.qrc
)

target_link_libraries(task3_gui PRIVATE
    task3_lib
    Qt5::Core
    Qt5::Gui
    Qt5::Qml
    Qt5::Quick
)

target_include_directories(task3_gui PRIVATE src/qt)
