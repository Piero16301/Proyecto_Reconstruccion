cmake_minimum_required(VERSION 3.19)
project(Proyecto_Reconstruccion)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

add_executable(Proyecto_Reconstruccion main.cpp Procesador.h Procesador.cpp Punto3D.h Punto3D.cpp
        Interfaz.h Interfaz.cpp CImg.h)

target_link_libraries(Proyecto_Reconstruccion -lGL -lGLU -lglut -lX11 -lpthread -fopenmp)