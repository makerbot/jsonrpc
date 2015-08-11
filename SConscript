# vim:ai:et:ff=unix:fileencoding=utf-8:sw=4:syntax=python:ts=4:

import os

optCC = 'gcc'
optCXX = 'g++'
optAR = 'ar'
optSTRIP = 'strip'

env = Environment(
    ENV=os.environ,
    tools=['default', 'mb_install'],
    toolpath=['#/../mw-scons-tools', '#/Install/mw-scons-tools'])

debug = ARGUMENTS.get('debug', 0)
target = ARGUMENTS.get('target', 0)
SYSROOT = ARGUMENTS.get('sysroot', 0)
TCPREFIX = ARGUMENTS.get('tcprefix', 0)
TCVERSION = ARGUMENTS.get('tcversion', 0)

if SYSROOT!=0:
	sysroot=SYSROOT

if TCPREFIX!=0:
	optCC=('%s-' % TCPREFIX) + optCC
	optCXX=('%s-' % TCPREFIX) + optCXX
	optAR=('%s-' % TCPREFIX) + optAR
	optSTRIP=('%s-' % TCPREFIX) + optSTRIP
	
if TCVERSION!=0:
	optCC=optCC + ('-%s' % TCVERSION)
	optCXX=optCXX + ('-%s' % TCVERSION)
	optAR=optAR + ('-%s' % TCPREFIX)
	
env_options = {
    "CC"    : optCC,
    "CXX"   : optCXX,
    "LD"    : optCXX,
    "AR"    : optAR,
    "STRIP" : optSTRIP
}

if int(debug):
    env.Append(CCFLAGS = '-g')

if target == 'arm':
	env.Replace(**env_options)
	if SYSROOT!=0:
		env.Append(CCFLAGS=['--sysroot=%s' % sysroot],
				   LINKFLAGS=['--sysroot=%s' % sysroot])			   
		env.Append(CCFLAGS=['-I%s/usr/include/arm-linux-gnueabihf/c++/4.9' % sysroot])
		env.Append(CCFLAGS=['-I%s/usr/include/c++/4.9' % sysroot])

env.MBAddStandardCompilerFlags()

env.MBAddIncludePaths([
    Dir('src/main/include/'),
    Dir('src/main/')])

env.MBDependsOnJsonCpp()

libjsonrpc = env.MBSharedLibrary(
    'jsonrpc', [Glob('src/main/cpp/*.cpp')])
env.Clean(libjsonrpc, '#/obj')

env.MBInstallLib(libjsonrpc, 'jsonrpc')
env.MBInstallHeaders(env.MBGlob('#/src/main/include/jsonrpc/*'), 'jsonrpc')

env.MBCreateInstallTarget()
