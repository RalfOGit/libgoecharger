project("goecharger")

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)

#
# configuration for libgoecharger
#
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
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    LIB_NAMESPACE=libgoecharger
)

#
# configuration for test application
#
set(TEST_SOURCES
    src/Test.cpp
)
add_executable(${PROJECT_NAME}_test ${TEST_SOURCES})
add_dependencies(${PROJECT_NAME}_test ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}_test PUBLIC ${PROJECT_NAME})

if (MSVC)
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME} ws2_32.lib Iphlpapi.lib)
else()
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME})
endif()



set_target_properties(${PROJECT_NAME}
    PROPERTIES OUTPUT_NAME ${PROJECT_NAME}
)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
