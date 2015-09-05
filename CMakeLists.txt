CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
CMAKE_POLICY(SET CMP0003 NEW)

PROJECT(petithttpescaper C)

SET(CMAKE_C_FLAGS "-O2 -msse2 -msse4.2 -Wall -fPIC -Wno-unused-function ${CMAKE_C_FLAGS}")

ADD_LIBRARY(petithttpescaper STATIC petithttpescaper.c)

INCLUDE_DIRECTORIES(deps/picotest)
ADD_EXECUTABLE(test.t test.c deps/picotest/picotest.c)
TARGET_LINK_LIBRARIES(test.t petithttpescaper)

ADD_CUSTOM_TARGET(test ./test.t
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS petithttpescaper test.t)

INSTALL(TARGETS petithttpescaper DESTINATION lib)
INSTALL(DIRECTORY . DESTINATION include FILES_MATCHING PATTERN "*.h")
