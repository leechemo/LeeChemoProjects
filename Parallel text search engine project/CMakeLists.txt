cmake_minimum_required(VERSION 4.0)
project(Concurrency_project)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(include)

add_executable(Concurrency_project
        include/SearchConfig.hpp
        include/SearchResult.hpp
        include/ThreadSafeQueue.hpp
        include/WorkerPool.hpp
        include/FileScanner.hpp
        include/FileEnumerator.hpp
        include/OutputWriter.hpp
        include/Util.hpp
        src/Util.cpp
        src/FileEnumerator.cpp
        src/FileScanner.cpp
        src/WorkerPool.cpp
        src/OutputWriter.cpp
        src/main.cpp)
