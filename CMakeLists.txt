cmake_minimum_required(VERSION 3.19)
project(Proyecto_Reconstruccion)

set(CMAKE_CXX_STANDARD 20)

add_executable(Proyecto_Reconstruccion main.cpp CImg.h CargadorImagenes.cpp CargadorImagenes.h Punto3D.cpp Punto3D.h)