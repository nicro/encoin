cmake_minimum_required(VERSION 3.5)

project(encoin LANGUAGES CXX)

set(OPENSSL_ROOT_DIR "/opt/homebrew/opt/openssl@3/")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_FILES "encoin/*.cpp")

add_executable(encoin ${SRC_FILES})
find_package(fmt)
find_package(OpenSSL REQUIRED)
find_package(SqliteOrm REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)
find_library(SECP256K1 secp256k1 PATHS "/usr/lib" "/usr/local/lib" "./secp256k1/.libs")

target_link_libraries(encoin sqlite_orm::sqlite_orm)
target_link_libraries(encoin OpenSSL::SSL ${SECP256K1})
target_link_libraries(encoin fmt::fmt)

include_directories(
    encoin
    "encoin/"
    "lib/"
    "/usr/local/include/"
    "/opt/homebrew/Cellar/boost/1.76.0/include"
)
