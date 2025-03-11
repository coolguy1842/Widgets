#!/bin/bash

dir="$(realpath $(dirname $0))"

CC=clang CXX=clang++ meson setup builddir --reconfigure
cd builddir

CC=clang CXX=clang++ meson compile -j12
# env GTK_DEBUG=interactive GDK_BACKEND=wayland ./widgets --style $dir/styles/main.scss ${@,2}

function run_applet() {
    sleep 1
    nm-applet
}

(trap 'kill 0' SIGINT; env GDK_BACKEND=wayland ./widgets --style $dir/styles/main.scss ${@,2} & run_applet ; fg)
killall nm-applet

cd ..