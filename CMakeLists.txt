cmake_minimum_required(VERSION 3.0)

#### PROJET ####

project(casse_brique
        VERSION 1.0
        DESCRIPTION "PROJET: Casse brique game"
        LANGUAGES CXX)



find_package(SDL2 REQUIRED)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
                PRIVATE
                #Le nom du projet
                src/${PROJECT_NAME}.cpp
                #SDL2
                src/utils/SDL.cpp
)

