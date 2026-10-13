# Create a library
add_library(sockets STATIC tcp_socket.c tcp_server.c tcp_client.c)

# Server and client executables
add_executable(server server.c)
target_link_libraries(server PRIVATE sockets)

add_executable(client client.c)
target_link_libraries(client PRIVATE sockets)
