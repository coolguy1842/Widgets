#!/bin/bash

./codegen.sh

dir="$(realpath $(dirname $0))"

CC=clang CXX=clang++ meson setup builddir --reconfigure
cd builddir

CC=clang CXX=clang++ meson compile -j12
# env GTK_DEBUG=interactive GDK_BACKEND=wayland ./widgets --style $dir/styles/main.scss ${@,2}
env GDK_BACKEND=wayland ./widgets --style $dir/styles/main.scss ${@,2}
cd ..