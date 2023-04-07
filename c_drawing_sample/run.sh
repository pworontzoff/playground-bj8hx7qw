#!/bin/sh


gcc main.c drawing.c -o drawing -lm
./drawing

echo "TECHIO> open -s /project/target/ output.html"