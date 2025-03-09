#!/bin/bash 

meson builddir

ln -s $PWD/builddir/compile_commands.json $PWD/compile_commands.json