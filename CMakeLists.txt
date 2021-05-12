project("goecharger")

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)

set(COMMON_SOURCES
    src/GoEChargerAPI.cpp
    src/GoEChargerDataMap.cpp
    src/HttpClient.cpp
    src/Json.cpp
    src/Url.cpp
    src/Test.cpp
)

add_library(${PROJECT_NAME} STATIC
    ${COMMON_SOURCES}
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    include
)
