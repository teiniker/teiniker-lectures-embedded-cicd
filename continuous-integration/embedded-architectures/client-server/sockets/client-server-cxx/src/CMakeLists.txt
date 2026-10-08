# Create a library 
add_library(sockets STATIC tcp_socket.cpp tcp_server.cpp tcp_client.cpp) 

# Server and client executables
add_executable(server server.cpp)
target_link_libraries(server PRIVATE sockets)

add_executable(client client.cpp)
target_link_libraries(client PRIVATE sockets)
