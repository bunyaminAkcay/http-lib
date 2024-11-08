add_library(http-lib
    request.cc
    requestHandler.cc
    response.cc
    server.cc
    httpServer.cc
)

target_include_directories(http-lib PUBLIC ${CMAKE_SOURCE_DIR}/include)