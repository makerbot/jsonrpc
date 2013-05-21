# vim:ai:et:ff=unix:fileencoding=utf-8:sw=4:syntax=python:ts=4:

AddOption('--debug_build', action='store_true', dest='debug_build')
debug = GetOption('debug_build')

Export('debug')

SConscript('SConscript', variant_dir='obj/', duplicate=1)
