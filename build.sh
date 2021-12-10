#!/bin/bash
[[ ! -d $DIR ]] && mkdir bin
gcc main.c -o ./bin/ac_cheat -g -Og
