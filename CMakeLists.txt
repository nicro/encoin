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
find_package(SqliteOrm REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)
find_library(SECP256K1 secp256k1 PATHS "/usr/lib" "/usr/local/lib" "./secp256k1/.libs")

target_link_libraries(encoin sqlite_orm::sqlite_orm)
target_link_libraries(encoin OpenSSL::SSL ${SECP256K1})
include_directories(
    encoin
    "include/"
    "lib/"
    "/usr/local/include/"
)
