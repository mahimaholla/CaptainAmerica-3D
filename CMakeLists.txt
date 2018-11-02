cmake_minimum_required(VERSION 3.12)
project(ComputacaoGraficaAJB C)

set(CMAKE_C_STANDARD 99)

add_executable(ComputacaoGraficaAJB capitao.c)
target_link_libraries(ComputacaoGraficaAJB -lGL -lglut -lGLU -lm)