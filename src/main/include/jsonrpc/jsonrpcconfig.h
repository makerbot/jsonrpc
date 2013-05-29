#ifndef JSONRPC_CONFIG_H_
#define JSONRPC_CONFIG_H_

# if defined(_MSC_VER)
#  if defined(JSONRPC_DLL_BUILD)
#   define JSONRPC_API __declspec(dllexport)
#  elif defined(JSONRPC_DLL)
#   define JSONRPC_API __declspec(dllimport)
#  endif  // THING_DLL_BUILD / THING_DLL
# endif  // if defined(_MSC_VER)
# if !defined(JSONRPC_API)
#  define JSONRPC_API
# endif

#endif  // INCLUDE_THING_CONFIG_H_