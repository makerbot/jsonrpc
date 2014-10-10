message("Configureing jsonrpc")
project(jsonrpc)

add_compiler_flags()

add_api(JSONRPC)
add_import_api(JSON)

set_shared(true)
install_headers(src/main/include/)

include_directories(${INCLUDE_OUTPUT_PATH})
include_directories(src/main)
add_subdirectory(src/main/cpp)
