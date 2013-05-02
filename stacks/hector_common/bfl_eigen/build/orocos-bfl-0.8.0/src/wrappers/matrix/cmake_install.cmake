# Install script for directory: /home/futuhal57/ros/stacks/hector_common/bfl_eigen/build/orocos-bfl-0.8.0/src/wrappers/matrix

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/home/futuhal57/ros/stacks/hector_common/bfl_eigen/install")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/bfl/wrappers/matrix" TYPE FILE FILES
    "/home/futuhal57/ros/stacks/hector_common/bfl_eigen/build/orocos-bfl-0.8.0/src/wrappers/matrix/matrix_wrapper.h"
    "/home/futuhal57/ros/stacks/hector_common/bfl_eigen/build/orocos-bfl-0.8.0/src/wrappers/matrix/vector_wrapper.h"
    "/home/futuhal57/ros/stacks/hector_common/bfl_eigen/build/orocos-bfl-0.8.0/src/wrappers/matrix/matrix_BOOST.h"
    "/home/futuhal57/ros/stacks/hector_common/bfl_eigen/build/orocos-bfl-0.8.0/src/wrappers/matrix/vector_BOOST.h"
    "/home/futuhal57/ros/stacks/hector_common/bfl_eigen/build/orocos-bfl-0.8.0/src/wrappers/matrix/matrix_NEWMAT.h"
    "/home/futuhal57/ros/stacks/hector_common/bfl_eigen/build/orocos-bfl-0.8.0/src/wrappers/matrix/vector_NEWMAT.h"
    "/home/futuhal57/ros/stacks/hector_common/bfl_eigen/build/orocos-bfl-0.8.0/src/wrappers/matrix/matrix_LTI.h"
    "/home/futuhal57/ros/stacks/hector_common/bfl_eigen/build/orocos-bfl-0.8.0/src/wrappers/matrix/vector_LTI.h"
    "/home/futuhal57/ros/stacks/hector_common/bfl_eigen/build/orocos-bfl-0.8.0/src/wrappers/matrix/matrix_EIGEN.h"
    "/home/futuhal57/ros/stacks/hector_common/bfl_eigen/build/orocos-bfl-0.8.0/src/wrappers/matrix/vector_EIGEN.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

