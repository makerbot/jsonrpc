# Copyright 2014 Makerbot Industries

# chosen arbitrarily
cmake_minimum_required(VERSION 2.8.9)

project(JsonRpc CXX)

set(${PROJECT_NAME}_VERSION 0.0.0)

# Common paths & utils
find_package(
  MakerBot
  CONFIG
  REQUIRED
  NO_CMAKE_BUILDS_PATH)

# Import json-cpp
find_package(
  JsonCpp ${${PROJECT_NAME}_VERSION}
  CONFIG
  REQUIRED
  NO_CMAKE_BUILDS_PATH)

include_directories(
  src/main/
  src/main/include)

add_library(
  jsonrpc
  SHARED

  src/main/cpp/jsonreader.cpp
  src/main/cpp/jsonrpc.cpp
  src/main/cpp/jsonrpccallback.cpp
  src/main/cpp/jsonrpcexception.cpp
  src/main/cpp/jsonrpcmethod.cpp
  src/main/cpp/jsonrpcprivate.cpp
  src/main/cpp/jsonrpcstream.cpp)

# Link in jsoncpp
target_link_libraries(jsonrpc JsonCpp::json)

mb_generate_config_files()
mb_set_api_visibility()

# Install The library, exporting the targets to the targets file
install(
  TARGETS jsonrpc
  EXPORT ${TARGETS_FILE}
  RUNTIME DESTINATION ${BIN_INSTALL_DESTINATION}
  LIBRARY DESTINATION ${LIB_INSTALL_DESTINATION}
  ARCHIVE DESTINATION ${LIB_INSTALL_DESTINATION}
)

# Install the target file for import
install(
  EXPORT ${TARGETS_FILE}
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CONFIG_INSTALL_DESTINATION}
)

# Install the include files
install(
  DIRECTORY src/main/include
  DESTINATION ${INCLUDE_INSTALL_DESTINATION}
)
