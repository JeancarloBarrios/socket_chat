cmake_minimum_required(VERSION 3.12)
project(socket_chat C)

set(CMAKE_C_STANDARD 11)
find_package (PkgConfig REQUIRED)

pkg_check_modules (JSONC REQUIRED json-c)
include_directories (
        ${JSONC_INCLUDE_DIRS}
)



#add_executable(socket_chat main.c server.c utils.c utils.h client.c)
add_executable(server server.c utils.c utils.h server_@.c poll.c)
#add_executable(client client.c utils.c utils.h)
target_link_libraries (server ${JSONC_LIBRARIES})
#target_link_libraries (client ${JSONC_LIBRARIES})