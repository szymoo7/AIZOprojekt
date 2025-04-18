cmake_minimum_required(VERSION 3.26)
project(AIZOprojekt)

set(CMAKE_CXX_STANDARD 17)

add_executable(AIZOprojekt main.cpp
        Timer.cpp
        Timer.h
        ArgumentParser.cpp
        ArgumentParser.h
        Sorter.cpp
        Sorter.h
        DynamicArray.cpp
        DynamicArray.h
        Helper.cpp
        Helper.h
)
