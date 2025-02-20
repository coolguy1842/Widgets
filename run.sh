#!/bin/bash

dir="$(realpath $(dirname $0))"

meson setup builddir --reconfigure
cd builddir

meson compile -j12
env GTK_DEBUG=interactive GDK_BACKEND=wayland ./widgets --style $dir/styles/main.scss ${@,2}
# env GDK_BACKEND=wayland ./widgets --style $dir/styles/main.scss ${@,2}
# env GDK_BACKEND=wayland ./widgets ${@,2}
cd ..