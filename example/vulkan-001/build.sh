#!/usr/bin/env bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

g++ -std=c++20 main.cpp -L/home/mrzli/projects/games/vulkan-sdk/x86_64/lib -lvulkan -o main
