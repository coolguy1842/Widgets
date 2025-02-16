#!/bin/bash

dir="$(realpath $(dirname $0))"

meson setup builddir --reconfigure
cd builddir

meson compile -j12
env GDK_BACKEND=wayland ./widgets --style $dir/styles/main.scss ${@,2}
cd ..