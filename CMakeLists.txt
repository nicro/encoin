cmake_minimum_required(VERSION 3.5)

project(encoin LANGUAGES CXX)

if (APPLE)
    # fix for brew openssl package
    # you might need to set your own path
    set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl/")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")

add_executable(encoin ${SRC_FILES})
find_package(OpenSSL REQUIRED)
target_link_libraries(encoin OpenSSL::SSL)
include_directories(encoin "include/")
