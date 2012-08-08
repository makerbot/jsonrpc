# vim:ai:et:ff=unix:fileencoding=utf-8:sw=4:syntax=python:ts=4:

import os
import sys

USE_GCOV = False

env = Environment(ENV=os.environ)
env.Append(CCFLAGS='-Wall')

libjsonenv = env.Clone()
libjsonenv.Append(CPPPATH=[Dir('#/../json-cpp/include/')])
libjsonenv.Repository(Dir('#/../json-cpp/src/lib_json/'))
libjson = libjsonenv.Library(
    'json', [
        File('json_reader.cpp'),
        File('json_value.cpp'),
        File('json_writer.cpp'),])

libjsonrpcenv = env.Clone()
libjsonrpcenv.Append(CCFLAGS='-g')
libjsonrpcenv.Append(CCFLAGS='-pedantic')
libjsonrpcenv.Append(CCFLAGS='-Wextra')
libjsonrpcenv.Append(CCFLAGS='-Wno-variadic-macros')
libjsonrpcenv.Append(CCFLAGS='-Wno-long-long')
libjsonrpcenv.Append(CCFLAGS='-Werror')
libjsonrpcenv.Append(CPPPATH=[Dir('#/../json-cpp/include/')])
libjsonrpcenv.Append(CPPPATH=[Dir('src/main/include/')])
libjsonrpcenv.Repository(Dir('#/../json-cpp/src/lib_json/'))
libjsonrpc = libjsonrpcenv.Library(
    'jsonrpc', [
        Glob('src/main/cpp/*.cpp'),])

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
