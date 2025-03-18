cmake_minimum_required(VERSION 3.16.3)
project(mDNS)

set(SRC
    src/main.c
)

set(HDR
)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${SRC})