# Install script for directory: /home/futuhal57/ros/stacks/hector_common/bfl_boost/build/orocos-bfl-0.8.0/examples

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

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/home/futuhal57/ros/stacks/hector_common/bfl_boost/build/orocos-bfl-0.8.0/examples/compare_filters/cmake_install.cmake")
  INCLUDE("/home/futuhal57/ros/stacks/hector_common/bfl_boost/build/orocos-bfl-0.8.0/examples/linear_kalman/cmake_install.cmake")
  INCLUDE("/home/futuhal57/ros/stacks/hector_common/bfl_boost/build/orocos-bfl-0.8.0/examples/nonlinear_kalman/cmake_install.cmake")
  INCLUDE("/home/futuhal57/ros/stacks/hector_common/bfl_boost/build/orocos-bfl-0.8.0/examples/nonlinear_particle/cmake_install.cmake")
  INCLUDE("/home/futuhal57/ros/stacks/hector_common/bfl_boost/build/orocos-bfl-0.8.0/examples/smoother/cmake_install.cmake")
  INCLUDE("/home/futuhal57/ros/stacks/hector_common/bfl_boost/build/orocos-bfl-0.8.0/examples/discrete_filter/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

