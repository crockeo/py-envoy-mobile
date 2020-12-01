#!/usr/bin/env bash

# this is meant to be executed from within bazel via bazel run //py_envoy_mobile:debug, it starts a
# gdb or lldb process on python3, depending on which are available
if command -v gdb &> /dev/null; then
    debugger=gdb
elif command -v lldb &> /dev/null; then
    debugger=lldb
else
    # TODO: allow a user to pass in a debugger
    echo "ERROR: neither gdb or lldb are available"
    exit 1
fi

$debugger -- python3 -m py_envoy_mobile
