CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
CMAKE_POLICY(SET CMP0003 NEW)

PROJECT(petithtmlescaper C)

SET(CMAKE_C_FLAGS "-O2 -msse2 -msse4.2 -Wall -fPIC ${CMAKE_C_FLAGS}")

ADD_LIBRARY(petithtmlescaper-static STATIC petithtmlescaper.c)
ADD_LIBRARY(petithtmlescaper SHARED petithtmlescaper.c)

SET_TARGET_PROPERTIES(petithtmlescaper-static PROPERTIES OUTPUT_NAME "petithtmlescaper")

INCLUDE_DIRECTORIES(deps/picotest)
ADD_EXECUTABLE(test.t test.c deps/picotest/picotest.c)
TARGET_LINK_LIBRARIES(test.t petithtmlescaper)
ADD_EXECUTABLE(bench.run bench.c)
TARGET_LINK_LIBRARIES(bench.run petithtmlescaper)

ADD_CUSTOM_TARGET(test ./test.t
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS petithtmlescaper test.t)

ADD_CUSTOM_TARGET(bench ./bench.run
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS petithtmlescaper bench.run)

INSTALL(TARGETS petithtmlescaper DESTINATION lib)
INSTALL(DIRECTORY . DESTINATION include FILES_MATCHING PATTERN "*.h")
