project("goecharger")

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)

set(COMMON_SOURCES
    src/GoEChargerAPI.cpp
    src/GoEChargerDataMap.cpp
    src/HttpClient.cpp
    src/Json.cpp
    src/Url.cpp
)
add_library(${PROJECT_NAME} STATIC
    ${COMMON_SOURCES}
)
target_include_directories(${PROJECT_NAME}
PUBLIC
    include
)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    LIB_NAMESPACE=libgoecharger
)


set(TEST_SOURCES
    src/Test.cpp
)
set(TEST_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
include_directories(${TEST_INCLUDE_DIR})
add_executable(${PROJECT_NAME}_test ${TEST_SOURCES})
add_dependencies(${PROJECT_NAME}_test ${PROJECT_NAME})

if (MSVC)
target_link_libraries(${PROJECT_NAME}_test goecharger.lib ws2_32.lib Iphlpapi.lib)
else()
target_link_libraries(${PROJECT_NAME}_test ${CMAKE_SOURCE_DIR}/build/libgoecharger.a)
endif()

set_target_properties(${PROJECT_NAME}
    PROPERTIES OUTPUT_NAME ${PROJECT_NAME}
)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
