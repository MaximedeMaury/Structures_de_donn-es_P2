cmake_minimum_required(VERSION 3.24)
project(Structures_de_donn-es_P2 C)

set(CMAKE_C_STANDARD 11)

add_executable(Structures_de_donn-es_P2
        main.c
        utils.c
        list.c
        hasse.c
        markov.c
        tarjan.c
)

target_link_libraries(Structures_de_donn-es_P2 m)