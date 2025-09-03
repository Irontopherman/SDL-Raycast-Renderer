cmake_minimum_required(VERSION 3.16)
project(test)

# Set the architecture to 64-bit (x86_64)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
list(APPEND CMAKE_PREFIX_PATH "/home/christopher/SDL_image/cmake")
include_directories("/home/christopher/SDL-Addons/Image_Loader/gcc_compatible/img_loader" "/home/christopher/SDL-Addons/Image_Loader/gcc_compatible/img_loader")
#add_subdirectory("/home/christopher/SDL-Addons/Image_Loader/gcc_compatible/img_loader" EXCLUDE_FROM_ALL)
include_directories("/home/christopher/SDL_image/include" "/home/christopher/SDL_image/include")
set(CMAKE_PREFIX_PATH "/home/christopher/SDL_image/cmake" "/home/christopher/SDL_image/cmake")
find_package(SDL3 REQUIRED)
#find_package(SDL3_image REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)
add_library(imgloader STATIC main.c /home/christopher/SDL-Addons/Image_Loader/gcc_compatible/img_loader/img_loader.c)
add_executable(test main.c)
target_link_libraries(test PRIVATE imgloader)

target_link_libraries(test PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)

