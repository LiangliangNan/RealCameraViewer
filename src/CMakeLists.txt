cmake_minimum_required(VERSION 3.12)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)

set(PROJECT_NAME "RealCameraViewer")
project(${PROJECT_NAME})

# request Easy3D
find_package(Easy3D REQUIRED)

add_executable(${PROJECT_NAME}
        main.cpp
        viewer.h
        viewer.cpp
        read_calibrated_images.cpp
        )

target_link_libraries(${PROJECT_NAME} easy3d::viewer)