# vim:ai:et:ff=unix:fileencoding=utf-8:sw=4:syntax=python:ts=4:

import os
import sys

USE_GCOV = False

env = Environment(ENV=os.environ)

if 'win32' == sys.platform:
    env.Tool('mb_mingw', toolpath=[Dir('submodules/mw-scons-tools')])
    env.Replace(CCFLAGS=[])

env.Append(CCFLAGS='-Wall')

libjsonrpcenv = env.Clone()
libjsonrpcenv.Append(CCFLAGS='-g')
libjsonrpcenv.Append(CCFLAGS='-pedantic')
libjsonrpcenv.Append(CCFLAGS='-Wextra')
libjsonrpcenv.Append(CCFLAGS='-Wno-variadic-macros')
libjsonrpcenv.Append(CCFLAGS='-Wno-long-long')
libjsonrpcenv.Append(CCFLAGS='-Werror')

libjsonrpcenv.Append(CPPPATH=[Dir('src/main/include/')])

libjsonrpcenv.Tool('mb_install', toolpath=[Dir('submodules/mw-scons-tools')])

libjsonrpcenv.MBAddDevelLibPath('#/../json-cpp/obj')
libjsonrpcenv.MBAddDevelIncludePath('#/../json-cpp/include')

libjsonrpcenv.Append(LIBS = 'jsoncpp')

libjsonrpc = libjsonrpcenv.SharedLibrary(
    'jsonrpc', [
        Glob('src/main/cpp/*.cpp'),])
libjsonrpcenv.Clean(libjsonrpc, '#\obj')

libjsonrpcenv.MBInstallLib(libjsonrpc)
libjsonrpcenv.MBInstallHeaders(libjsonrpcenv.MBGlob('#/src/main/include/*'))

libjsonrpcenv.MBCreateInstallTarget()

'''
testenv = cppenv.Clone()
testenv.AlwaysBuild('check')
testenv.Append(LIBS=[libjsonrpc, 'gtest', 'pthread'])
for node in Glob('#/obj/src/test/cpp/test-*.cpp'):
    root, ext = os.path.splitext(os.path.basename(node.abspath))
    test = testenv.Program(root, [node])
    alias = testenv.Alias('check', [test], test[0].abspath)
    testenv.AlwaysBuild(alias)
'''
