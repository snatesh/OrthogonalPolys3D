CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(orthoPoly)

MESSAGE(STATUS "ACCESSING TEST DIRECTORY: ${PROJECT_SOURCE_DIR}")

add_test(NAME orthoPolyTest COMMAND runUnitTests ${CMAKE_CURRENT_LIST_DIR}/F.txt)
