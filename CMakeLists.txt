cmake_minimum_required(VERSION 3.5)

project(encoin LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRC_FILES CONFIGURE_DEPENDS
    ${PROJECT_NAME}/*.h
    ${PROJECT_NAME}/*.cpp
    ${PROJECT_NAME}/net/*.h
    ${PROJECT_NAME}/net/*.cpp
    ${PROJECT_NAME}/crypto/*.h
    ${PROJECT_NAME}/crypto/*.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})
include_directories(${PROJECT_NAME}/ ${Boost_INCLUDE_DIR})


find_package(Boost 1.40 COMPONENTS program_options REQUIRED )
find_package(OpenSSL REQUIRED)
find_package(SqliteOrm REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)
find_library(SECP256K1 secp256k1)

target_link_libraries(${PROJECT_NAME} sqlite_orm::sqlite_orm)
target_link_libraries(${PROJECT_NAME} OpenSSL::SSL ${SECP256K1})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

if (UNIX)
    include_directories("/usr/local/include/")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif (UNIX)
