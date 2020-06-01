project(Journal)

cmake_minimum_required (VERSION 3.5)

add_definitions(-std=c++11)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(server_api_test src/test_server_api.cpp )
set(client_api_test src/test_client_api.cpp )
set(client_api_test_multi_user src/test_client_api_multi_user.cpp )

include_directories(./include)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 

include_directories(/usr/local/opt/openssl/include)
link_directories(/usr/local/opt/openssl/lib)

endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 

add_library(
    server_api
    src/server_api.cpp
    src/utils.cpp
    src/journal.cpp
    )
add_library(
    client_api
    src/client_api.cpp
    src/utils.cpp
    )

add_executable(server_api_test ${server_api_test})
add_executable(client_api_test ${client_api_test})
add_executable(client_api_test_multi_user ${client_api_test_multi_user})
set(client_api_test_multi_user src/test_client_api_multi_user.cpp )

target_link_libraries(server_api_test server_api)
target_link_libraries(client_api_test client_api)
target_link_libraries(client_api_test_multi_user client_api)

target_link_libraries(server_api_test z ssl uWS)
target_link_libraries(client_api_test z ssl uWS)
target_link_libraries(client_api_test_multi_user z ssl uWS)