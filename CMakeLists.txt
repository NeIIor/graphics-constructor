cmake_minimum_required(VERSION 3.15)
project(RayTracing3D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(RayTracing3D
    main.cpp
    
    core/AppState.cpp
    core/Event.cpp
    
    ui/Widget.cpp
    ui/Window.cpp
    ui/Button.cpp
    ui/ControlPanel.cpp
    ui/InfoPanel.cpp
    ui/CameraController.cpp
    
    ray_tracing/Sphere.cpp
    ray_tracing/Light.cpp
    ray_tracing/RayTracer.cpp
    
    graphics/SFMLSystem.cpp
    graphics/RayTracingView.cpp
)

target_link_libraries(RayTracing3D sfml-graphics sfml-window sfml-system)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(RayTracing3D PRIVATE DEBUG)
    target_compile_options(RayTracing3D PRIVATE -g -O0)
else()
    target_compile_options(RayTracing3D PRIVATE -O2)
endif()