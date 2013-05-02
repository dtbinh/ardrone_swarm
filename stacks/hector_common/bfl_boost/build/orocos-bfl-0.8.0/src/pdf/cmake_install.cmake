# Install script for directory: /home/futuhal57/ros/stacks/hector_common/bfl_boost/build/orocos-bfl-0.8.0/src/pdf

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/home/futuhal57/ros/stacks/hector_common/bfl_boost/install")
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
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/bfl/pdf" TYPE FILE FILES
    "/home/futuhal57/ros/stacks/hector_common/bfl_boost/build/orocos-bfl-0.8.0/src/pdf/pdf.h"
    "/home/futuhal57/ros/stacks/hector_common/bfl_boost/build/orocos-bfl-0.8.0/src/pdf/conditionalpdf.h"
    "/home/futuhal57/ros/stacks/hector_common/bfl_boost/build/orocos-bfl-0.8.0/src/pdf/discretepdf.h"
    "/home/futuhal57/ros/stacks/hector_common/bfl_boost/build/orocos-bfl-0.8.0/src/pdf/discreteconditionalpdf.h"
    "/home/futuhal57/ros/stacks/hector_common/bfl_boost/build/orocos-bfl-0.8.0/src/pdf/mcpdf.h"
    "/home/futuhal57/ros/stacks/hector_common/bfl_boost/build/orocos-bfl-0.8.0/src/pdf/mcpdf.cpp"
    "/home/futuhal57/ros/stacks/hector_common/bfl_boost/build/orocos-bfl-0.8.0/src/pdf/gaussian.h"
    "/home/futuhal57/ros/stacks/hector_common/bfl_boost/build/orocos-bfl-0.8.0/src/pdf/uniform.h"
    "/home/futuhal57/ros/stacks/hector_common/bfl_boost/build/orocos-bfl-0.8.0/src/pdf/conditionalgaussian.h"
    "/home/futuhal57/ros/stacks/hector_common/bfl_boost/build/orocos-bfl-0.8.0/src/pdf/conditionalgaussian_additivenoise.h"
    "/home/futuhal57/ros/stacks/hector_common/bfl_boost/build/orocos-bfl-0.8.0/src/pdf/analyticconditionalgaussian.h"
    "/home/futuhal57/ros/stacks/hector_common/bfl_boost/build/orocos-bfl-0.8.0/src/pdf/analyticconditionalgaussian_additivenoise.h"
    "/home/futuhal57/ros/stacks/hector_common/bfl_boost/build/orocos-bfl-0.8.0/src/pdf/linearanalyticconditionalgaussian.h"
    "/home/futuhal57/ros/stacks/hector_common/bfl_boost/build/orocos-bfl-0.8.0/src/pdf/filterproposaldensity.h"
    "/home/futuhal57/ros/stacks/hector_common/bfl_boost/build/orocos-bfl-0.8.0/src/pdf/EKF_proposaldensity.h"
    "/home/futuhal57/ros/stacks/hector_common/bfl_boost/build/orocos-bfl-0.8.0/src/pdf/mixture.h"
    "/home/futuhal57/ros/stacks/hector_common/bfl_boost/build/orocos-bfl-0.8.0/src/pdf/mixture.cpp"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

