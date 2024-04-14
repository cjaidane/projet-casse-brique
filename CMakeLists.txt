cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
)