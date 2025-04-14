cmake_minimum_required(VERSION 3.10)
project(OpenCV_Mingw_Example)

set(CMAKE_CXX_STANDARD 17)

# Path to OpenCVConfig.cmake
set(OpenCV_DIR "C:/OpenCV_Mingw/install")  # Path where OpenCVConfig.cmake is located

# Specify include directory for OpenCV headers
include_directories("C:/OpenCV_Mingw/include")

# Specify library directory for OpenCV libraries
link_directories("C:/OpenCV_Mingw/install/x64/mingw/lib")  # Adjust path based on actual installation

set(IMGUI_DIR "D:/Coding/Node Assignment/libs/imgui")  # Adjust to your ImGui directory

# Add ImGui source files
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
)

# Specify include directory for ImGui headers
include_directories(${IMGUI_DIR})


add_executable(main
    src/main.cpp

    # Graph
    src/graph/NodeGraph.cpp

    # Nodes
    src/nodes/ImageInputNode.cpp
    src/nodes/OutputNode.cpp
    # Add more nodes here as needed:
    src/nodes/BrightnessContrastNode.cpp
    src/nodes/ColorChannelSplitterNode.cpp
    ${IMGUI_SOURCES}  # Link ImGui source files
)


find_package(OpenCV REQUIRED)


# Link OpenCV libraries
target_link_libraries(main ${OpenCV_LIBS})
