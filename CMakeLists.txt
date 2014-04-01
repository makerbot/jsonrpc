# Copyright 2014 Makerbot Industries

# chosen arbitrarily
cmake_minimum_required(VERSION 2.8.9)

project(json-cpp CXX)

set(JSONRPC_VERSION 0.0.0)

# Install locations
set(CONFIG_INSTALL_DESTINATION cmake)
set(LIB_INSTALL_DESTINATION lib)
set(BIN_INSTALL_DESTINATION bin)
set(INCLUDE_INSTALL_DESTINATION include)

# Import json-cpp
find_package(
  JsonCpp
  CONFIG
  REQUIRED
  VERSION ${JSONRPC_VERSION}
  NO_CMAKE_BUILDS_PATH)

include_directories(
  src/main/
  src/main/include)

# This is the macro used in headers to indicate export of a function/class
set(JSONRPC_API JSONRPC_API)

if(WIN32)
  add_definitions("-D${JSONRPC_API}=__declspec(dllexport)")
else()
  add_definitions(
    "-fvisibility=hidden"
    "-D${JSONRPC_API}=__attribute__ ((visibility (\"default\")))")
endif()

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
target_link_libraries(jsonrpc JSONCPP::json)

# for configure_package_config_file() and write_basic_package_version_file()
include(CMakePackageConfigHelpers)

# Names of files for exporting
set(JSONRPC_CONFIG "${CMAKE_CURRENT_BINARY_DIR}/JsonRpcConfig.cmake")
set(JSONRPC_CONFIG_TEMPLATE "./JsonRpcConfig.cmake.in")
set(JSONRPC_CONFIG_VERSION "${CMAKE_CURRENT_BINARY_DIR}/JsonRpcConfigVersion.cmake")
set(JSONRPC_TARGETS_FILE "JsonRpcTargets")

# Create the config file from the config template
configure_package_config_file(
  "${JSONRPC_CONFIG_TEMPLATE}"
  "${JSONRPC_CONFIG}"
  INSTALL_DESTINATION ${CONFIG_INSTALL_DESTINATION}
  PATH_VARS
    CONFIG_INSTALL_DESTINATION
    INCLUDE_INSTALL_DESTINATION
    JSONRPC_TARGETS_FILE
)

# Create the version file
write_basic_package_version_file(
  "${JSONRPC_CONFIG_VERSION}"
  VERSION ${JSONRPC_VERSION}
  COMPATIBILITY SameMajorVersion
)

# Install The library, exporting the targets to the targets file
install(
  TARGETS jsonrpc
  EXPORT ${JSONRPC_TARGETS_FILE}
  RUNTIME DESTINATION ${BIN_INSTALL_DESTINATION}
  LIBRARY DESTINATION ${LIB_INSTALL_DESTINATION}
  ARCHIVE DESTINATION ${LIB_INSTALL_DESTINATION}
)

# Install the config & target files for import
install(
  FILES "${JSONRPC_CONFIG}" "${JSONRPC_CONFIG_VERSION}"
  DESTINATION ${CONFIG_INSTALL_DESTINATION}
)

install(
  EXPORT ${JSONRPC_TARGETS_FILE}
  NAMESPACE JSONRPC::
  DESTINATION ${CONFIG_INSTALL_DESTINATION}
)

# Install the include files
install(
  DIRECTORY src/main/include
  DESTINATION ${INCLUDE_INSTALL_DESTINATION}
)
