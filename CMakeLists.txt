cmake_minimum_required(VERSION 3.0)
project(assetc)

set(CMAKE_C_STANDARD 11)

# set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -march=native)  # march=native for best performance
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-long-long -pedantic -Wno-unused-function -Wno-missing-braces")


add_executable(assetc
        src/main.c
        src/generate.c
        src/file.c
        src/load.c
        src/apply.c
        src/asset.c
        )


add_executable(example
        example/main.c
        example/asset.c
        )


#
# install:
#
install(TARGETS assetc
        RUNTIME DESTINATION bin
        )