#!/bin/bash
echo "Compiling SvetOS Universal Builder..."
gcc -o tools/os-builder tools/os-builder.c
if [ $? -eq 0 ]; then
    echo "✓ Builder compiled successfully"
    chmod +x tools/os-builder
else
    echo "✗ Compilation failed"
    exit 1
fi
