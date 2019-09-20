cmake_minimum_required(VERSION 3.14)
project(Client C)

set(CMAKE_C_STANDARD 99)

add_executable(Server
        Servidor.c
        Socket.c
        Socket.h
        Socket_Cliente.c
        Socket_Cliente.h Socket_Servidor.c Socket_Servidor.h )

add_executable(Cliente
        Cliente.c
        Socket.c
        Socket.h
        Socket_Cliente.c
        Socket_Cliente.h Socket_Servidor.c Socket_Servidor.h )