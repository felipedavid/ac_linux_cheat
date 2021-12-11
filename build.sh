#!/bin/bash
[[ ! -d "bin" ]] && mkdir bin
gcc main.c -o ./bin/ac_cheat -g -Og
