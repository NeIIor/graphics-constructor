cmake_minimum_required(VERSION 3.15)

project(QuantumNexus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

set(SOURCE_FILES
    main.cpp
    core/CosmicEvents.cpp
    interface/NexusPanel.cpp
    interface/CameraControlsPanel.cpp
    interface/ObjectListPanel.cpp 
    rendering/PhotonTracer.cpp
    rendering/CosmicView.cpp
    system/RenderEngine.cpp
)

add_executable(QuantumNexus ${SOURCE_FILES})

target_link_libraries(QuantumNexus sfml-graphics sfml-window sfml-system)

target_include_directories(QuantumNexus PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(QuantumNexus PRIVATE DEBUG)
    target_compile_options(QuantumNexus PRIVATE -g -O3)
else()
    target_compile_options(QuantumNexus PRIVATE -O2)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/arialmt.ttf")
    add_custom_command(TARGET QuantumNexus POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/arialmt.ttf"
        $<TARGET_FILE_DIR:QuantumNexus>/arialmt.ttf
    )
>>>>>>> c7acdca (simple gui and optor)
endif()