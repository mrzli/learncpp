#!/usr/bin/env bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"/src

g++ \
  -std=c++20 \
  -fdiagnostics-color=always \
  -g \
  -pedantic-errors \
  -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion -Werror \
  main.cpp \
  -L/home/mrzli/projects/games/vulkan-sdk/x86_64/lib -lvulkan -lglfw \
  -o ../build/main
