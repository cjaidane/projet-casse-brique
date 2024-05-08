cmake_minimum_required(VERSION 3.0) #version minimale de cmake
set(CMAKE_CXX_STANDARD 11) #version du c++
set(CMAKE_CXX_STANDARD_REQUIRED ON) #le c++ est obligatoire

#### PROJET ####

project(casse_brique
        VERSION 1.0
        DESCRIPTION "PROJET: Casse brique game"
        LANGUAGES CXX) #langage du projet


find_package(SDL2 REQUIRED) #pour trouver SDL2 et required inique que c'est essentiel
find_package(PkgConfig REQUIRED) #pour chercher SDL2_ttf
pkg_search_module(SDL2TTF REQUIRED SDL2_ttf) #pour trouver SDL2_ttf

link_directories(${SDL2_LIBRARY_DIRS}) #pour ajouter les librairies de SDL2

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
                PRIVATE
                #Le nom du projet
                src/main.cpp
                #SDL2
                src/game.cpp
                src/window.cpp
                src/paddle.cpp
                src/ball.cpp
                src/brick.cpp
)

target_include_directories(${PROJECT_NAME}
                                PRIVATE
                                ${SDL2_INCLUDE_DIRS}
                                ${SDL2TTF_INCLUDE_DIRS}
                               
                                include
                                include/game
                                include/window
                                include/paddle
                                include/ball
                                include/brick
                               
                
)

target_link_libraries(${PROJECT_NAME}
                        PRIVATE
                        ${SDL2_LIBRARIES}
                        ${SDL2TTF_LIBRARIES}
                        #SDL2_ttf
)

