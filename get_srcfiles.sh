#!/bin/sh

for i in src/*.cpp; do
  echo $i
done

for i in src/**/*.cpp; do
  echo $i
done
