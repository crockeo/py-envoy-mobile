#!/usr/bin/env bash

# this is meant to be executed from within bazel via bazel run //py_envoy_mobile:debug, it starts a
# gdb or lldb process on python3, depending on which are available
lldb -- python3 -m py_envoy_mobile.__init__
