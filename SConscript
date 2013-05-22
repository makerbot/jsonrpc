# vim:ai:et:ff=unix:fileencoding=utf-8:sw=4:syntax=python:ts=4:

import os
import sys

env = Environment(ENV=os.environ)

Import('debug')

includes = [
    Dir('#/src/main/include/jsonrpc/')]
sources = [
    Glob('src/main/cpp/*.cpp')]

libjsonrpcenv = env.Clone()
libjsonrpcenv.Tool('mb_install', toolpath=[Dir('submodules/mw-scons-tools')])

# Compiler flags
libjsonrpcenv.Append(CCFLAGS='-Wall')

if libjsonrpcenv.MBIsWindows():
    if debug:
        libjsonrpcenv.Append(CCFLAGS='-Zi')
        libjsonrpcenv.Append(CCFLAGS='-Od')
    else:
        libjsonrpcenv.Append(CCFLAGS='-O2')
    # assignment operator could not be generated
    libjsonrpcenv.Append(CCFLAGS='/wd4512')
    # unreferenced inline function has been removed
    libjsonrpcenv.Append(CCFLAGS='/wd4514')
    # function not inlined
    libjsonrpcenv.Append(CCFLAGS='/wd4710')
    # 'bytes' bytes padding added after member 'member'
    libjsonrpcenv.Append(CCFLAGS='/wd4820')
    # exception specification
    libjsonrpcenv.Append(CCFLAGS='/wd4986')
    # Turn on proper exception handling. Come on, msvc, really?
    libjsonrpcenv.Append(CCFLAGS='/EHsc')
    # Use the json-cpp dll delcarations
    libjsonrpcenv.Append(CCFLAGS='/DJSON_DLL')

else:
    if debug:
        libjsonrpcenv.Append(CCFLAGS='-g')
    else:
        libjsonrpcenv.Append(CCFLAGS='-O2')

    libjsonrpcenv.Append(CCFLAGS='-pedantic')
    libjsonrpcenv.Append(CCFLAGS='-Wextra')
    libjsonrpcenv.Append(CCFLAGS='-Wno-variadic-macros')
    libjsonrpcenv.Append(CCFLAGS='-Wno-long-long')
    libjsonrpcenv.Append(CCFLAGS='-Werror')

libjsonrpcenv.Append(CPPPATH=[Dir('src/main/include/')])

libjsonrpcenv.MBAddDevelLibPath('#/../json-cpp/obj')
libjsonrpcenv.MBAddDevelIncludePath('#/../json-cpp/include')

libjsonrpcenv.MBAddLib('jsoncpp')

libjsonrpcenv.MBSetLibSymName('jsonrpc')
libjsonrpc = libjsonrpcenv.SharedLibrary('jsonrpc', sources)

libjsonrpcenv.Clean(libjsonrpc, '#/obj')

libjsonrpcenv.MBInstallLib(libjsonrpc, 'jsonrpc')
libjsonrpcenv.MBInstallHeaders(libjsonrpcenv.MBGlob('#/src/main/include/jsonrpc/*'),
                               'jsonrpc')

libjsonrpcenv.MBCreateInstallTarget()
