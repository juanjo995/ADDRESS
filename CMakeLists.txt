cmake_minimum_required(VERSION 3.5)
project(ADDRESS)

set(CMAKE_CXX_STANDARD 17)

# Realease
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(include)

# Sources
set(SOURCES
        src/main.cpp
        src/ShaderProgram.cpp
        src/Renderer.cpp
        src/VAO.cpp
        src/Camera.cpp
        src/Star.cpp
        src/Star.h
        src/Grid.cpp
        src/Grid.h
        src/Axis.cpp
        src/Axis.h
        src/GUI.cpp
        src/GUI.h
        src/Utils.h
        src/Utils.cpp
        src/StarCatalog.cpp
        src/StarCatalog.h
        src/Parameters.h
        src/FileDialog.h
        src/FileDialog.cpp)

set(LINK_LIBRARIES glad glfw imgui CCfits cfitsio nfd)
add_subdirectory(include/imgui)
add_subdirectory(include/glad)
add_subdirectory(glfw-3.3.2)
add_subdirectory(include/nativefiledialog-extended)

#Path to CCfits
include_directories(/usr/local/include/CCfits)
include_directories(/usr/local/include)

#Paths to CCfits and cfitsio on Windows
include_directories("C:/Program Files (x86)/CCfits/include")
include_directories("C:/Program Files (x86)/CCfits/include/CCfits")
include_directories("C:/Program Files (x86)/CFITSIO/include")
link_directories("C:/Program Files (x86)/CCfits/lib")
link_directories("C:/Program Files (x86)/CFITSIO/lib")

#For CCfits on macOS
link_directories(/usr/local/lib)

#For glad library
link_directories(./lib)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_executable(ADDRESS ${SOURCES} ${OpenDialog})

# Link libraries
target_link_libraries(ADDRESS ${LINK_LIBRARIES})
