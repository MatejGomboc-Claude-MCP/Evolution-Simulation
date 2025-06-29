cmake_minimum_required(VERSION 3.0)

project(common LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(common STATIC
    utils.cpp
    utils.h
    program.cpp
    program.h
    txtfileparser.cpp
    txtfileparser.h
    instruction.cpp
    instruction.h
    biological_sim.cpp
    biological_sim.h
    instructions/multiply.cpp
    instructions/multiply.h
    instructions/add.cpp
    instructions/add.h
    instructions/subtract.cpp
    instructions/subtract.h
    instructions/divide.cpp
    instructions/divide.h
    instructions/copy.cpp
    instructions/copy.h
    instructions/nop.cpp
    instructions/nop.h
    instructions/init.cpp
    instructions/init.h
    instructions/increase.cpp
    instructions/increase.h
    instructions/decrease.cpp
    instructions/decrease.h
    instructions/negate.cpp
    instructions/negate.h
    instructions/equal.cpp
    instructions/equal.h
    instructions/not.cpp
    instructions/not.h
    instructions/greater.cpp
    instructions/greater.h
    instructions/smaller.cpp
    instructions/smaller.h
    instructions/and.cpp
    instructions/and.h
    instructions/or_.cpp
    instructions/or_.h
    instructions/loop.cpp
    instructions/loop.h
    instructions/condition.cpp
    instructions/condition.h
    instructions/return.cpp
    instructions/return.h
    instructions/set.cpp
    instructions/set.h
    instructions/clear.cpp
    instructions/clear.h
    instructions/invert.cpp
    instructions/invert.h
    instructions/mate_request.cpp
    instructions/mate_request.h
    instructions/consume_food.cpp
    instructions/consume_food.h
    instructions/mate_accept.cpp
    instructions/mate_accept.h
)

target_include_directories(common PRIVATE
    instructions
)

include(TestBigEndian)

TEST_BIG_ENDIAN(IS_BIG_ENDIAN)

if(IS_BIG_ENDIAN)
    message(FATAL_ERROR "Only little endian architectures are supported!")
endif()
