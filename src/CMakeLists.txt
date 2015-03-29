add_library (dispatch
             Message.cpp
             Server.cpp
             Client.cpp
             Mailer.cpp
             )

include_directories (${dispatch_SOURCE_DIR}/../leplib/src)

if(CMAKE_HOST_WIN32)
    target_link_libraries(dispatch ${target_link_libraries} lep ws2_32)
endif(CMAKE_HOST_WIN32)

if(CMAKE_HOST_UNIX)
    target_link_libraries(dispatch ${target_link_libraries} pthread)
endif(CMAKE_HOST_UNIX)
